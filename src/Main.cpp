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

    	/* Port-related variables */
		struct sp_port **port_list = NULL;
		uint32_t n_ports = 0;
		struct sp_port *local_list[10] = { NULL };
		uint32_t local_index = 0;
		struct sp_port *the_port = NULL;
		 
		/* Buffer-related variables */
		uint32_t in_bytes = 0;
		uint8_t in_buf[128] = { 0x00 };
		uint32_t in_buf_bytes = 0;
		uint32_t out_bytes = 0;
		uint8_t out_buf[128] = "Hello World!";
		uint32_t out_buf_bytes = 12;
		 
		uint32_t res = 0;
		uint32_t ii = 0;
		 
	  /* 
	   * List port, copying to a local list 
	   */
	  printf("1. List ports ...\n");
	  if (sp_list_ports(&port_list) == SP_OK)
	  {
	    for(ii = 0; port_list[ii]; ii++)
	    {
	      printf("Found port '%s' .. ", sp_get_port_name(port_list[ii]));
	      if (!(sp_copy_port(port_list[ii], &local_list[ii])))
	      {
	        printf("copied OK\n");
	        n_ports++;
	      }
	      else
	      {
	        printf("copy FAILED !!\n");
	        break;
	      }
	    }
	  }
	  else
	  {
	    printf("Port list FAILED !!\n");
	    return 1;
	  }
	 
	  /* Free the list */
	  sp_free_port_list(port_list);
	 
	  /* 
	   * Set a port to open
	   */
	  the_port = local_list[local_index];
	 
	  /* 
	   * Open by list index instead name 
	   */
	  printf("2. Open port by index #%u ...\n", local_index);
	  if (sp_open(the_port, PORT_CONF_FLAGS) == SP_OK)
	  {
	       sp_set_baudrate(the_port, PORT_CONF_BAUDRATE);
	           sp_set_bits(the_port, PORT_CONF_BITS);
	         sp_set_parity(the_port, PORT_CONF_PARITY);
	       sp_set_stopbits(the_port, PORT_CONF_STOPBITS);
	    sp_set_flowcontrol(the_port, PORT_CONF_FLOWCONTROL);
	    printf(" + Port open and configure OK\n");
	  }
	  else
	  {
	    printf(" ! Port open FAILED\n");
	  }
	 
	  /*
	   * Write something to port
	   */
	  printf("3. Writing to port #%u ...\n - buffer data: ", local_index);
	  for (ii = 0; ii < out_buf_bytes; ii++)
	  {
	    printf("%02X", out_buf[ii]);
	  }
	  printf("\n");
	  out_bytes = sp_blocking_write(the_port, out_buf, out_buf_bytes, 1000);
	  if (out_bytes != out_buf_bytes)
	  {
	    printf(" ! Port write FAILED\n");
	    return 1;
	  }
	  printf(" + Port write OK\n");
	 
	  /*
	   * Read something from port
	   */
	  printf("4. Read from port #%u ...\n", local_index);
	  in_bytes = sp_blocking_read_next(the_port, in_buf, 128, 1000);
	  if (in_bytes != out_buf_bytes)
	  {
	    printf(" ! Received %u bytes, expected %u. Read FAILED\n",
	           in_bytes,
	           out_buf_bytes);
	    return 1;
	  }
	  printf(" - buffer data: ");
	  for (ii = 0; ii < in_bytes; ii++)
	  {
	    printf("%02X", out_buf[ii]);
	  }
	  printf("\n");
	  printf(" + Port read OK\n");
	 
	  /*
	   * Close and free resources
	   */
	  printf("5. Closing and freeing resources #%u ...\n", local_index);
	  if (sp_close(the_port) != SP_OK)
	  {
	    printf(" ! Port close FAILED\n");
	    return 1;
	  }
	  printf(" + Port close OK\n");
	  sp_free_port(the_port);
  

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
