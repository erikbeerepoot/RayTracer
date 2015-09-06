//
//  ComputeManager.h
//  RayTracer
//
//  Created by Erik E. Beerepoot on 2012-08-28.
//  Copyright (c) 2012 Erik E. Beerepoot. All rights reserved.
//

#ifndef RayTracer_ComputeManager_h
#define RayTracer_ComputeManager_h

#include <string>
#include <cstring>
#include <OpenCL/cl.h>

class ComputeManager {
    public:
        static ComputeManager* SharedComputeManager();
    
        bool BuildKernelFromFile(std::string sourceFilePath,std::string kernelName,cl_kernel *kernel);
        void AllocateBufferOfSize(size_t size,cl_mem *deviceMem,void *hostMem, int mode);
        cl_command_queue            CommandQueue();
    private:
        ComputeManager();
    	static ComputeManager 		*sharedComputeManager;

        bool ConfigureComputationFramework();
       
};

#endif
