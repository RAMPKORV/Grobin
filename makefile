UNAME := $(shell uname -s)
CPP = g++
CPPFLAGS = `allegro-config --libs`

ifeq ($(UNAME), Linux)
all: grobin.out
grobin.out : gr13.cpp
	g++ gr13.cpp $(CPPFLAGS) -o grobin.out
endif
ifeq ($(UNAME), Windows)
all: grobin.exe
grobin.exe : gr13.cpp
	g++ gr13.cpp $(CPPFLAGS) -o grobin.exe
endif


