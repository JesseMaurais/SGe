#include "OpenCL.hpp"
#include "Manager.hpp"
#include "SDL.hpp"
#include <cstdio>
#include <vector>
#include <exception>


static const char *GetErrorString(cl_int error)
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


signed OpenCL::SetError(const char *origin, cl_int error)
{
	return SDL_SetError("%s: %s", origin, GetErrorString(error));
}


cl_platform_id *OpenCL::GetPlatformIDs()
{
	static struct PlatformIDs : std::vector<cl_platform_id>
	{
		cl_platform_id *Pointer()
		{
			return empty() ? nullptr : data();
		}

		PlatformIDs()
		{
			cl_uint num_platforms = 0;
			do
			{
				resize(num_platforms);
				cl_platform_id *platforms = Pointer();
				cl_int error = clGetPlatformIDs(num_platforms, platforms, &num_platforms);
				if (error)
				{
					OpenCL::SetError("clGetPlatformIDs", error);
					clear();
					return;
				}
			}
			while (size() < num_platforms);
			push_back(nullptr);
		}

	} singleton;

	return singleton.Pointer();
}


cl_device_id *OpenCL::GetDeviceIDs(cl_device_type type)
{
	static struct DeviceIDs : std::vector<cl_device_id>
	{
		cl_device_id *Pointer()
		{
			return empty() ? nullptr : data();
		}

		DeviceIDs() = default;
		DeviceIDs(cl_platform_id platform, cl_device_type type)
		{
			cl_uint num_devices = 0;
			do
			{
				reserve(num_devices);
				cl_device_id *devices = Pointer();
				cl_int error = clGetDeviceIDs(platform, type, num_devices, devices, &num_devices);
				if (error)
				{
					OpenCL::SetError("clGetDeviceIDs", error);
					clear();
					return;
				}
			}
			while (size() < num_devices);
			push_back(nullptr);
		}

	} singleton;

	if (CL_DEVICE_TYPE_DEFAULT == type)
	{
		cl_platform_id *platforms = OpenCL::GetPlatformIDs();
		if (platforms)
		{
			for (std::size_t it = 0; singleton.size() < 2 and platforms[it]; ++it)
			{
				singleton = DeviceIDs(platforms[it], type);
			}
		}
	}

	return singleton.Pointer();
}


cl_context OpenCL::GetContext(cl_context_properties *properties)
{
	struct ComputeContext
	{
		cl_context context = nullptr;

		ComputeContext() = default;
		ComputeContext(cl_context_properties *properties)
		{
			cl_device_id *devices = OpenCL::GetDeviceIDs();
			if (devices)
			{
				cl_uint num_devices = 0;
				while (devices[num_devices++]);
				cl_int error;
				context = clCreateContext(properties, num_devices, devices, OnNotify, this, &error);
				if (error)
				{
					OpenCL::SetError("clCreateContext", error);
				}
			}
		}
		~ComputeContext()
		{
			if (context)
			{
				cl_int error = clReleaseContext(context);
				if (error)
				{
					OpenCL::SetError("clReleaseContext", error);
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
	if (properties or not singleton.context)
	{
		// Update for compatibility with given properties
		singleton = ComputeContext(properties);
	}
	return singleton.context;
}

cl_command_queue OpenCL::GetCommandQueue(cl_command_queue_properties properties)
{
	static struct CommandQueue
	{
		cl_command_queue queue = nullptr;

		CommandQueue() = default;
		CommandQueue(cl_command_queue_properties properties)
		{
			cl_device_id *devices = OpenCL::GetDeviceIDs();
			if (devices)
			{
				cl_context context = OpenCL::GetContext();
				if (context)
				{
					for (std::size_t it = 0; not queue and devices[it]; ++it)
					{
						cl_int error;
						queue = clCreateCommandQueue(context, devices[it], properties, &error);
						if (error)
						{
							OpenCL::SetError("clCreateCommandQueue", error);
						}
					}
				}
			}
		}
		~CommandQueue()
		{
			cl_int error = clReleaseCommandQueue(queue);
			if (error)
			{
				OpenCL::SetError("clReleaseCommandQueue", error);
			}
		}

	} singleton;
	// Either new or not initialized
	if (properties or not singleton.queue)
	{
		// Update for compatibility with given properties
		singleton = CommandQueue(properties);
	}
	return singleton.queue;
}

