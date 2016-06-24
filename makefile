LIB_PATHS=-L/usr/local/Cellar/opencv/2.4.12/lib
LIBS=-lopencv_highgui -lopencv_core
all: Assignment2

Assignment2: Assignment2.cpp
	g++ Assignment2.cpp ${LIB_PATHS} ${LIBS} -o Assignment2

.PHONY:clean

clean:
	rm Assignment2
