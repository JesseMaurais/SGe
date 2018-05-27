#ifndef Test_hpp
#define Test_hpp

namespace test
{
    class Unit
    {
        static void Run(char const *pattern);

    protected:

        Unit(char const *name);
        virtual ~Unit();
        virtual void Run() = 0;
    };
}

#endif // file
