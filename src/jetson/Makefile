CC=clang++
CXXFLAGS=-O3 -I/usr/local/include/opencv4 -L/usr/local/lib/ -I/usr/include/octomap -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_imgcodecs -lopencv_videoio -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_stitching -loctomath -loctomap -lDUO -lopencv_cudaarithm -lopencv_cudafeatures2d -li2c


map.o: map/Map.cpp map/Map.h
	$(CC) $(CXXFLAGS) -c map/Map.cpp -o map.o

libi2c.o: i2c/libi2c.cpp i2c/libi2c.h
	$(CC) $(CXXFLAGS) -c i2c/libi2c.cpp -o libi2c.o

i2c.o: i2c/I2C.cpp i2c/I2C.h
	$(CC) $(CXXFLAGS) -c i2c/I2C.cpp -o i2c.o

robot.o: Robot.cpp Robot.h
	$(CC) $(CXXFLAGS) -c Robot.cpp -o robot.o

vision.o: vision/Vision.cpp vision/Vision.h vision/DUO3D.h
	$(CC) $(CXXFLAGS) -c vision/Vision.cpp -o vision.o

navegacion.o: navegacion/navegacion.cpp navegacion/navegacion.h
	$(CC) $(CXXFLAGS) -c navegacion/navegacion.cpp -o navegacion.o

localization.o: localization/Localization.cpp localization/Localization.h map/Map.h Robot.h
	$(CC) $(CXXFLAGS) -c localization/Localization.cpp -o localization.o

main.o: main.cpp
	$(CC) $(CXXFLAGS) -c main.cpp

all: main.o vision.o robot.o libi2c.o i2c.o map.o navegacion.o localization.o
	$(CC) $(CXXFLAGS) -o robofinder main.o vision.o robot.o libi2c.o i2c.o map.o navegacion.o localization.o


clean:
	rm *.o
