#################################################
# Description: Makefile for the RayTracer		#
# Author: Erik E. Beerepoot						#
# Date: 22 August 2012 - 2015                   #
#################################################

#Path defintions
ROOT = .
BIN = $(ROOT)/bin/
SRC = $(ROOT)/src/
INC = $(ROOT)/include/

#Compilation defintions
CXX = g++-4.9

#Headers, Source, Libs
SOURCEFILES = $(SRC)PNGImage.cpp $(SRC)Scene.cpp $(SRC)ImageRenderer.cpp $(SRC)Camera.cpp $(SRC)GeometricTypes.cpp $(SRC)ComputeManager.cpp $(SRC)AcceleratedPinholeCamera.cpp

all: target
	
target:
	@echo "Building Raytracer"
	$(CXX) -std=c++11 -framework OpenCL -o $(BIN)RayTracer $(SOURCEFILES)  -lpng -lz -I$(INC) -I/opt/local/include -I/usr/local/Cellar/libpng/1.6.18/include/ -L/usr/local/Cellar/libpng/1.6.18/lib/
	
clean:
	rm $(BIN)RayTracer*
