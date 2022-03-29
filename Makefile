XX = g++
CXXFLAGS = -Wall


proj2: fleet.o mytest.cpp
		$(CXX) $(CXXFLAGS) fleet.o mytest.cpp -o proj2

fleet.o: fleet.h fleet.cpp
		$(CXX) $(CXXFLAGS) -c fleet.cpp
run:
		proj2

val:
		valgrind proj2

clean:
		rm *~
		rm *.o





