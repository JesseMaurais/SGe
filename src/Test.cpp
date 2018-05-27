#include "Test.hpp"
#include <iostream>
#include <exception>
#include <string>
#include <map>

namespace
{
    std::map<test::Unit *, std::string> tests;

    bool Match(std::string_view needle, std::string_view haystack)
    {
        std::string_view::size_type i, j;
        for (i = j = 0; needle.size() > i; ++i, ++j)
        {
            switch (needle[i])
            {
            default:
                if (haystack[j] != needle[i])
                {
                    return false;
                }
                break;

            case '?':
                if (haystack.size() == j)
                {
                    return false;
                }
                break;

            case '*':
                if (needle.size() == (i + 1))
                {
                    return true;
                }

                while (haystack.size() > j)
                {
                    if (Match(needle.substr(i + 1), haystack.substr(j + 1)))
                    {
                        return true;
                    }
                    ++j;
                }
                return false;
            }
        }
        return haystack.size() == j;
    }
}

test::Unit::Unit(char const *name)
{
    tests[this] = name;
}

test::Unit::~Unit()
{
    auto const it = tests.find(this);
    if (tests.end() != it)
    {
        tests.erase(it);
    }
}

void test::Unit::Run(char const *pattern)
{
    auto &out = std::cerr;
    for (auto const pair : tests)
    {
        if (Match(pattern, pair.second))
        {
            out << "Test: " << pair.second << "\n";
            try
            {
                pair.first->Run();
            }
            catch (std::exception const &except)
            {
                out << "Exception: " << except.what() << "\n";
            }
            out << std::endl;
        }
    }
}
