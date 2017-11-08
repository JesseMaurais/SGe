#include "Kernel.hpp"
#include "OpenCL.hpp"

namespace
{
	cl_program CreateWithBinary(std::vector<std::vector<unsigned char>> const &code)
	{
		// Collect raw C strings.
		std::vector<std::size_t> lengths;
		std::vector<unsigned char const *> binaries;
		for (auto const &line : code)
		{
			lengths.emplace_back(line.size());
			binaries.emplace_back(line.data());
		}

		cl_context const context = OpenCL::GetContext();
		cl_device_id const *devices = OpenCL::GetDeviceIDs();

		cl_int error, count = 0;
		while (devices[count]) ++count;
		std::vector<cl_int> status(code.size());

		cl_program const program = clCreateProgramWithBinary
			(context, count, devices, lengths.data(), binaries.data(), status.data(), &error);

		if (error)
		{
			OpenCL::SetError("clCreateProgramWithSource", error);
		}
		return program;
	}

	cl_program CreateWithSource(std::vector<std::string> const &lines)
	{
		// Collect raw C strings.
		std::vector<std::size_t> lengths;
		std::vector<char const *> strings;
		for (auto const &line : lines)
		{
			lengths.emplace_back(line.size());
			strings.emplace_back(line.data());
		}
		// Create from source lines
		cl_int error;
		cl_context const context = OpenCL::GetContext();
		cl_program const program = clCreateProgramWithSource
			(context, lines.size(), strings.data(), lengths.data(), &error);
		// Record error if failed
		if (error)
		{
			OpenCL::SetError("clCreateProgramWithSource", error);
		}
		return program;
	}

	// Abstract interface for OpenCL programs

	class Kernel::Program
	{
	public:

		cl_program Create() = 0;
		bool Build();

	private:

		static void OnNotify(cl_program program, void *that);
	};

	class BinaryCode : public Kernel::Program
	{
	public:

		BinaryCode(std::string sourceCode);
		cl_program Create() override;

	private:

		std::vector<std::vector<unsigned char>> binaryCode;
	};

	class SourceCode : public Kernel::Program
	{
	public:

		SourceCode(std::string sourceCode);
		cl_program Create() override;

	private:

		std::string sourceCode;
	};
}

cl_program BinaryCode::Create()
{
	return CreateWithBinary(binaryCode);
}

cl_program SourceCode::Create()
{
	// Split the code into lines.
	std::vector<std::string> lines;
	str::split(lines, sourceCode, "\n");
	return CreateWithSource(lines);
}

bool Kernel::Program::Build()
{
	while (true)
	{
		cl_uint count = 0;
		cl_device_id const *devices = OpenCL::GetDeviceIDs();
		while (devices[count]) ++count;

		cl_program const program = Create();
		if (program)
		{
			cl_int error = clBuildProgram(program, count, devices, options.c_str(), OnNotify, this);
			if (error)
			{
				OpenCL::SetError("clBuildProgram", error);
			}
		}

		return false;
	}
}

bool Kernel::UpdateSource()
{
	if (source)
	{
		return source->Build();
	}
	return false;
}
