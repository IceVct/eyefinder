// Main for the pupil center algorithm

#include "Main.hpp"

int main(int argsc, char *argsv[]){

	double rmin, rmax;											// rminiris and rmaxiris are the minimum and maximum values of the iris radius 
	Mat image;
	bool webCam = true, camCenter = false;

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
		
		clock_t start = 0, finish = 0;
		int counter = 0;
		int centerCameraX = 0, centerCameraY = 0, dx = 0, dy = 0;
		centerCameraX = capture.get(CV_CAP_PROP_FRAME_WIDTH)/2;
		centerCameraY = capture.get(CV_CAP_PROP_FRAME_HEIGHT)/2;
		printf("camera (%d, %d)\n", centerCameraX, centerCameraY);

	    while (capture.read(frame)){

	    	if(counter <= 60){
	    		counter++;
	    	}

	    	int64 start = cv::getTickCount();
	        if(frame.empty()){
	            cout << " --(!) No captured frame -- Break!" << endl;
	            break;
	        }

	        
	        if(counter == 59){
	        	thresh(frame, rmin, rmax, 0.2);
	        	printf("pupil (%d, %d)\n", cp[1], cp[0]);
	        	dx = computeDistanceX(cp[1], centerCameraX);
				dy = computeDistanceY(cp[0], centerCameraY);
				cout << "dx = " << dx << endl;
				cout << "dy = " << dy << endl;
				drawMarker(frame, Point(cp[1], cp[0]), Scalar(0, 255, 0));
				imshow("pupil", frame);
				waitKey(0);
	        }             

	        if(counter > 59) camCenter = controlPlatform(cp[1], cp[0], centerCameraX, centerCameraY);

	        if(!camCenter){
	        	drawMarker(frame, Point(centerCameraX, centerCameraY), Scalar(0, 0, 255));
	        }else{
	        	drawMarker(frame, Point(centerCameraX, centerCameraY), Scalar(255, 255, 255));
	        }

	        imshow("OUTPUT", frame);

	        char c = (char)waitKey(70);
	        if(c == 27) break; // escape

	        double fps = cv::getTickFrequency() / (cv::getTickCount() - start);
        	//std::cout << "FPS : " << fps << std::endl;
	    }

	    cout << capture.get(CV_CAP_PROP_FRAME_WIDTH) << endl;
	    cout << capture.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
	    
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
