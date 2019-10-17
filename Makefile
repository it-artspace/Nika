FILES = main.cpp Controller.cpp Canvas.cpp point.cpp cluster.cpp kmeans.cpp
CPFLAGS = -std=c++11 -w
all:
	g++ ${FILES} ${CPFLAGS} -o geom
