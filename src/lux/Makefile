CC=g++ -std=c++11
CCFLAGS=-I/usr/local/include
LDFLAGS=-L/usr/local/lib -llua -ldl

all: test1 test2 test3 test4 test5 test6 test7 test8

test1: test1.cpp lxthunk.hpp
	$(CC) -g -o test1 test1.cpp $(CCFLAGS) $(LDFLAGS)

test2: test2.cpp lxarray.hpp
	$(CC) -g -o test2 test2.cpp $(CCFLAGS) $(LDFLAGS)

test3: test3.cpp lxbytes.hpp
	$(CC) -g -o test3 test3.cpp $(CCFLAGS) $(LDFLAGS)

test4: test4.cpp lxthunk.hpp lxarray.hpp
	$(CC) -g -o test4 test4.cpp $(CCFLAGS) $(LDFLAGS)

test5: test5.cpp lxpanic.hpp
	$(CC) -g -o test5 test5.cpp $(CCFLAGS) $(LDFLAGS)

test6: test6.cpp lxclass.hpp
	$(CC) -g -o test6 test6.cpp $(CCFLAGS) $(LDFLAGS)

test7: test7.cpp lxtuple.hpp
	$(CC) -g -o test7 test7.cpp $(CCFLAGS) $(LDFLAGS)

test8: test8.cpp lxarray.hpp lxbytes.hpp lxstack.hpp lxthunk.hpp
	$(CC) -g -o test8 test8.cpp $(CCFLAGS) $(LDFLAGS) -lSDL2

clean:
	rm test1 test2 test3 test4 test5 test6 test7 test8
