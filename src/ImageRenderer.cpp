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
 * /name    ImageRenderer
 * /brief   Class that implements the actual rendering of a scene, renders to an image.
 * /author  Erik E. Beerepoot
 */
#include "ImageRenderer.hpp"
#include "Scene.hpp"
#include "AcceleratedPinholeCamera.hpp"
#include "Camera.hpp"
#include "GenericTypes.hpp"
#include "GeometricTypes.hpp"

#include <sstream>
#include <iostream>
#include <thread>
#include <png.h>
 
const std::string kVersionString = "v0.2";
const Distance kRayLength = 5.0_m;
const int kNumThreads = 16;
 
 int main(int argc, char**arv){
	//print welcome message
	std::cout << "RayTracer " << kVersionString << " by Erik E. Beerepoot" << std::endl;
	
	//Construct scene
	Scene scene(Size(4.0_m,4.0_m,2.0_m));
    pixel_t color;
    color.red = 136;
    color.green = 136;
    color.blue = 136;
    scene.AddPlane(Point(3.01_m,0.0_m,0.1_m),Point(3.01_m,4.0_m,2.0_m),color);
     
    //Zebra stripe "target"
	color.red = 1;
	color.green = 1;
	color.blue = 1;
	scene.AddPlane(Point(3.0_m,1.0_m,0.5_m),Point(3.0_m,1.1_m,1.5_m),color);
	scene.AddPlane(Point(3.0_m,1.2_m,0.5_m),Point(3.0_m,1.3_m,1.5_m),color);
	scene.AddPlane(Point(3.0_m,1.4_m,0.5_m),Point(3.0_m,1.5_m,1.5_m),color);
	scene.AddPlane(Point(3.0_m,1.6_m,0.5_m),Point(3.0_m,1.7_m,1.5_m),color);
	scene.AddPlane(Point(3.0_m,1.8_m,0.5_m),Point(3.0_m,1.9_m,1.5_m),color);

	color.red = 255;
	color.green = 255;
	color.blue = 255;
	scene.AddPlane(Point(3.0_m,1.1_m,0.5_m),Point(3.0_m,1.2_m,1.5_m),color);
    scene.AddPlane(Point(3.0_m,1.3_m,0.5_m),Point(3.0_m,1.4_m,1.5_m),color);
	scene.AddPlane(Point(3.0_m,1.5_m,0.5_m),Point(3.0_m,1.6_m,1.5_m),color);
	scene.AddPlane(Point(3.0_m,1.7_m,0.5_m),Point(3.0_m,1.8_m,1.5_m),color);
	scene.AddPlane(Point(3.0_m,1.9_m,0.5_m),Point(3.0_m,2.0_m,1.5_m),color);
     
// ***************************************
//             VICON LAB SIM
// ***************************************
//	pixel_t color;
//	color.red = 255;
//	color.green = 0;
//	color.blue = 0;
//	scene.AddPlane(Point(2.2_m,0.0_m,0.5_m),Point(2.2_m,0.1_m,2.0_m),color);
//	scene.AddPlane(Point(2.2_m,0.2_m,0.5_m),Point(2.2_m,0.3_m,2.0_m),color);
//	scene.AddPlane(Point(2.2_m,0.4_m,0.5_m),Point(2.2_m,0.5_m,2.0_m),color);
//	scene.AddPlane(Point(2.2_m,0.6_m,0.5_m),Point(2.2_m,0.7_m,2.0_m),color);
//	scene.AddPlane(Point(2.2_m,0.8_m,0.5_m),Point(2.2_m,0.9_m,2.0_m),color);
//	
//	color.red = 0;
//	color.green = 0;
//	color.blue = 255;
//	scene.AddPlane(Point(2.2_m,0.1_m,0.5_m),Point(2.2_m,0.2_m,2.0_m),color);
//	scene.AddPlane(Point(2.2_m,0.3_m,0.5_m),Point(2.2_m,0.4_m,2.0_m),color);
//	scene.AddPlane(Point(2.2_m,0.5_m,0.5_m),Point(2.2_m,0.6_m,2.0_m),color);
//	scene.AddPlane(Point(2.2_m,0.7_m,0.5_m),Point(2.2_m,0.8_m,2.0_m),color);
//	scene.AddPlane(Point(2.2_m,0.9_m,0.5_m),Point(2.2_m,1.0_m,2.0_m),color);
//	
//	
//	color.red = 136;
//	color.green = 136;
//	color.blue = 136;
//	scene.AddPlane(Point(0.0_m,0.0_m,0.0_m),Point(4.0_m,2.5_m,0.0_m),color);
//	
//	color.red = 212;
//	color.green = 212;
//	color.blue = 212;
//	scene.AddPlane(Point(0.0_m,0.0_m,0.0_m),Point(4.0_m,0.0_m,2.0_m),color);
//	
//	color.red = 200;
//	color.green = 200;
//	color.blue = 200;
//	scene.AddPlane(Point(3.99_m,0.0_m,0.0_m),Point(3.99_m,2.49_m,2.0_m),color);
		
	//Create camera
	Point camCentre(0.0_m,1.5_m,1.0_m);
	Orientation camOrientation(0.0_rad,0.0_rad,6.2_rad);
	Velocity camVelocity(0.0_m/1.0_s,0.0_m/1.0_s,0.0_m/1.0_s,0.0_rad/1.0_s,0.0_rad/1.0_s,0.0_rad/1.0_s);
	PinholeCamera cam(camCentre,camOrientation,camVelocity);
	
	//Render the scene
    //ImageRenderer renderer("c:\\RayTracer\\output\\");
    ImageRenderer renderer("~");

    renderer.RenderScene(scene,&cam);
	return SUCCESS;
 }
 
 /** 
  * /name 	ImageRenderer
  * /brief	Constructor for ImageRenderer. Takes the destination image path as a parameter.
  * /param	destImagePath - The path of the image to be rendered to.
  */
 ImageRenderer::ImageRenderer(std::string destPath) : _outputPath(destPath){}

 /** 
  * /name 	RenderScene (overloaded method)
  * /brief	Renders the scene to an image. Returns 0 on success.
  * /param	scene - The scene object to render.
  * /param	camera - The camera object used to view the scene.
  * /notes	Side effect: Creates image containing rendering for each camera, appending a sequence number.  
  */

