
#ifndef __ACCELERATED_PINHOLE_CAMERA_HPP
#define __ACCELERATED_PINHOLE_CAMERA_HPP

#include <iostream>
#include <OpenCL/cl.h>

#include "Camera.hpp"

class AcceleratedPinholeCamera : public Camera {
    public:
        AcceleratedPinholeCamera(Point centre,Orientation orientation,Velocity velocity);
        std::vector<Point> Trace(Distance distance);
    private:
        cl_Point *_h_out;
        cl_kernel _kernel;
        cl_mem _d_out;
        
};

#endif /* defined(__RayTracer__CameraKernelWrapper__) */
