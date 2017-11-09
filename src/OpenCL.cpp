#include "OpenCL.hpp"
#include "Manager.hpp"
#include "Error.hpp"
#include "SDL.hpp"
#include <cstdio>
#include <vector>

namespace
{
	const char *ErrorString(cl_int error)
	{
		switch (error)
		{
		default:
			return "Unknown error";
		case CL_SUCCESS:
			return "Success";
		case CL_DEVICE_NOT_FOUND:
			return "Device not found";
		case CL_DEVICE_NOT_AVAILABLE:
			return "Device not available";
		case CL_COMPILER_NOT_AVAILABLE:
			return "Compiler not available";
		case CL_MEM_OBJECT_ALLOCATION_FAILURE:
			return "Memory object allocation failure";
		case CL_OUT_OF_RESOURCES:
			return "Out of resources";
		case CL_OUT_OF_HOST_MEMORY:
			return "Out of host memory";
		case CL_PROFILING_INFO_NOT_AVAILABLE:
			return "Profiling info not available";
		case CL_MEM_COPY_OVERLAP:
			return "Memory copy overlap";
		case CL_IMAGE_FORMAT_MISMATCH:
			return "Image format mismatch";
		case CL_IMAGE_FORMAT_NOT_SUPPORTED:
			return "Image format not supported";
		case CL_BUILD_PROGRAM_FAILURE:
			return "Build program failure";
		case CL_MAP_FAILURE:
			return "Map failure";
		case CL_MISALIGNED_SUB_BUFFER_OFFSET:
			return "Miss-aligned sub buffer offset";
		case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
			return "Execution status error for events in wait list";
		case CL_COMPILE_PROGRAM_FAILURE:
			return "Compile program failure";
		case CL_LINKER_NOT_AVAILABLE:
			return "Linker not available";
		case CL_LINK_PROGRAM_FAILURE:
			return "Link program failure";
		case CL_DEVICE_PARTITION_FAILED:
			return "Device partition failed";
		case CL_KERNEL_ARG_INFO_NOT_AVAILABLE:
			return "Kernel argument info not available";
		case CL_INVALID_VALUE:
			return "Invalid value";
		case CL_INVALID_DEVICE_TYPE:
			return "Invalid device type";
		case CL_INVALID_PLATFORM:
			return "Invalid platform";
		case CL_INVALID_DEVICE:
			return "Invalid device";
		case CL_INVALID_CONTEXT:
			return "Invalid context";
		case CL_INVALID_QUEUE_PROPERTIES:
			return "Invalid queue properties";
		case CL_INVALID_COMMAND_QUEUE:
			return "Invalid command queue";
		case CL_INVALID_HOST_PTR:
			return "Invalid host pointer";
		case CL_INVALID_MEM_OBJECT:
			return "Invalid memory object";
		case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
			return "Invalid image format descriptor";
		case CL_INVALID_IMAGE_SIZE:
			return "Invalid image size";
		case CL_INVALID_SAMPLER:
			return "Invalid sampler";
		case CL_INVALID_BINARY:
			return "Invalid binary";
		case CL_INVALID_BUILD_OPTIONS:
			return "Invalid build options";
		case CL_INVALID_PROGRAM:
			return "Invalid program";
		case CL_INVALID_PROGRAM_EXECUTABLE:
			return "Invalid program executable";
		case CL_INVALID_KERNEL_NAME:
			return "Invalid kernel name";
		case CL_INVALID_KERNEL_DEFINITION:
			return "Invalid kernel definition";
		case CL_INVALID_KERNEL:
			return "Invalid kernel";
		case CL_INVALID_ARG_INDEX:
			return "Invalid argument index";
		case CL_INVALID_ARG_VALUE:
			return "Invalid argument value";
		case CL_INVALID_ARG_SIZE:
			return "Invalid argument size";
		case CL_INVALID_KERNEL_ARGS:
			return "Invalid kernel arguments";
		case CL_INVALID_WORK_DIMENSION:
			return "Invalid work dimension";
		case CL_INVALID_WORK_GROUP_SIZE:
			return "Invalid work group size";
		case CL_INVALID_WORK_ITEM_SIZE:
			return "Invalid work item size";
		case CL_INVALID_GLOBAL_OFFSET:
			return "Invalid global offset";
		case CL_INVALID_EVENT_WAIT_LIST:
			return "Invalid event wait list";
		case CL_INVALID_EVENT:
			return "Invalid event";
		case CL_INVALID_OPERATION:
			return "Invalid operation";
		case CL_INVALID_GL_OBJECT:
			return "Invalid OpenGL object";
		case CL_INVALID_BUFFER_SIZE:
			return "Invalid buffer size";
		case CL_INVALID_MIP_LEVEL:
			return "Invalid mip level";
		case CL_INVALID_GLOBAL_WORK_SIZE:
			return "Invalid global work size";
		case CL_INVALID_PROPERTY:
			return "Invalid property";
		case CL_INVALID_IMAGE_DESCRIPTOR:
			return "Invalid image descriptor";
		case CL_INVALID_COMPILER_OPTIONS:
			return "Invalid compiler options";
		case CL_INVALID_LINKER_OPTIONS:
			return "Invalid linker options";
		case CL_INVALID_DEVICE_PARTITION_COUNT:
			return "Invalid device partition count";
		}
	}
}


bool OpenCL::SetError(const char *origin, cl_int error)
{
	return SDL::SetError(ColonSeparator, origin, ErrorString(error));
}

