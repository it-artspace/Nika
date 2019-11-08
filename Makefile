FILES = main.cpp Controller.cpp Canvas.cpp point.cpp cluster.cpp kmeans.cpp spanTree.cpp
CPFLAGS = -std=c++11 -w -g
all:
	g++ ${FILES} ${CPFLAGS} -o geom
