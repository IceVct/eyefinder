#pragma once
#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <string.h>
#include <float.h>
#include <math.h> 
#include <opencv2/ml.hpp>


using namespace cv;
using namespace std;

#define PI 3.1416					/* global variable to the PI value*/

/* global variables */
extern int ci[3];					/* ci:the parametrs[xc,yc,radius] of the limbic boundary */
extern int cp[3];					/* cp:the parametrs[xc, yc, radius] of the pupilary boundary */


void find(Mat *image, vector<int> &vecX, vector<int> &vecY);
void findNan(vector<int> &vecX, vector<int> &vecY);
void findCloseBorder(vector<int> &vecX, vector<int> &vecY, int rmin, int rmax, int rows, int cols);
vector<double> conv(vector<double> A, vector<double> B, char *option);
void findMaxb(Mat *matrix, int *x, int *y);
double lineint(Mat image, vector<int> C, int r, int n, char *part);
bool any(vector<double> toBeSearchedVector, double paramValue, char const *logicComparator);
void partiald(double *b, int *r, vector<double> &blur, Mat *image, vector<int> &C, int rmin, int rmax, float sigma, int n, char part[7]);
void search(Mat image, int rmin, int rmax, int x, int y, char *option);
void thresh(Mat image, double rmin, double rmax, double scale);


#endif /* FUNCTIONS_HPP */