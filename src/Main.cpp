// Main for the pupil center algorithm

#include "Main.hpp"

void parse_serial(char *byte_buff, int byte_num) {
  for(int i = 0; i < byte_num;i++){
    printf("%c", byte_buff[i]);
  }
  printf("\n");
}

void list_ports() {
  int i;
  struct sp_port **ports;

  sp_return error = sp_list_ports(&ports);
  if (error == SP_OK) {
    for (i = 0; ports[i]; i++) {
      printf("Found port: '%s'\n", sp_get_port_name(ports[i]));
    }
    sp_free_port_list(ports);
  } else {
    printf("No serial devices detected\n");
  }
  printf("\n");
}

int main(int argsc, char *argsv[]){

	double rmin, rmax;											// rminiris and rmaxiris are the minimum and maximum values of the iris radius 
	Mat image;
	bool webCam = false, camCenter = false;

	if(argsc < 2){
		cout << "At least two arguments must be passed!" << endl;
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

	    	if(counter <= 100){
	    		counter++;
	    	}

	    	int64 start = cv::getTickCount();
	        if(frame.empty()){
	            cout << " --(!) No captured frame -- Break!" << endl;
	            break;
	        }

	        
	        if(counter == 99){
	        	thresh(frame, rmin, rmax, 0.2);
	        	printf("pupil (%d, %d)\n", cp[1], cp[0]);
				controlPlatform(cp[1], cp[0], centerCameraX, centerCameraY);
				drawMarker(frame, Point(cp[1], cp[0]), Scalar(0, 255, 0));
				drawMarker(frame, Point(centerCameraX, centerCameraY), Scalar(0, 0, 255));
				imshow("pupil", frame);
				waitKey(0);
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
	    
    }else{

      struct sp_port *port;
      list_ports();
      const char *desired_port = "/dev/ttyUSB0";
      printf("Opening port '%s' \n", desired_port);
	  sp_return error = sp_get_port_by_name(desired_port,&port);
	  if (error == SP_OK) {
	    error = sp_open(port,SP_MODE_WRITE);
	    if (error == SP_OK) {
	      int byte_num;
	      sp_set_baudrate(port,250000);
	 		uint8_t s[] = {0x20, 0x00, 0xff, 0xa2};
	        byte_num = sp_nonblocking_write(port, s, 4);
	        //parse_serial(byte_buff,byte_num);
	        //fflush(stdout);
	      

	      sp_close(port);
	    } else {
	      printf("Error opening serial device\n");
	    }
	  } else {
	    printf("Error finding serial device\n");
	  }

		// reading the image		
		//image = imread(argsv[1], CV_LOAD_IMAGE_UNCHANGED);

		// evaluating the image to the function thresh
		//thresh(image, rmin, rmax, 0.3);

		// drawMarker(image, Point(cp[1], cp[0]), Scalar(0, 255, 0));
		// namedWindow( "OUTPUT IMAGE", CV_WINDOW_NORMAL );// Create a window for display.
		// imshow( "OUTPUT IMAGE", image);                   // Show our image inside it.
	 // 	waitKey(0);  

    }
		
	return 0;
}
