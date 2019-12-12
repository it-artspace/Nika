FILES = main.cpp miscellaneous/Controller.cpp Objects/Canvas.cpp Objects/point.cpp Objects/cluster.cpp algorithms/kmeans.cpp algorithms/spanTree.cpp
CPFLAGS = -std=c++11 -w -g
all:
	g++ ${FILES} ${CPFLAGS} -o geom
