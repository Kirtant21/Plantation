CXX = g++
CXXFLAGS = -Wall

mytest.o: iqueue.o mytest.cpp
	$(CXX) $(CXXFLAGS) iqueue.o mytest.cpp -o mytest
iqueue.o: iqueue.cpp iqueue.h
	$(CXX) $(CXXFLAGS) -c iqueue.cpp

run:
	./mytest
val:
	valgrind ./mytest
fullCheck:
	valgrind --leak-check=full ./mytest
debug:
	gdb ./mytest
clean:
	rm *.o*
	rm *~
	rm *#
s:
	emacs iqueue.cpp
sh:
	emacs iqueue.h
t:
	emacs mytest.cpp
mk:
	emacs makefile
submit:
	cp iqueue.h iqueue.cpp mytest.cpp ~/cs341proj/proj3/
chkSub:
	ls ~/cs341proj/proj3/
