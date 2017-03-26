#ifndef OpenCL_hpp
#define OpenCL_hpp

#include <CL/cl.h>

signed OpenCL_SetError(const char *string, cl_int error);

cl_platform_id *OpenCL_GetPlatformIDs();
cl_device_id *OpenCL_GetDeviceIDs(cl_device_type type, cl_platform_id platform = nullptr);

#endif//file
