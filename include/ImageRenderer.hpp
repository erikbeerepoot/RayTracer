#ifndef __IMAGERENDERER_HPP
#define __IMAGERENDERER_HPP
/**
 * Filename:	ImageRenderer.hpp 
 * Purpose:		Interface for ImageRenderer class. Performs the ray-tracing magic.
 * Author:		Erik E. Beerepoot
 */
#include "Camera.hpp"
#include "AcceleratedPinholeCamera.hpp"
#include "Scene.hpp"
#include "PNGImage.hpp"

#include <vector>
#include <string>
 
 class Renderer {
	public:
//			virtual int RenderScene(const Scene& scene,const Camera* camera) = 0;
//			virtual int RenderScene(const Scene& scene,const std::vector<Camera*> cameras) = 0;
			virtual int CancelRendering() = 0;
 };
 
 /* Notes: The ImageRenderer class just generates an image of the scene. There are other options here, such as 
  * using an OpenCL based renderer / ray-tracer, or outputting to the screen, as opposed to an image */
 class ImageRenderer : public Renderer {
	public:
			ImageRenderer(std::string destImgPath);
           
			int RenderScene(const Scene& scene, PinholeCamera* camera);
            int RenderScene(const Scene& scene,AcceleratedPinholeCamera* camera);
			int RenderScene(const Scene& scene,const std::vector<Camera*> cameras);
			int CancelRendering();
	private:
			std::string _outputPath;
			
 };
 #endif