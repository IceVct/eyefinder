#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#define REACHED true
#define NOTREACHED false

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

using namespace cv;
using namespace std;

int computeDistanceX(int pupilX, int cameraX);
int computeDistanceY(int pupilY, int cameraY);
void moveXLeft();
void moveXRight();
void moveYUp();
void moveYDown();
bool controlPlatform(int pupilX, int pupilY, int cameraX, int cameraY);


#endif /* PLATFORM_HPP */