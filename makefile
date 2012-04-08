UNAME := $(shell uname -s)
CPP = g++
CPPFLAGS = `allegro-config --libs`

ifeq ($(UNAME), Linux)
grobin.out : gr13.cpp
	g++ gr13.cpp $(CPPFLAGS) -o grobin.out
endif
ifeq ($(UNAME), Windows)
grobin.exe : gr13.cpp
	g++ gr13.cpp $(CPPFLAGS) -o grobin.exe
endif


