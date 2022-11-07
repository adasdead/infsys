#ifndef HARDWARE_GPU_H
#define HARDWARE_GPU_H

#define CL_TARGET_OPENCL_VERSION 220

#include <CL/cl.h>

#include "infsys.h"

#define GPU_NAME_SIZE   128
#define GPU_VENDOR_SIZE 64

struct gpu_info
{
    char name[GPU_NAME_SIZE];
    char vendor[GPU_VENDOR_SIZE];
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
        clGetDeviceInfo(device, CL_DEVICE_NAME, GPU_NAME_SIZE,
                        info->name, NULL);
        
        clGetDeviceInfo(device, CL_DEVICE_VENDOR, GPU_VENDOR_SIZE,
                        info->vendor, NULL);
    }
}

#endif /* HARDWARE_GPU_H */