bool OpenCL::CheckError(const char *origin, cl_int error)
{
	return error and OpenCL::SetError(origin, error);
}

bool OpenCL::LogError(const char *origin, cl_int error)
{
	return error and SDL::perror(origin, ErrorString(error));
}


std::vector<cl_platform_id> const &OpenCL::GetPlatformIDs()
{
	static struct PlatformIDs : std::vector<cl_platform_id>
	{
		PlatformIDs()
		{
			cl_uint num_platforms = 0;
			do
			{
				resize(num_platforms);
				cl_platform_id *platforms = empty() ? nullptr : data();
				cl_int error = clGetPlatformIDs(num_platforms, platforms, &num_platforms);
				if (OpenCL::CheckError("clGetPlatformIDs", error))
				{
					SDL::perror(CannotQueryValue);
					clear();
					break;
				}
			}
			while (size() < num_platforms);
		}

	} singleton;

	return singleton;
}


std::vector<cl_device_id> const &OpenCL::GetDeviceIDs(cl_device_type type)
{
	static struct DeviceIDs : std::vector<cl_device_id>
	{
		DeviceIDs() = default;
		DeviceIDs(cl_platform_id platform, cl_device_type type)
		{
			cl_uint num_devices = 0;
			do
			{
				resize(num_devices);
				cl_device_id *devices = empty() ? nullptr : data();
				cl_int error = clGetDeviceIDs(platform, type, num_devices, devices, &num_devices);
				if (OpenCL::CheckError("clGetDeviceIDs", error))
				{
					SDL::perror(CannotQueryValue);
					clear();
					break;
				}
			}
			while (size() < num_devices);
		}

	} singleton;

	static cl_device_type current = CL_DEVICE_TYPE_DEFAULT;
	if (singleton.empty() or type != current)
	{
		if (not type) type = current;
		for (cl_platform_id const platform : OpenCL::GetPlatformIDs())
		{
			singleton = DeviceIDs(platform, type);
			if (not singleton.empty())
			{
				current = type;
				break;
			}
		}
	}

	return singleton;
}

std::vector<cl_device_id> OpenCL::GetDeviceIDs(cl_context context)
{
	struct DeviceIDs : std::vector<cl_device_id>
	{
		DeviceIDs(cl_context context)
		{
			unsigned count = 0;
			do
			{
				resize(count);
				std::size_t bytes = size()*sizeof(cl_device_id);
				cl_device_id *devices = empty() ? nullptr : data();
				cl_int error = clGetContextInfo(context, CL_CONTEXT_DEVICES, bytes, devices, &bytes);
				if (OpenCL::CheckError("clGetContextInfo", error))
				{
					SDL::perror(CannotQueryValue);
					clear();
					break;
				}
				count = bytes/sizeof(cl_device_id);
			}
			while (size() < count);
		}

	} devices = context;
	return devices;
}

cl_context OpenCL::GetContext(std::vector<cl_context_properties> const &properties)
{
	struct Context
	{
		cl_context context = nullptr;

		Context() = default;
		Context(std::vector<cl_context_properties> properties)
		{
			auto &devices = OpenCL::GetDeviceIDs();
			if (not devices.empty())
			{
				cl_int error;
				properties.push_back(0); // ensure terminal
				context = clCreateContext(properties.data(), devices.size(), devices.data(), OnNotify, this, &error);
				if (OpenCL::CheckError("clCreateContext", error))
				{
					SDL::perror(CannotAllocateResource);
				}
			}
		}
		~Context()
		{
			if (context)
			{
				cl_int error = clReleaseContext(context);
				if (OpenCL::CheckError("clReleaseContext", error))
				{
					SDL::perror(CannotFreeResource);
				}
			}
		}

	private:

		static void OnNotify(const char *error, const void *info, std::size_t size, void *that)
		{
			(void) info;
			(void) size;
			(void) that;
			SDL::perror(__func__, error);
		}

	} singleton;
	// Either new or not initialized
	if (not properties.empty() or not singleton.context)
	{
		// Update for compatibility with given properties
		singleton = Context(properties);
	}
	return singleton.context;
}

cl_command_queue OpenCL::GetCommandQueue(std::vector<unsigned long> const &properties)
{
	static struct CommandQueue
	{
		cl_command_queue queue = nullptr;

		CommandQueue() = default;
		CommandQueue(std::vector<unsigned long> properties)
		{
			cl_context context = OpenCL::GetContext();
			if (context)
			{
				auto devices = OpenCL::GetDeviceIDs(context);
				properties.push_back(0); // ensure terminal
				for (cl_device_id const device : devices)
				{
					cl_int error;
					queue = clCreateCommandQueueWithProperties(context, device, properties.data(), &error);
					if (OpenCL::CheckError("clCreateCommandQueueWithProperties", error))
					{
						SDL::perror(CannotAllocateResource);
					}
					else if (queue)
					{
						break;
					}
				}
			}
		}
		~CommandQueue()
		{
			cl_int error = clReleaseCommandQueue(queue);
			if (OpenCL::CheckError("glReleaseCommandQueue", error))
			{
				SDL::perror(CannotFreeResource);
			}
		}

	} singleton;
	// Either new or not initialized
	if (not properties.empty() or not singleton.queue)
	{
		// Update for compatibility with given properties
		singleton = CommandQueue(properties);
	}
	return singleton.queue;
}

