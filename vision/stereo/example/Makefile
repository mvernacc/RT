#This variable defines the flags to link in the openCV libraries
OPENCV = `pkg-config --cflags --libs opencv`

all: calib match

calib: stereo_calib.cpp
	g++  stereo_calib.cpp $(OPENCV) -o stereo_calib
match: stereo_match.cpp
	g++ stereo_match.cpp `pkg-config --cflags --libs opencv` -o stereo_match

clean:
	rm -rf *o
