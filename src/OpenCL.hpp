/** \file
 * Utilities for working with OpenAL:
 * 		Reporting of errors through SDL
 * 		Audio device and context singletons for automatic resource management
 */

#ifndef OpenCL_hpp
#define OpenCL_hpp

#include <CL/cl.h>

namespace OpenCL
{
	/// Set the SDL error string to the given error code
	signed SetError(const char *origin, cl_int error);

	cl_platform_id *GetPlatformIDs();
	cl_device_id *GetDeviceIDs(cl_device_type type = CL_DEVICE_TYPE_DEFAULT);
	cl_context GetContext(cl_context_properties *properties = nullptr);
	cl_command_queue GetCommandQueue(cl_command_queue_properties properties = 0);
}

#endif//file
