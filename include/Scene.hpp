#ifndef __SCENE_HPP
#define __SCENE_HPP
/**
 * Filename:	Scene.hpp 
 * Purpose:		Interface for Scene class. Allows the construction of a fixed size scene and the addition of primitives.
 * Author:		Erik E. Beerepoot
 */
 #include "GeometricTypes.hpp"
 
 #include <vector>
 
 class Scene {
	public:
		Scene();
		Scene(Size size);
		~Scene();
		
		pixel_t CheckPoints(std::vector<Point>& points) const;
		void AddPlane(Point upperLeft,Point lowerRight,pixel_t color);
		void AddRightCuboid(Point centroid, Size size);
	private:
		void AllocScene();
		void DeallocScene();
		
		bool ClipPoint(Point& point) const;
		void ClipRightCuboid(Point& centroid, Size& size);
		pixel_t* At(Point pix);
		
		pixel_t*** _sceneData;
		Size _sceneSize;
		Size _gridDim;
		
		pixel_t operator() (Point pix) { 
			int idx,idy,idz;
			idx = (pix.x / _gridDim.length).get();
			idy = (pix.y / _gridDim.width).get();
			idz = (pix.z / _gridDim.height).get();
			return _sceneData[idx][idy][idz];
		};
 };
 #endif