#include "Kernel.hpp"
#include "OpenCL.hpp"

namespace
{
	class Kernel::Program
	{
	public:

		bool CreateProgram() = 0;
		bool UpdateSource() override;

	protected:

		cl_program program = nullptr;

	private:

		static void OnNotify(cl_program program, void *that);
	};

	class SourceCode : public Kernel::Program
	{
	public:

		SourceCode(std::string sourceCode);
		bool CreateProgram() override;

	private:

		std::string sourceCode;
	};
}

bool SourceCode::CreateProgram()
{
	// Split the code into lines.
	std::vector<std::string> lines;
	str::split(lines, sourceCode, "\n");
	// Collect raw C strings.
	std::vector<std::size_t> lengths;
	std::vector<char const *> strings;
	for (auto const &line : lines)
	{
		lengths.emplace_back(line.size());
		strings.emplace_back(line.data());
	}
	// Create from source lines
	cl_context const context = OpenCL::GetContext();
	if (context)
	{
		cl_int error;
		program = clCreateProgramWithSource(context, lines.size(), strings.data(), lengths.data(), &error);
		if (OpenCL::CheckError("clCreateProgramWithSource", error))
		{
			SDL::LogError(CannotAllocateResource);
		}
	}
	return program;
}

bool Kernel::Program::UpdateSource()
{
	if (CreateProgram())
	{
		auto &devices = OpenCL::GetDeviceIDs();
		cl_int error = clBuildProgram(program, devices.size(), devices.data(), options.c_str(), OnNotify, this);
		if (not OpenCL::CheckError("clBuildProgram", error))
		{
			return true;
		}
		SDL::LogError(CannotBuildProgram);
	}
	return false;
}
