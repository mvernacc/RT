#This variable defines the flags to link in the openCV libraries
OPENCV = `pkg-config --cflags --libs opencv`

all:
	g++ -g match.cpp read_string_list.cpp calib.cpp stereo_main.cpp calib_capture.cpp $(OPENCV) -o stereo 
stereo_match:
	g++ -g stereo_match.cpp $(OPENCV) -o stereo_match
clean:
	rm -rf *o
