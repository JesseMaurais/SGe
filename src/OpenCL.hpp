/** \file
 * Utility functions for working with OpenCL.
 */

#ifndef OpenCL_hpp
#define OpenCL_hpp

#include <CL/cl.h>
#include <vector>

namespace OpenCL
{
	/// Set the SDL error string to the given error code. Always returns true.
	bool SetError(const char *origin, cl_int error);
	/// Set the SDL error string to the current error code. Returns true when an error exists.
	bool CheckError(const char *origin, cl_int error);
	/// Log the current error with SDL. Returns true when an error exists.
	bool LogError(const char *origin, cl_int error);

	std::vector<cl_platform_id> const &GetPlatformIDs();
	std::vector<cl_device_id> const &GetDeviceIDs(cl_device_type type = 0);
	std::vector<cl_device_id> GetDeviceIDs(cl_context context);
	cl_context GetContext(std::vector<cl_context_properties> const &properties = {});
	cl_command_queue GetCommandQueue(std::vector<unsigned long> const &properties = {});
}

#endif//file
