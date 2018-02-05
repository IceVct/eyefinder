#include "Functions.hpp"

/*
*	THIS CODE WAS ADAPTED FROM Anirudh SK matlab implementation	
*	THUS, THIS IS A TRANSLATION AND ADAPTION FROM MATLAB TO OPENCV
*   IT CAN BE DOWNLOADED IN THIS LINK: 
*   https://www.mathworks.com/matlabcentral/fileexchange/15652-iris-segmentation-using-daugman-s-integrodifferential-operator?focused=3889799&tab=function
*/

int ci[3];					/* ci:the parametrs[xc,yc,radius] of the limbic boundary */
int cp[3];					/* cp:the parametrs[xc, yc, radius] of the pupilary boundary */


// Function that finds a x and y to the highest value of maxb, which is the max value of blur for each of the centre points
// -- find(maxb == max(max(maxb))) from matlab 
void findMaxb(Mat *matrix, int *x, int *y){

	Mat mat_aux = *matrix;
	double maximo = -DBL_MAX;
	int count = 0;

	for (int r = 0; r < mat_aux.rows; r++){
		for (int c = 0; c < mat_aux.cols; c++){
			if (mat_aux.at<double>(r, c) > maximo){
				*x = r;
				*y = c;
				maximo = mat_aux.at<double>(r, c);
			}
		}
	}
}


// Function that computes the convolution operation
vector<double> conv(vector<double> A, vector<double> B, string option){
	int nconv = 0;
	int i = 0, j = 0, i1 = 0;
	double tmp;

	//allocated convolution array
	nconv = A.size() + B.size() - 1;
	vector<double> C(nconv);

	//convolution process
	for (i = 0; i<nconv; i++){
		i1 = i;
		tmp = 0.0;
		for (j = 0; j<B.size(); j++){
			if (i1 >= 0 && i1<A.size()) tmp = tmp + (A[i1] * B[j]);

			i1 -= 1;
			C[i] = tmp;
		}
	}

	// if the option argument is the string "same", creates a new array, else, returns the C vector itself
	if(option.compare("same") == 0){
		int count = 0; // used as index in tmpC array creation
		vector<double> tmpC(A.size()); // creates a temporary array with A's same size
		for (int i = 0; i < nconv; i++) {
			if (i >= B.size() / 2 && i < nconv - B.size() / 2) {
				tmpC[count] = C[i];
				count++; // it sums count only in this conditional
			}
		}
		return tmpC;
	}

	//returns convolution array
	return C;
}


// Function that find the pixels close to the borders-- lines 61 to 63 in matlab code
void findCloseBorder(list<int> &vecX, list<int> &vecY, int rmin, int rmax, int rows, int cols) {

	for (list<int>::iterator it = vecX.begin(), it2 = vecY.begin(); it != vecX.end() && it2 != vecY.end(); it++, it2++){
		// if any of the x or y values are close to the borders, it saves the index in the auxiliar list
		if (*it <= (rmin - 1) || *it2 <= (rmin - 1) || *it > ((rows - rmin) - 1) ||
			*it2 > ((cols - rmin) - 1)){

			// Deleting the pixel coordinates that are too close to the borders, setting their values to -1
			// and then erasing them from the list
			*it = -1;
			*it2 = -1;
		}
	}
}


// Function that looks for NaN values at the vecX and vecY vectors and deletes them -- lines 57 to 59 in matlab code
void findNan(list<int> &vecX, list<int> &vecY) {

	vecX.remove(-1);
	vecY.remove(-1);

}


// Function that looks for pixel values that are lesser than 0.5 at the image and store them at the vecX and vecY vectors
// -- find(I < 0.5) from matlab
void find(Mat *image, list<int> &vecX, list<int> &vecY) {

	Mat im_aux = *image;

	for (int c = 0; c < im_aux.cols; c++)
	{
		for (int r = 0; r < im_aux.rows; r++)
		{
			if (im_aux.at<double>(r, c) < 0.5)
			{
				// storing the values in the end of each list
				vecX.push_back(r);
				vecY.push_back(c);
			}
		}
	}
}


