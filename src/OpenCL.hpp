#ifndef OpenCL_hpp
#define OpenCL_hpp

#include <CL/cl.h>

signed OpenCL_SetError(const char *string, cl_int error);

cl_platform_id *OpenCL_GetPlatformIDs();
cl_device_id *OpenCL_GetDeviceIDs(cl_device_type type = CL_DEVICE_TYPE_DEFAULT);
cl_context OpenCL_GetContext(cl_context_properties *properties = nullptr);
cl_command_queue OpenCL_GetCommandQueue(cl_command_queue_properties properties = 0);

#endif//file
