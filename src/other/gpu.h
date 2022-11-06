#ifndef OTHER_GPU_H
#define OTHER_GPU_H

#define CL_TARGET_OPENCL_VERSION 220

#include <CL/cl.h>

#include "infsys.h"

struct gpu_info
{
    char name[128];
    char vendor[64];
};

static void gpu_identify(struct gpu_info *info)
{
    cl_platform_id platform_id = NULL;
    cl_device_id device = NULL;   

    cl_int ret = clGetPlatformIDs(1, &platform_id, NULL);
    
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, 
            &device, NULL);

    if (ret == CL_SUCCESS)
    {
        clGetDeviceInfo(device, CL_DEVICE_NAME, 128,
                        info->name, NULL);
        
        clGetDeviceInfo(device, CL_DEVICE_VENDOR, 64,
                        info->vendor, NULL);
    }
}

#endif /* OTHER_GPU_H */