// Function that will compute the normalized line integral(for Daugman's Integro Differential Operator) around a circular contour.
// A polygon of a large number of sides is used for aproximating a circle
// Assuming that the image that is being passed as argument is not in double, but in uint8
double lineint(Mat image, int centerX, int centerY, int r, int n, string part) {
	double theta = (2*PI)/n; // computes the angle of the n-sides polygon
	double L = 0.0, s = 0.0, value = 0.0; // variable initialization
	vector<double> angle; // vector that will store all of the angles from 0 to 2*pi theta by theta
	vector<double> x(n), y(n); // vectors with double x and y values, for the polar coordinates 
	int rows = image.rows; // amount of rows in the image
	int cols = image.cols; // amount of columns in the image

	// calculando o vetor angle, como na linha 28 do matlab
	// computing the angle vector as in the line 28 in matlab
	for (double i = theta; i <= 2 * PI + theta; i += theta) {
		angle.push_back(i);
	}

	// Storing in the x and y vectors the polar coordinates for each angle in angle vector
	for (int i = 0; i < n; i++) {
		x[i] = (centerX + 1) - r*sin(angle[i]);
		y[i] = (centerY + 1) + r*cos(angle[i]);
	}

	// line 31 matlab condition
	// it calls the function any, and if any of those conditions are true, the integral result is 0
	if (any(x, (double)rows, ">=") || any(y, (double)cols, ">=")
		|| any(x, 1, "<=") || any(y, 1, "<=")) {
		L = 0.0;
		return L;
	}

	//equivale a linha 37 do matlab
	// line 37 of matlab, and consists of computing the integral for pupil
	if(part.compare("pupil") == 0){
		s = 0.0;
		for (int i = 0; i < n; i++) {
			value = image.at<double>(round(x[i]) - 1, round(y[i]) - 1); // takes the pixel value for each
																	    // i-th value of x and y
			s += value;
		}
		L = s/n;
	}

	// line 47 of matlab, which consists on the iris integral computing
	if(part.compare("iris") == 0){
		s = 0.0;

		// loop from 0 to n/8
		for (int i = 0; i < round(n / 8); i++) {
			value = image.at<double>(round(x[i]) - 1, round(y[i]) - 1); // takes the pixel value for each
																	    // i-th value of x and y		
			s += value;
		}

		// loop from 3n/8 to 5n/8
		for (int i = round((3 * n) / 8); i < round((5 * n) / 8); i++) {
			value = image.at<double>(round(x[i]) - 1, round(y[i]) - 1); // takes the pixel value for each
																	    // i-th value of x and y		
			s += value;
		}

		// loop from 7n/8 to n
		for (int i = round((7 * n) / 8); i < n; i++) {
			value = image.at<double>(round(x[i]) - 1, round(y[i]) - 1); // takes the pixel value for each
																	    // i-th value of x and y		
			s += value;
		}

		L = (2*s)/n;

	}

	return L;

}


/* Function that implements the function any from matlab, which is used in the function lineint
  It consists on a function that receives a vector, a value and a string and based on the string, that must be
  the following logic comparators: ==, !=, >= <=, > and <. And then, based on the comparator, it searches the value parameter
  in the vector
*/
bool any(vector<double> toBeSearchedVector, double paramValue, string logicComparator) {
	double value = 0.0;

	if(logicComparator.compare("==") == 0){
		for (int i = 0; i < toBeSearchedVector.size(); i++) {
			value = (int)(toBeSearchedVector[i] / 0.0001)*0.0001;
			if (value == paramValue) return true;
		}
	}
	else if(logicComparator.compare("!=") == 0){
		for (int i = 0; i < toBeSearchedVector.size(); i++) {
			value = (int)(toBeSearchedVector[i] / 0.0001)*0.0001;
			if (value != paramValue) return true;
		}
	}
	else if(logicComparator.compare(">=") == 0){
		for (int i = 0; i < toBeSearchedVector.size(); i++) {
			value = (int)(toBeSearchedVector[i] / 0.0001)*0.0001;
			if (value >= paramValue) return true;
		}
	}
	else if(logicComparator.compare("<=") == 0){
		for (int i = 0; i < toBeSearchedVector.size(); i++) {
			value = (int)(toBeSearchedVector[i] / 0.0001)*0.0001;
			if (value <= paramValue) return true;
		}
	}
	else if(logicComparator.compare(">") == 0){
		for (int i = 0; i < toBeSearchedVector.size(); i++) {
			value = (int)(toBeSearchedVector[i] / 0.0001)*0.0001;
			if (value > paramValue) return true;
		}
	}
	else if(logicComparator.compare("<") == 0){
		for (int i = 0; i < toBeSearchedVector.size(); i++) {
			value = (int)(toBeSearchedVector[i] / 0.0001)*0.0001;
			if (value < paramValue) return true;
		}
	}

	// returns false if its neither of the logic comparators defined above
	return false;
}

