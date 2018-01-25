#pragma once
#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <list>
#include <string.h>
#include <string>
#include <float.h>
#include <math.h> 
#include <opencv2/ml.hpp>


using namespace cv;
using namespace std;

#define PI 3.1416					/* global variable to the PI value*/

/* global variables */
extern int ci[3];					/* ci:the parametrs[xc,yc,radius] of the limbic boundary */
extern int cp[3];					/* cp:the parametrs[xc, yc, radius] of the pupilary boundary */


int otsuAlgorithm(Mat image, int *imgHist);
void find(Mat *image, list<int> &vecX, list<int> &vecY);
void findNan(list<int> &vecX, list<int> &vecY);
void findCloseBorder(list<int> &vecX, list<int> &vecY, int rmin, int rmax, int rows, int cols);
vector<double> conv(vector<double> A, vector<double> B, string option);
void findMaxb(Mat *matrix, int *x, int *y);
double lineint(Mat image, int centerX, int centerY, int r, int n, string part);
bool any(vector<double> toBeSearchedVector, double paramValue, string logicComparator);
void partiald(double *b, int *r, vector<double> &blur, Mat *image, int centerX, int centerY, int rmin, int rmax, float sigma, int n, string part);
void search(Mat image, int rmin, int rmax, int x, int y, string option);
void thresh(Mat image, double rmin, double rmax, double scale);


#endif /* FUNCTIONS_HPP */