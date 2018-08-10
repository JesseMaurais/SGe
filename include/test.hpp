#ifndef test_hpp
#define test_hpp

#include "dbg.hpp"

namespace test
{
	class interface
	{
	public:
		virtual void run() = 0;
	protected:
		interface(char const *name);
		virtual ~interface();
	};

	void run(char const *pattern = nullptr);

	template <bool Debug, typename Type = void> class enabled { };
	template <typename Type> class enabled<true, Type> { static inline Type unit; };

	template <typename Type> struct unit : interface, enabled<DEBUG, Type>
	{
	protected:
		unit(char const *name)
		: interface(name)
		{ }
	private:
		unit &operator = (unit const &) = delete;
		void *operator new (std::size_t) = delete;
	};
}

#endif // file
