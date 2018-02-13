#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#define SUCCESS 1
#define ERROR -1

#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <list>
#include <string.h>
#include <string>
#include <float.h>
#include <math.h> 
#include <ctime>
#include <libserialport.h>
#include <stdint.h>

using namespace cv;
using namespace std;

int pixelToMicroMeters(int pixelValue);
int computeDistanceX(int pupilX, int cameraX);
int computeDistanceY(int pupilY, int cameraY);
int moveXLeft(int distanceToMove, struct sp_port *port);
int moveXRight(int distanceToMove, struct sp_port *port);
int moveYUp(int distanceToMove, struct sp_port *port);
int moveYDown(int distanceToMove, struct sp_port *port);
int controlPlatform(int pupilX, int pupilY, int cameraX, int cameraY, char *portName);


#endif /* PLATFORM_HPP */