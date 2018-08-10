#ifndef test_hpp
#define test_hpp

namespace test
{
	class unit
	{
	protected:
		virtual void run() = 0;
		unit(char const *name);
		virtual ~unit();
	private:
		unit &operator = (unit const &) = delete;
		void *operator new(std::size_t) = delete;
	};

	void run(char const *pattern = nullptr);
}

#endif // file
