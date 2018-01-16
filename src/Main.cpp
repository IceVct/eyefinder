/* OperadorIntegroDiferencial.cpp : Tradução do codigo Operador Integro Diferencial de Matlab para C++ */
// Main for the pupil center algorithm

#include "Main.hpp"


int main(int argsc, char *argsv[]){

	double rmin, rmax;											// rminiris and rmaxiris are the minimum and maximum values of the iris radius 
	Mat image;
	bool webCam = false;

	if(argsc < 2){
		std::cout << "At least two arguments must be passed!" << std::endl;
		exit(1);
	}
	

	rmin = 30.00;						
	rmax = 150.00;						

	
	// if the bool variable webCam is true, the images will be aquired by the camera, else, by argument
    if(webCam){
    	VideoCapture capture;

    	Mat frame;
    	capture.open(0);

    	// properties for the camera frame capture
		capture.set(CV_CAP_PROP_FRAME_WIDTH, 400);
		capture.set(CV_CAP_PROP_FRAME_HEIGHT, 300);
		capture.set(CV_CAP_PROP_FPS, 5);

	    if (!capture.isOpened()){ 
	    	cout << "--(!)Error opening video capture\n" << endl;
	    	return -1; 
	    }
		
	    while (capture.read(frame)){
	        if(frame.empty()){
	            cout << " --(!) No captured frame -- Break!" << endl;
	            break;
	        }

			thresh(frame, rmin, rmax, 0.2);
			drawMarker(frame, Point(ci[1], ci[0]), Scalar(0, 255, 0));
	        imshow("OUTPUT", frame);

	        char c = (char)waitKey(100);
	        if(c == 27) break; // escape
	    }

	    cout << capture.get(CV_CAP_PROP_FRAME_WIDTH) << endl;
	    cout << capture.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
	    
    }else{

		// reading the image		
		image = imread(argsv[1], CV_LOAD_IMAGE_UNCHANGED);

		// evaluating the image to the function thresh
		thresh(image, rmin, rmax, 0.3);

		drawMarker(image, Point(ci[1], ci[0]), Scalar(0, 255, 0));
		namedWindow( "OUTPUT IMAGE", CV_WINDOW_NORMAL );// Create a window for display.
		imshow( "OUTPUT IMAGE", image);                   // Show our image inside it.
	 	waitKey(0);  

    }
		
	return 0;
}
