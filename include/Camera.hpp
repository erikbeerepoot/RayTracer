#ifndef __CAMERA_HPP
#define __CAMERA_HPP
/**
 * Filename:	Camera.hpp 
 * Purpose:		Interface for Camera class. 
 * Author:		Erik E. Beerepoot
 */
#include "GeometricTypes.hpp"
 
#include <vector>
 
struct FOV {
	public:
		Angle vertical;
		Angle horizontal;
		
		FOV() : vertical(0.698_rad), horizontal(0.524_rad) {};
};
 
struct PixelPitch {
	public:
		Distance vertical;
		Distance horizontal;
		
		PixelPitch() : vertical(0.004_m) , horizontal(0.003_m) {};
};
 
struct Resolution {
	public:
		int vertical;
		int horizontal;
		
		Resolution() : vertical(480), horizontal(480) {};
};

struct Sensor {
	public:
		PixelPitch pitch;
		Resolution resolution;
		
		Sensor() {};
};


class Camera {
	public:
		//Camera parameters
		Sensor		sensor;
		FOV			fieldOfView;
		int			framerate;

		Camera(Point centre,Orientation orientation,Velocity velocity);
		//virtual std::vector<Point> TraceRay(int u,int v,Distance distance) const = 0;
	protected:
		//Euclidian params
		Point 		_centre;
		Orientation _orientation;
		Velocity 	_velocity;
        Time        _samplingTime;
};

class PinholeCamera : public Camera {
	public:
		PinholeCamera(Point centre,Orientation orientation,Velocity velocity);
		std::vector<Point> TraceRay(int u,int v,Distance distance) const;
    
        void TraceRay(int& u,int& v,Distance& distance,std::vector<Point> &points) const;
        void UpdatePosition();
};

 #endif