// Function that implements the partiald.m code from matlab and computes the partial derivative of the normalized
// line integral(for Daugman's Integro Differential Operator), holding the centre coordinates constant
// and then smooths it by a gaussian of appropriate sigma. Function also returns the maximum value of blur and the corresponding radius
void partiald(double *b, int *r, vector<double> &blur, Mat *image, int centerX, int centerY, int rmin, int rmax, float sigma, int n, string part)
{
	int count;					// count from the matlab code 
	int p = 0;
	double L_auxiliar;			// temp variable for the output of the lineint function
	vector<int> R;				// R from matlab code
	vector<double> L;			// L from matlab code
	vector<double> D;			// D from matlab code
	vector<double> f;			// f from matlab code


	// storing on the vector R the sum of rmin with each value between the rmax - rmin range
	for (int i = 0; i <= (rmax - rmin); i++) { R.push_back(rmin + i); }		/* line 23 from matlab */
	count = R.size();														/* line 24 from matlab */

	for (int k = 0; k < count; k++)
	{
		L_auxiliar = lineint(*image, centerX, centerY, R[k], n, part);

		if (L_auxiliar == 0.0)
		{
			break;
		}
		else
		{
			L.push_back(L_auxiliar);		// storing the result of the line integral in the L vector
		}
	}

	D.push_back(0);							// line 35 from matlab 

	// matlab diff() function implementation with a row vector argument
	// it computes the derivative of the line integral result
	for (int j = 1; j < L.size(); j++)
	{
		D.push_back(L[j] - L[j - 1]);
	}

	// lines 38 to 42 from matlab
	// generating the convolution kernel f
	if (sigma == 1.0)
	{
		for (int i = 0; i < 7; i++) { f.push_back(1.0 / 7.0); }
	}
	else
	{
		f.push_back(0.0003);	f.push_back(0.1065);
		f.push_back(0.7866);	f.push_back(0.1065);
		f.push_back(0.0003);
	}

	// calling the convolution function, using f as kernel
	blur = conv(D, f, "same");

	// computing the absolute value of each value in the convoluted vector blur
	for (int i = 0; i < blur.size(); i++)
	{
		blur[i] = abs(blur[i]);
	}

	// lines 46 to 48 from matlab
	// it computes the largest number in the range passed as argument
	*b = *max_element(blur.begin(), blur.end());

	// loop for finding the index with the largest value of the convoluted blur vector
	for (int i = 0; i < blur.size(); i++)
	{
		if (blur[i] == *b)
		{
			p = i;
			break;
		}
	}

	// computes the blurred partial derivative
	*r = R[p];
	*b = blur[p];

}
// Function to detect the pupil or iris boundary it searches a certain subset of the image with a given
// radius range(rmin,rmax) around a 10*10 neighbourhood of the point x,y given as input
void search(Mat image, int rmin, int rmax, int x, int y, string option){
	int rows = image.rows; // amount of rows in the image
	int cols = image.cols; // amount of columns in the image
	int radius = 0; // variable for storing the pupil or iris radius
	vector<double> blur; // variable used for receiving one of partiald function outputs
	int r = 0; // variable that stores one of partiald results
	double b = 0.0; // variable that stores one of partiald results
	double sigma = 0.5; // sigma that will be used on partiald function
	vector<int> R;
	Mat maxb = Mat::zeros(rows, cols, CV_64F);	// same as in matlab, receives the radius computed in the partiald function
	Mat maxrad = Mat::zeros(rows, cols, CV_64F); // same as in matlab, receives the max value of the convoluted vector, that contains the derivative of the line integral

	// Filling R vector, as in the line 20 in the matlab function
	for (int i = rmin; i <= rmax; i++) {
		R.push_back(i);
	}

	// loop that starts at the line 23 from the matlab function
	// variables that stores the values of the circle center
	int centerX = 0;
	int centerY = 0;
	// the center will receive values depending on where it is in 10*10 the neighbourhood of the point x, y
	// and it will compute partial derivative for each value in the range
	for (int i = x - 5; i <= x + 5; i++) {
		for (int j = y - 5; j <= y + 5; j++) {
			centerX = i;
			centerY = j;
			partiald(&b, &r, blur, &image, centerX, centerY, rmin, rmax, sigma, 600, option);
			maxrad.at<double>(i, j) = (double)r;
			maxb.at<double>(i, j) = b;
		}
	}
	// lines 30 and 31 from matlab
	int X = 0, Y = 0; // vai receber o resultado da funcao findMaxb
	findMaxb(&maxb, &X, &Y);
	radius = maxrad.at<double>(X - 1, Y - 1);

	// ci stores values related to iris and cp to pupil
	// if the option passed as argument is pupil, then the global variable cp will receive the values computed
	// else if is iris, ci receives de values

	// Since this code was adapted from matlab, X is equal to Y and Y is equal to X
	if(option.compare("pupil") == 0){
		cp[0] = X; // Y
		cp[1] = Y; // X
		cp[2] = radius;
	}
	else if(option.compare("iris") == 0){
		ci[0] = X; // Y
		ci[1] = Y; // X
		ci[2] = radius;
	}
	else {
		// if is not pupil nor iris
		return;
	}
}

