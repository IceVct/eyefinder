// Main for the pupil center algorithm

#include "Main.hpp"

int main(int argsc, char *argsv[]){

	double rmin, rmax;											// rmin and rmax are the minimum and maximum values of the iris radius 
	Mat image;
	bool webCam = true;
	int platformSucc = 0;

	if(argsc < 3){
		cout << "At least three arguments must be passed!" << endl;
		exit(1);
	}
	
	int centerCameraX = 0, centerCameraY = 0, dx = 0, dy = 0;

	rmin = 30.00;						
	rmax = 90.00;						

	// if the bool variable webCam is true, the images will be aquired by the camera, else, by argument

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
	centerCameraX = capture.get(CV_CAP_PROP_FRAME_WIDTH)/2;
	centerCameraY = capture.get(CV_CAP_PROP_FRAME_HEIGHT)/2;
	printf("camera (%d, %d)\n", centerCameraX, centerCameraY);

    while (capture.read(frame)){

    	if(counter <= 100){
    		counter++;
    	}

    	int64 start = cv::getTickCount();
        if(frame.empty()){
            cout << " --(!) No captured frame -- Break!" << endl;
            break;
        }

        
        if(counter == 99){
        	thresh(frame, rmin, rmax, 0.3);
        	printf("pupil (%d, %d)\n", cp[1], cp[0]);
			platformSucc = controlPlatform(cp[1], cp[0], centerCameraX, centerCameraY, argsv[1]);
			if(platformSucc == ERROR){
				exit(1);
			}
			drawMarker(frame, Point(cp[1], cp[0]), Scalar(0, 255, 0));
			drawMarker(frame, Point(centerCameraX, centerCameraY), Scalar(0, 0, 255));
			imshow("pupil", frame);
			//waitKey(0);
        }             

        drawMarker(frame, Point(centerCameraX, centerCameraY), Scalar(0, 0, 255));
        
        imshow("OUTPUT", frame);

        char c = (char)waitKey(10);
        if(c == 27) break; // escape

        double fps = cv::getTickFrequency() / (cv::getTickCount() - start);
    	//cout << "FPS : " << fps << endl;
    }

    cout << capture.get(CV_CAP_PROP_FRAME_WIDTH) << endl;
    cout << capture.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
	    
    
		
	return 0;
}