int ImageRenderer::RenderScene(const Scene& scene,PinholeCamera* camera){
	static long renderNum = 1;	
	
	//Create output path
	std::stringstream ss;
	ss << _outputPath << "render-" << renderNum << ".png";
	
	//Create output image
	PNGImage img(ss.str(),480,640);
    
	/* 
	* For every pixel:
	* Shoot ray from origin of pixel in space, in a particular direction, for a particular distance.
	* If the ray encounters a scene object, return the reflected colour. Else, return black (?).
	*/
	pixel_t emptyPix;
	emptyPix.red = 255;
    
    for(int y=0;y < camera->sensor.resolution.vertical;y+=1){
		for(int x=0;x < camera->sensor.resolution.horizontal;x+=1){
            //generate ray trajectory
            std::vector<Point> points = camera->TraceRay(x,y,kRayLength);
            
            //update camera position
            camera->UpdatePosition();
                                   
            //Check for intersection with scene, write to image
            img.SetPixel(x,y,scene.CheckPoints(points));
		}
	}
	int result = img.Write();

	return SUCCESS;
}

void ThreadedTraceRay(int x,int y,const Scene &scene,const PinholeCamera* phc,PNGImage *img){

}

int ImageRenderer::RenderScene(const Scene& scene,AcceleratedPinholeCamera* camera){
    Distance distance(kRayLength);
    
    
    std::vector<Point> points = camera->Trace(distance);
    
    //Check for intersection with scene, write to image
    //img.SetPixel(x,y,scene.CheckPoints(points));
    
}

 /**
  * /name 	RenderScene (overloaded method)
  * /brief	Renders the scene to an image. Returns 0 on success.
  * /param	scene - The scene object to render.
  * /param	cameras - The camera objects used to view the scene.
  * /notes	Side effect: Creates image containing rendering for each camera, appending a sequence number.
  */
int ImageRenderer::RenderScene(const Scene& scene,const std::vector<Camera*> cameras){
	if(cameras.size()==0) return ERROR;
	return ERROR;
    
//	//Render the scene for each camera
//	for(auto it=cameras.begin();it!=cameras.end();++it){
//		RenderScene(scene,cameras.at(0));	
//	}
}

/**
 * /name	CancelRendering 
 * /brief	Cancels any currently in progress, returns 0 on success.
 */
int ImageRenderer::CancelRendering(){
	//cancel the rendering
	return ERROR;
}
 
 