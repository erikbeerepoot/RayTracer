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
 * /name    ComputeManager
 * /brief   C++ abstraction layer for low-level OpenCL code
 * /author  Erik E. Beerepoot
 */
#define DEBUG

#include "ComputeManager.hpp"

#include <stdio.h>
#include <iostream>
#include <fstream>


ComputeManager *ComputeManager::sharedComputeManager = NULL;

const int kMaxNumDevices = 5;
const int kMaxNumPlatforms = 5;

cl_device_id _devices[kMaxNumDevices];
cl_platform_id _platforms[kMaxNumPlatforms];
cl_context _context;
cl_command_queue _queue;

ComputeManager::ComputeManager(){
	if(ConfigureComputationFramework()==false){
        std::cout << "Failed to configure computation framework! Exiting..." << std::endl;
        exit(1);
    }
}

ComputeManager *ComputeManager::SharedComputeManager(){
	if(sharedComputeManager==NULL)sharedComputeManager = new ComputeManager();
	return sharedComputeManager;
}

bool ComputeManager::ConfigureComputationFramework(){
    cl_int ret;
        
    //get available platforms
    cl_uint platformCount;
    ret = clGetPlatformIDs(kMaxNumPlatforms,_platforms,&platformCount);
    if(ret!=CL_SUCCESS) return false;
    
    //get all opencl devices on the system
    cl_device_type deviceType = CL_DEVICE_TYPE_GPU;
    cl_uint numDevices;
    
    ret = clGetDeviceIDs(_platforms[0],deviceType,kMaxNumDevices,_devices,&numDevices);
    if(ret!=CL_SUCCESS) return false;
    
    //print device info if required
    #ifdef DEBUG
    char name[50];
    for(int idx=0;idx<numDevices;idx++){

        ret = clGetDeviceInfo(_devices[idx],CL_DEVICE_NAME,(size_t)50,(void*)name,NULL);
        printf("Device name: %s \n", name);
    }
    #endif

    
    _context = clCreateContext(NULL, 1, _devices, NULL, NULL, &ret);
    if (ret != CL_SUCCESS) return ret;
    
    // Command-queue
    _queue = clCreateCommandQueue(_context, _devices[0], 0, &ret);
    if (ret != CL_SUCCESS) return ret;
    return true;
}

void ComputeManager::AllocateBufferOfSize(size_t size,cl_mem *deviceMem,void *hostMem, int mode){
    cl_int error;

    //create gpu buffer
    (*deviceMem) = clCreateBuffer(_context,mode,size,hostMem,&error);
    
    //if errors were encountered, make sure we can't use the mem pointers
    if(error!=CL_SUCCESS){
        printf("Error: failed allocate cl_mem %d\n", error);

        deviceMem = NULL;
        hostMem = NULL;
    }
}

bool ComputeManager::BuildKernelFromFile(const std::string sourceFilePath,const std::string kernelName,cl_kernel *kernel){
	char *sourceString = NULL;
	size_t length = 0;
    cl_int error;
    
    /* Step 1: Load the file
     * First, we open a new file, and check if it is valid */
    std::ifstream sourceFileStream(sourceFilePath.c_str(),std::ios_base::in);
    if(sourceFileStream.fail()) return false;

    //we get length of file
    sourceFileStream.seekg(0,std::ios::end);
    length  = sourceFileStream.tellg();
    sourceFileStream.seekg(0,std::ios::beg);
    
    
    //we read in the entire sourcefile
    sourceString = new char[length];
    if(sourceString==NULL) return false;
    
    sourceFileStream.read(sourceString,length);
    sourceFileStream.close();
    
    //Step 2 & 3: Create & build program from source string (using the first context)
    cl_program prog = clCreateProgramWithSource(_context,1,(const char**)&sourceString,&length,&error);
    if(error!=CL_SUCCESS) return false;
    
    error = clBuildProgram(prog, 1, &_devices[0], NULL, NULL, NULL);
    if(error!=CL_SUCCESS){
        //Show results of build
        // Shows the log
        char* build_log;
        size_t log_size;

        //Get the size for the buffer that must hold the build results
        clGetProgramBuildInfo(prog, _devices[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        build_log = new char[log_size+1];

        //Get the build results
        clGetProgramBuildInfo(prog,_devices[0], CL_PROGRAM_BUILD_LOG,log_size,build_log,NULL);
        build_log[log_size] = '\0';
        std::cout << build_log << std::endl;
        return false;
    }

    //Create the kernel
    *kernel = clCreateKernel(prog,kernelName.c_str(),&error);
    if(error!=CL_SUCCESS) return false;
	return true;
}

cl_command_queue  ComputeManager::CommandQueue(){
    return _queue;
}

