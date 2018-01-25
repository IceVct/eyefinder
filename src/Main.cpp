// Main for the pupil center algorithm

#include "Main.hpp"


int main(int argsc, char *argsv[]){

	double rmin, rmax;											// rminiris and rmaxiris are the minimum and maximum values of the iris radius 
	Mat image;
	bool webCam = false, camCenter = false;

	if(argsc < 2){
		std::cout << "At least two arguments must be passed!" << std::endl;
		exit(1);
	}
	

	rmin = 30.00;						
	rmax = 90.00;						

	
	// if the bool variable webCam is true, the images will be aquired by the camera, else, by argument
    if(webCam){
    	VideoCapture capture;

    	Mat frame;
    	capture.open(0);

    	// properties for the camera frame capture
		capture.set(CV_CAP_PROP_FRAME_WIDTH, 400);
		capture.set(CV_CAP_PROP_FRAME_HEIGHT, 400);

	    if (!capture.isOpened()){ 
	    	cout << "--(!)Error opening video capture\n" << endl;
	    	return -1; 
	    }
		
	    while (capture.read(frame)){
	        if(frame.empty()){
	            cout << " --(!) No captured frame -- Break!" << endl;
	            break;
	        }

	        // draws a red cross in the frame
	        if(!camCenter){
	        	line(frame, Point(0, frame.rows/2.0), Point(frame.cols - 1, frame.rows/2.0), Scalar(0, 0, 255), 2, 8);
	        	line(frame, Point(frame.cols/2.0, 0), Point(frame.cols/2.0, frame.rows - 1), Scalar(0, 0, 255), 2, 8);
	        }else{
	        	line(frame, Point(0, frame.rows/2.0), Point(frame.cols - 1, frame.rows/2.0), Scalar(0, 255, 0), 2, 8);
	        	line(frame, Point(frame.cols/2.0, 0), Point(frame.cols/2.0, frame.rows - 1), Scalar(0, 255, 0), 2, 8);
	        }

			thresh(frame, rmin, rmax, 0.2);
			drawMarker(frame, Point(cp[1], cp[0]), Scalar(0, 255, 0));

			// if the camera center is equal to the pupil center, repaint the line as green
			if(Point(cp[1], cp[0]) == Point(frame.cols/2.0, frame.rows/2.0)){
				camCenter = true;
			}else{
				camCenter = false;
			}

	        imshow("OUTPUT", frame);

	        char c = (char)waitKey(70);
	        if(c == 27) break; // escape
	    }

	    // cout << capture.get(CV_CAP_PROP_FRAME_WIDTH) << endl;
	    // cout << capture.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
	    
    }else{

		// reading the image		
		image = imread(argsv[1], CV_LOAD_IMAGE_UNCHANGED);

		// evaluating the image to the function thresh
		thresh(image, rmin, rmax, 0.3);

		drawMarker(image, Point(cp[1], cp[0]), Scalar(0, 255, 0));
		namedWindow( "OUTPUT IMAGE", CV_WINDOW_NORMAL );// Create a window for display.
		imshow( "OUTPUT IMAGE", image);                   // Show our image inside it.
	 	waitKey(0);  

    }
		
	return 0;
}
