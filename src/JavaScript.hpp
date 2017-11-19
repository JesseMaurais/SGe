#ifndef JavaScript_hpp
#define JavaScript_hpp

#include <string>
#include <vector>
#include <cstdint>

namespace js
{
	class Engine
	{
	public:
		Engine();
		~Engine();
	};

	enum Parse
	{
		Global, Strict
	};

	using Snapshot = std::vector<std::uint32_t>;

	bool ParseAndSaveSnapshot(std::string const &source, enum Parse opts, Snapshot &snapshot);
	bool ExecuteSnapshot(Snapshot const &shapshot, bool copyBytecode);
}

#endif // file
