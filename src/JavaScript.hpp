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

	using Snapshot = std::vector<std::uint32_t>;

	enum Parse { None=0, Global=1, Strict=2 };
	bool SaveSnapshot(std::string const &source, Snapshot &buffer, enum Parse opt = None);

	constexpr bool CopyBytecode = true;
	bool ExecuteSnapshot(Snapshot const &shapshot, bool copyBytecode = false);
}

#endif // file
