/** \file
 * Utilities for working with OpenAL:
 * 		Reporting of errors through SDL
 * 		Audio device and context singletons for automatic resource management
 */

#ifndef OpenCL_hpp
#define OpenCL_hpp

#include <CL/cl.h>

/// Set the SDL error string to the given error code
signed OpenCL_SetError(const char *origin, cl_int error);

cl_platform_id *OpenCL_GetPlatformIDs();
cl_device_id *OpenCL_GetDeviceIDs(cl_device_type type = CL_DEVICE_TYPE_DEFAULT);
cl_context OpenCL_GetContext(cl_context_properties *properties = nullptr);
cl_command_queue OpenCL_GetCommandQueue(cl_command_queue_properties properties = 0);

#endif//file
