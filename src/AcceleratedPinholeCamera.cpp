/**
 *      ___           ___           ___           ___
 *     /\  \         /\  \         /\  \         /\__\
 *    /::\  \       /::\  \       /::\  \       /:/  /
 *   /:/\:\  \     /:/\ \  \     /:/\:\  \     /:/  /
 *  /::\~\:\  \   _\:\~\ \  \   /::\~\:\  \   /:/  /
 * /:/\:\ \:\__\ /\ \:\ \ \__\ /:/\:\ \:\__\ /:/__/
 * \/__\:\/:/  / \:\ \:\ \/__/ \/_|::\/:/  / \:\  \
 *      \::/  /   \:\ \:\__\      |:|::/  /   \:\  \
 *      /:/  /     \:\/:/  /      |:|\/__/     \:\  \
 *     /:/  /       \::/  /       |:|  |        \:\__\
 *     \/__/         \/__/         \|__|         \/__/
 * -----------------------------------------------------
 * ----------> Autonomous Space Robotics Lab <----------
 * -----------------------------------------------------
 * /name    AcceleratedPinholeCamera
 * /brief   Implements OpenCL accelerated spherical camera
 * /author  Erik E. Beerepoot
 */

#include "AcceleratedPinholeCamera.hpp"
#include "ComputeManager.hpp"

const std::string kKernelSourceFile = "../kernels/CameraKernels.cl";
const std::string kKernelName = "traceray";
const float kSpatialSamplingDistance = (float)0.05;
/**
 * /name PinholeCamera
 * /brief Constructor for Camera object. Sets sensible defaults, using common parameters for a CCD image sensor and camera.
 */
AcceleratedPinholeCamera::AcceleratedPinholeCamera(Point centre,Orientation orientation,Velocity velocity) : Camera(centre,orientation,velocity) {
	//default camera is DSLR like (4mmx3mm sensor, 640x480)
	sensor.pitch.vertical = 0.000004_m;
	sensor.pitch.horizontal = 0.000003_m;
	sensor.resolution.vertical = 480;
	sensor.resolution.horizontal = 640;
	
	// Field of view: 40 deg x 30 deg
	fieldOfView.vertical = 0.524_rad;
	fieldOfView.horizontal = 0.698_rad;
	
	// Framerate: ~real time
	framerate = 30;
    
    ComputeManager *mgr = ComputeManager::SharedComputeManager();
    
    //Build kernel
    if(mgr->BuildKernelFromFile(kKernelSourceFile,kKernelName,&_kernel)==false){
        std::cout << "Failed to build kernel. Exiting..." << std::endl;
        exit(1);
    }
}

std::vector<Point> AcceleratedPinholeCamera::Trace(Distance distance) {
    std::vector<Point> points;
    
    ComputeManager *mgr = ComputeManager::SharedComputeManager();
    if(mgr==NULL) return points;
    
    //Allocate host memory
    int numPoints = static_cast<int>(distance.get()/kSpatialSamplingDistance);
    std::cout << "Num points: " << numPoints << std::endl;
    _h_out = (cl_Point*)malloc(sensor.resolution.horizontal*numPoints*sizeof(cl_Point));
    if(_h_out==NULL){
        std::cout << "Failed to allocate host buffer..." << std::endl;
        return std::vector<Point>();
    }

    //Allocate device memory
    mgr->AllocateBufferOfSize(sensor.resolution.horizontal*numPoints*sizeof(cl_Point),&_d_out,(void*)_h_out,CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR);
    if(_h_out==NULL){
        std::cout << "Failed to allocate host buffer..." << std::endl;
        return points;
    }
    
    //Centre pixel
	static int u_c = sensor.resolution.horizontal / 2;
	static int v_c = sensor.resolution.vertical / 2;
	
	//Angular difference / ray
	static Angle diff_u = ((fieldOfView.horizontal / 2) / u_c);
	static Angle diff_v = ((fieldOfView.vertical / 2) / v_c);
    
    //get centre
    cl_Point centre;
    Distance t =  (_centre.x);
    centre.x = t.get();
    t =  (_centre.y);
    centre.y = t.get();
    t =  (_centre.z);
    centre.z = t.get();
    
    //Save sensor pitch
    t = sensor.pitch.horizontal;
    float p_hor = t.get();
    t = sensor.pitch.vertical;
    float p_vert = t.get();
    
    //Save other vars
    float dist = (float)distance.get();
    float f_diff_u = (float)diff_u.get();
    float f_diff_v = (float)diff_v.get();
    int row = 1;
    cl_int err = 0;
    
    //Set kernel parameters
    err |= clSetKernelArg(_kernel, 0, sizeof(float), &dist);
    err |= clSetKernelArg(_kernel, 1, sizeof(int), &row);
    err |= clSetKernelArg(_kernel, 2, sizeof(int), &u_c);
    err |= clSetKernelArg(_kernel, 3, sizeof(int), &v_c);
    err |= clSetKernelArg(_kernel, 4, sizeof(float), &f_diff_u);
    err |= clSetKernelArg(_kernel, 5, sizeof(float), &f_diff_v);
    err |= clSetKernelArg(_kernel, 6, sizeof(float), &p_hor);
    err |= clSetKernelArg(_kernel, 7, sizeof(float), &p_vert);
    err |= clSetKernelArg(_kernel, 8, sizeof(cl_Point), &_centre);
    err |= clSetKernelArg(_kernel, 9, sizeof(cl_mem), &_d_out);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to set kernel arguments! %d\n", err);
        return points;
    }
    
    //Get pointer to queue
    cl_command_queue q = mgr->CommandQueue();
    
    //Set kernel parameters
    const size_t width = static_cast<size_t>(sensor.resolution.horizontal);
    const size_t global[2] = {width,1};
    const size_t local = 1;

    //queue kernel for execution
    err = clEnqueueNDRangeKernel(q,_kernel,2,NULL,&global[0],NULL,0,NULL,NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to enqueue kernel! %d\n", err);
        return points;
    }
    //wait for kernel to finish
    clFinish(q);
    
    //Read back the results from the device to verify the output
    err = clEnqueueReadBuffer( q, _d_out, CL_TRUE, 0, sensor.resolution.horizontal*numPoints*sizeof(cl_Point),_h_out,0,  NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to enqueue read buffer! %d\n", err);
        return points;
    }

#warning continue here
    /*
    Point tmpPoint(0.0_m,0.0_m,0.0_m);
    for(int index=0;index<width*numPoints;index++){
        tmpPoint.x = _h_out[index].x;
        tmpPoint.y = _h_out[index].y;
        tmpPoint.z = _h_out[index].z;
        
        //points.push_back(
    }
    */
    
    printf("(%f,%f,%f),",_h_out[1].x,_h_out[1].y,_h_out[1].z);
    printf("(%f,%f,%f),",_h_out[40000].x,_h_out[40000].y,_h_out[2000].z);
    

    return points;
}