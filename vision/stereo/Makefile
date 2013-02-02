#This variable defines the flags to link in the openCV libraries
OPENCV = `pkg-config --cflags --libs opencv`

all:
	g++ read_string_list.cpp calib.cpp stereo_main.cpp calib_capture.cpp $(OPENCV) -o stereo 
match:
	g++ stereo_match.cpp $(OPENCV) -o match
clean:
	rm -rf *o
