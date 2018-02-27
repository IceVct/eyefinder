#include "Platform.hpp"

// Masks for the platform commands (in bytes)
uint8_t XAXIS = 0x12;
uint8_t YAXIS = 0x22;
uint8_t RIGHT = 0x00; // up for Y axis
uint8_t LEFT = 0x40; // down for Y axis


// Functions that implements the platform related functions

// Function that receives a value in pixels from the x axis and convert it to um (micrometers)
// so that can be passed as argument to the camera platform
int pixelToMicroMetersX(int pixelValueX){

	// the convertion varies depending on the distance of the camera to the face
	return 110*pixelValueX;
}

// Function that receives a value in pixels from the y axis and convert it to um (micrometers)
// so that can be passed as argument to the camera platform
int pixelToMicroMetersY(int pixelValueY){

	// the convertion varies depending on the distance of the camera to the face
	return 95*pixelValueY;
}

// Function that computes the distance from pupil center to camera center
// for the x axis
int computeDistanceX(int pupilX, int cameraX){

	return pupilX - cameraX;
}

// Function that computes the distance from pupil center to camera center
// for the y axis
int computeDistanceY(int pupilY, int cameraY){

	return pupilY - cameraY;
}

// Function that moves the platform x axis left
int moveXLeft(int distanceToMove, struct sp_port *port){
	uint8_t command[4]; // the bytes 3 and 4 consists on the distance that the axis will be shifted
	int numBytes = 0; 

	// building the command
	command[0] = XAXIS;
	command[1] = LEFT;
	printf("Moving x axis to left by %d(0x%X) um\n", distanceToMove, distanceToMove);
	command[2] = (distanceToMove >> 8) & 0xFF;
	command[3] = distanceToMove & 0x000000FF;
	printf("Command that will be passed = 0x%X%X%X%X\n", XAXIS, LEFT, command[2], command[3]);

	numBytes = sp_nonblocking_write(port, command, sizeof(command));
	if(numBytes != sizeof(command)){
		return ERROR;
	}

	return SUCCESS;
} 

// Function that moves the platform x axis right
int moveXRight(int distanceToMove, struct sp_port *port){
	uint8_t command[4]; // the bytes 3 and 4 consists on the distance that the axis will be shifted
	int numBytes = 0; 

	// building the command
	command[0] = XAXIS;
	command[1] = RIGHT;
	printf("Moving x axis to right by %d(0x%X) um\n", distanceToMove, distanceToMove);
	command[2] = (distanceToMove >> 8) & 0xFF;
	command[3] = distanceToMove & 0x000000FF;
	printf("Command that will be passed = 0x%X%X%X%X\n", XAXIS, RIGHT, command[2], command[3]);

	numBytes = sp_nonblocking_write(port, command, sizeof(command));
	if(numBytes != sizeof(command)){
		return ERROR;
	}

	return SUCCESS;
}

// Function that moves the platform y axis up
int moveYUp(int distanceToMove, struct sp_port *port){
	uint8_t command[4]; // the bytes 3 and 4 consists on the distance that the axis will be shifted
	int numBytes = 0; 

	// building the command
	command[0] = YAXIS;
	command[1] = RIGHT;
	printf("Moving y axis up by %d(0x%X) um\n", distanceToMove, distanceToMove);
	command[2] = (distanceToMove >> 8) & 0xFF;
	command[3] = distanceToMove & 0x000000FF;
	printf("Command that will be passed = 0x%X%X%X%X\n", YAXIS, RIGHT, command[2], command[3]);

	numBytes = sp_nonblocking_write(port, command, sizeof(command));
	if(numBytes != sizeof(command)){
		return ERROR;
	}

	return SUCCESS;
}

// Function that moves the platform y axis down
int moveYDown(int distanceToMove, struct sp_port *port){
	uint8_t command[4]; // the bytes 3 and 4 consists on the distance that the axis will be shifted
	int numBytes = 0; 

	// building the command
	command[0] = YAXIS;
	command[1] = LEFT;
	printf("Moving y axis down by %d(0x%X) um\n", distanceToMove, distanceToMove);
	command[2] = (distanceToMove >> 8) & 0xFF;
	command[3] = distanceToMove & 0x000000FF;
	printf("Command that will be passed = 0x%X%X%X%X\n", YAXIS, LEFT, command[2], command[3]);

	numBytes = sp_nonblocking_write(port, command, sizeof(command));
	if(numBytes != sizeof(command)){
		return ERROR;
	}

	return SUCCESS;
}

/* Function for implementing the platform moving logic. It has as inputs the pupil center coordinates and
 the camera center coordinates */
