#include "test.hpp"
#include "dbg.hpp"
#include <exception>
#include <iostream>
#include <string>
#include <regex>
#include <map>

namespace test
{
	static std::map<interface*, std::string> all_tests;

	void run(char const *pattern)
	{
		std::regex regex(pattern ? pattern : R"(.*?)");

		for (auto const &pair : all_tests)
		{
			if (std::regex_match(pair.second, regex)) try
			{
				std::cerr << "testing " << pair.second << std::endl;
				pair.first->run();
			}
			catch (std::exception const &except)
			{
				std::cerr << "\t" << except.what() << std::endl;
			}
		}
	}

	interface::interface(char const *name)
	{
		verify(all_tests.try_emplace(this, name).second);
	}

	interface::~interface()
	{
		verify(1 == all_tests.erase(this));
	}
}

struct TestTest : test::unit<TestTest>
{
	TestTest() : unit("My Test")
	{ }

	void run() override
	{
		std::cout << "Hello World" << std::endl;
	}
};

int main(int argc, char **argv)
{
	test::run();
}

