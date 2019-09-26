FILES = main.cpp Controller.cpp Canvas.cpp point.cpp cluster.cpp
CPFLAGS = -std=c++11 -w
all:
	g++ ${FILES} ${CPFLAGS} -o geom
