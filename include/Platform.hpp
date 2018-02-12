#ifndef PLATFORM_HPP
#define PLATFORM_HPP

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
void moveXLeft(int distanceToMove);
void moveXRight(int distanceToMove);
void moveYUp(int distanceToMove);
void moveYDown(int distanceToMove);
void controlPlatform(int pupilX, int pupilY, int cameraX, int cameraY);


#endif /* PLATFORM_HPP */