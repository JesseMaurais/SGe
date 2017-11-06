/** \file
 * Utility functions for working with OpenCL.
 */

#ifndef OpenCL_hpp
#define OpenCL_hpp

#include <CL/cl.h>

namespace OpenCL
{
	/// Set the SDL error string to the given error code. Always returns true.
	bool SetError(const char *origin, cl_int error);

	cl_platform_id *GetPlatformIDs();
	cl_device_id *GetDeviceIDs(cl_device_type type = CL_DEVICE_TYPE_DEFAULT);
	cl_context GetContext(cl_context_properties const *properties = nullptr);
	cl_command_queue GetCommandQueue(cl_queue_properties const *properties = nullptr);
	cl_program &GetProgram(unsigned id);
	cl_kernel &GetKerenel(unsigned id);
}

#endif//file