// Function to search for the centre coordinates of the pupil, iris and both radius
// It makes use of Camus&Wildes' method to select the possible centre coordinates first
// The method consist of thresholding followed by checking if the selected points(by thresholding) correspond to a local minimum in 
// their immediate(3*s) neighbourhood these points serve as the possible centre coordinates for the iris.Once the iris has 
// been detected(using Daugman's method);the pupil's centre coordinates are found by searching a 10*10 neighbourhood around 
// the iris centre and varying the radius until a maximum is found (using  Daugman's integrodifferential operator)
void thresh(Mat image, double rmin, double rmax, double scale)
{

	Mat output;							// image with the pupil center detected
	double M = 1.0;						// minimum value 
	int rows = 0, cols = 0, r = 0;
	double b = 0.0;
	vector<double> blur;
	int centerX = 0, centerY = 0; // variables for the x,y coordinates of the pupil centre
	rmin = rmin * scale;
	rmax = rmax * scale;

	cvtColor(image, output, CV_BGR2GRAY);			// converting the image to gray scale e using output as the result of this conversion

	// reflection removal
	// instead of following the matlab implementation, that used the imfill() function
	// we used the morphologic operation of erose
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(7, 7)); // kernel used for the erosion
	erode(output, output, element);
	GaussianBlur(output, output, Size(5, 5), 2, 2);

	output.convertTo(output, CV_64F, 1.0 / 255);		// converting the output to double
	resize(output, output, Size(), scale, scale);

	rows = output.rows;
	cols = output.cols;

	list<int> vecX;		// list that stores the x axis features of the image
	list<int> vecY;		// list that stores the y axis features of the image
	find(&output, vecX, vecY); // function call that stores the pixels of the image that are lesser than 0.5

	// implementation of the lines 45 to 56 from matlab code
	for (list<int>::iterator it = vecX.begin(), it2 = vecY.begin(); it != vecX.end() && it2 != vecY.end(); it++, it2++){
	
		if ((*it >(rmin - 1)) && (*it2 > (rmin - 1)) && (*it <= ((rows - rmin) - 1)) && (*it2 < ((cols - rmin) - 1)))
		{
			M = 1.0;
			// computing the local minimum
			for (int u = -1; u <= 1; u++)
			{
				for (int j = -1; j <= 1; j++)
				{
					if (output.at<double>(*it + u, *it2 + j) < M)
					{
						M = output.at<double>(*it + u, *it2 + j);
					}
				}
			}

			if (output.at<double>(*it, *it2) != M)	// if the pixel is not the local minimum
			{
				*it = -1;		// NaN
				*it2 = -1;		// NaN 
			}
		}
	}

	
	findCloseBorder(vecX, vecY, rmin, rmax, rows, cols); // ignore the local minimum that are close to the borders
	findNan(vecX, vecY); // erases the NaN presence from the forward list
	
	Mat maxb = Mat::zeros(rows, cols, CV_64F);			// line 68 from matlab 
	Mat maxrad = Mat::zeros(rows, cols, CV_64F);		// line 69 from matlab 


	// implementation of the lines 72 to 76 from matlab
	for (list<int>::iterator it = vecX.begin(), it2 = vecY.begin(); it != vecX.end() && it2 != vecY.end(); it++, it2++){

		centerX = *it;
		centerY = *it2;

		partiald(&b, &r, blur, &output, centerX, centerY, rmin, rmax, 1.0, 400, "pupil");

		maxb.at<double>(*it, *it2) = b;
		maxrad.at<double>(*it, *it2) = (double)r;
	}

	int x, y;
	findMaxb(&maxb, &x, &y);

	search(output, rmin, rmax, x, y, "pupil");

	// adjusting the features acquired for the original scale
	cp[0] = cp[0] / scale;
	cp[1] = cp[1] / scale;
	cp[2] = cp[2] / scale;

}