// The function also write in a file the distance and direction that the platform moved, so that when the program is over
// is only necessary to run another code that returns the platform to the original position
int controlPlatform(int pupilX, int pupilY, int cameraX, int cameraY, char *portName){
	int distanceX = 0, distanceY = 0;
	int umX = 0, umY = 0; // distances in micrometers of each axis
	int successX = 0, successY = 0; // variables that will be used to check if the commands were sent correctly to the platform
	struct sp_port *port; // structure for opening the port that will send the commands to the platform
	FILE *fileDistances = NULL; // file descriptor

	// computing the distances for the x and y axis
	distanceX = computeDistanceX(pupilX, cameraX);
	distanceY = computeDistanceY(pupilY, cameraY);

	//converting the distances to um so that can be passed as argument to the camera platform
	umX = pixelToMicroMetersX(distanceX);
	umY = pixelToMicroMetersY(distanceY);

	// file for writing the distances and directions that the platform was moved
	fileDistances = fopen("distanceAndDirection.txt", "w");
	if(!fileDistances){
		cout << "Unable to open the file!" << endl;
		return ERROR;
	}

	// port necessary configurations
	// finding the port device
	sp_return getPort = sp_get_port_by_name(portName, &port);
	if(getPort != SP_OK){
		cout << "Failed to find the device with this port name!" << endl;
		return ERROR;
	}

	// opening the port device
	if(sp_open(port, SP_MODE_READ_WRITE) == SP_OK){
	  	sp_set_baudrate(port, 250000); // default for the platform
	}else{
		cout << "Failed to open the port!" << endl;
		return ERROR;
	}  	

	// MOVE UP
	// camera center x coordinate is already on the same x coordinate that pupil center and camera y 
	// coordinate is higher than pupil center y coordinate
	if(distanceX == 0 && distanceY < 0){
		umY = -umY;
		successY = moveYUp(umY, port);
		
		// if the platform goes up, then return to the original position
		fprintf(fileDistances, "1\n");
		fprintf(fileDistances, "%x %x %d\n", YAXIS, LEFT, umY);
	}

	// MOVE DOWN
	// camera center x coordinate is already on the same x coordinate that pupil center and camera y 
	// coordinate is lesser than pupil center y coordinate
	if(distanceX == 0 && distanceY > 0){
		successY = moveYDown(umY, port);
		
		// if the platform goes down, then return to the original position
		fprintf(fileDistances, "1\n");
		fprintf(fileDistances, "%x %x %d\n", YAXIS, RIGHT, umY);
	}

	// MOVE RIGHT
	// camera center y coordinate is already on the same y coordinate that pupil center and camera x
	// coordinate is lesser than pupil center x coordinate
	if(distanceX > 0 && distanceY == 0){
		successX = moveXRight(umX, port);
		
		// if the platform goes right, then return to the original position
		fprintf(fileDistances, "1\n");
		fprintf(fileDistances, "%x %x %d\n", XAXIS, LEFT, umX);
	}

	// MOVE LEFT
	// camera center y coordinate is already on the same y coordinate that pupil center and camera x 
	// coordinate is higher than pupil center x coordinate
	if(distanceX < 0 && distanceY == 0){
		umX = -umX;
		successX = moveXLeft(umX, port);
		
		// if the platform goes left, then return to the original position
		fprintf(fileDistances, "1\n");
		fprintf(fileDistances, "%x %x %d\n", XAXIS, RIGHT, umX);
	}

	// MOVE UP RIGHT
	// camera center x coordinate is lesser than pupil center x coordinate and camera y coordinate
	// is higher than pupil y coordinate
	if(distanceX > 0 && distanceY < 0){
		umY = -umY;
		successY = moveYUp(umY, port);
		successX = moveXRight(umX, port);
		
		// if the platform goes up and right, then return to the original position
		fprintf(fileDistances, "2\n");
		fprintf(fileDistances, "%x %x %d\n", XAXIS, LEFT, umX);
		fprintf(fileDistances, "%x %x %d\n", YAXIS, LEFT, umY);
	}

	// MOVE UP LEFT
	// camera center x coordinate is higher than pupil center x coordinate and camera y coordinate
	// is higher than pupil y coordinate
	if(distanceX < 0 && distanceY < 0){
		umX = -umX;
		umY = -umY;
		successY = moveYUp(umY, port);
		successX = moveXLeft(umX, port);
		
		// if the platform goes up and left, then return to the original position
		fprintf(fileDistances, "2\n");
		fprintf(fileDistances, "%x %x %d\n", XAXIS, RIGHT, umX);
		fprintf(fileDistances, "%x %x %d\n", YAXIS, LEFT, umY);
	}

	// MOVE DOWN RIGHT
	// camera center x coordinate is lesser than pupil center x coordinate and camera y coordinate
	// is lesser than pupil y coordinate
	if(distanceX > 0 && distanceY > 0){
		successY = moveYDown(umY, port);
		successX = moveXRight(umX, port);
		
		// if the platform goes down and right, then return to the original position
		fprintf(fileDistances, "2\n");
		fprintf(fileDistances, "%x %x %d\n", XAXIS, LEFT, umX);
		fprintf(fileDistances, "%x %x %d\n", YAXIS, RIGHT, umY);
	}

	// MOVE DOWN LEFT
	// camera center x coordinate is higher than pupil center x coordinate and camera y coordinate
	// is lesser than pupil y coordinate
	if(distanceX < 0 && distanceY > 0){
		umX = -umX;
		successY = moveYDown(umY, port);
		successX = moveXLeft(umX, port);
		
		// if the platform goes down and left, then return to the original position
		fprintf(fileDistances, "2\n");
		fprintf(fileDistances, "%x %x %d\n", XAXIS, RIGHT, umX);
		fprintf(fileDistances, "%x %x %d\n", YAXIS, RIGHT, umY);
	}

	//closing the file
	fclose(fileDistances);

	// port freeing and closing
	if(sp_close(port) != SP_OK){
		cout << "Failed to close the port!" << endl;
	    return ERROR;
	}
	cout << "Port closed successfully!" << endl;
	sp_free_port(port);

	if(successX == ERROR || successY == ERROR){
		cout << "Error while sending commands to the platform" << endl;
		return ERROR;
	}

	return SUCCESS;
}