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
 * /name    Camera
 * /brief   Implements details of a spherical camera in c++.
 * /author  Erik E. Beerepoot
 */

#include "Camera.hpp"
#include "ComputeManager.hpp"
#include <iostream>

const Distance kSpatialSamplingDistance = 0.005_m;
const Angle kPi = 3.14159265358979_rad;

/**
 * /name Camera
 * /brief Constructor for Camera object. Sets sensible defaults, using common parameters for a CCD image sensor and camera.
 */
Camera::Camera(Point centre,Orientation orientation,Velocity velocity) : _centre(centre), _orientation(orientation), _velocity(velocity) , _samplingTime(0.0_s) {
	//default camera is DSL like (4mmx3mm sensor, 640x480)
	sensor.pitch.vertical = 0.004_m;
	sensor.pitch.horizontal = 0.003_m;
	sensor.resolution.vertical = 480;
	sensor.resolution.horizontal = 640;
	
	// Field of view: 40 deg x 30 deg
	fieldOfView.vertical = 0.524_rad;
	fieldOfView.horizontal = 0.698_rad;
	
	// Framerate: ~real time
	framerate = 2;
    _samplingTime = Time(double(1/double(framerate * sensor.resolution.vertical * sensor.resolution.horizontal)));
}

/**
 * /name PinholeCamera
 * /brief Constructor for Camera object. Sets sensible defaults, using common parameters for a CCD image sensor and camera.
 */
PinholeCamera::PinholeCamera(Point centre,Orientation orientation,Velocity velocity) : Camera(centre,orientation,velocity) {
	//default camera is DSLR like (4mmx3mm sensor, 640x480)
	sensor.pitch.vertical = 0.000004_m;
	sensor.pitch.horizontal = 0.000003_m;
	sensor.resolution.vertical = 480;
	sensor.resolution.horizontal = 640;
	
	// Field of view: 40 deg x 30 deg
	fieldOfView.vertical = 0.524_rad;
	fieldOfView.horizontal = 0.698_rad;
	
	// Framerate: ~real time
	framerate = 2;
    _samplingTime = Time(double(1/double(framerate * sensor.resolution.vertical * sensor.resolution.horizontal)));
}

/**
 * /name TraceRay
 * /brief Traces a ray from the pixel at (u,v) for "distance" metres. Returning a vector of points the ray "visits"
 */
std::vector<Point> PinholeCamera::TraceRay(int u,int v,Distance distance) const {
	//Centre pixel
	static int u_c = sensor.resolution.horizontal / 2;
	static int v_c = sensor.resolution.vertical / 2;
	
	//Angular difference / ray
	static Angle diff_u = ((fieldOfView.horizontal / 2) / u_c);
	static Angle diff_v = ((fieldOfView.vertical / 2) / v_c);
	
	//Calculate current angle relative to image plane	
	Angle theta_u = diff_u * (u_c - u);
	Angle theta_v = diff_v * (v - v_c) + (kPi/2);
	
	//Calculate origin of this point relative to image plane
	Distance y = sensor.pitch.horizontal * (u - u_c);
	Distance z = sensor.pitch.vertical * (v - v_c);
	
	Point pixelLocation(_centre.x,_centre.y + y,_centre.z + z);	
	Point pixelStep = AzInclRangeToXYZ(theta_u,theta_v,kSpatialSamplingDistance);
	Point nextPixel = pixelLocation;
	std::vector<Point> points;
	
	for(Distance d=0.0_m;d<=distance;d=d+kSpatialSamplingDistance){
		nextPixel = nextPixel + pixelStep;
		points.push_back(nextPixel);
	}
    return points;
}

/**
 * /name TraceRay
 * /brief Traces a ray from the pixel at (u,v) for "distance" metres. Returning a vector of points the ray "visits"
 */
void PinholeCamera::TraceRay(int& u,int& v,Distance& distance,std::vector<Point> &points) const {
	//Centre pixel
	static int u_c = sensor.resolution.horizontal / 2;
	static int v_c = sensor.resolution.vertical / 2;
	
	//Angular difference / ray
	static Angle diff_u = ((fieldOfView.horizontal / 2) / u_c);
	static Angle diff_v = ((fieldOfView.vertical / 2) / v_c);
	
	//Calculate current angle relative to image plane
    Angle theta_u = diff_u * (u_c - u) + _orientation.yaw;
	Angle theta_v = diff_v * (v - v_c) + (kPi/2) + _orientation.pitch;
	
	//Calculate origin of this point relative to image plane
	Distance y = sensor.pitch.horizontal * (u - u_c);
	Distance z = sensor.pitch.vertical * (v - v_c);
	
	Point pixelLocation(_centre.x,_centre.y + y,_centre.z + z);
	Point pixelStep = AzInclRangeToXYZ(theta_u,theta_v,kSpatialSamplingDistance);
	Point nextPixel = pixelLocation;
	
	for(Distance d=0.0_m;d<=distance;d=d+kSpatialSamplingDistance){
		nextPixel = nextPixel + pixelStep;
		points.push_back(nextPixel);
	}
}

/**
 * /name    UpdatePosition
 * /brief   Updates the position of this camera object, assuming a single ray has been shot
 */
void PinholeCamera::UpdatePosition(){
    //Update position
    _centre.x = _centre.x + _samplingTime * _velocity.V_x;
    _centre.y = _centre.y + _samplingTime * _velocity.V_y;
    _centre.z = _centre.z + _samplingTime * _velocity.V_z;
    
    //Update orientation
    _orientation.roll = _orientation.roll + _samplingTime * _velocity.w_x;
    _orientation.pitch = _orientation.pitch + _samplingTime * _velocity.w_y;
    _orientation.yaw = _orientation.yaw + _samplingTime * _velocity.w_z;
}

