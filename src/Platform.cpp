#include "Platform.hpp"

// Masks for the platform commands (in bytes)
uint8_t XAXIS = 0x12;
uint8_t YAXIS = 0x22;
uint8_t RIGHT = 0x00; // up for Y axis
uint8_t LEFT = 0x40; // down for Y axis


// Functions that implements the platform related functions

struct sp_port *port;

// Function that receives a value in pixels and convert it to um (micrometers)
// so that can be passed as argument to the camera platform
int pixelToMicroMeters(int pixelValue){
	// 1 pixel is equal to 264,5833 um, so 265 is a rounded value
	return (int)264.58*pixelValue;
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
int controlPlatform(int pupilX, int pupilY, int cameraX, int cameraY, char *portName){
	int distanceX = 0, distanceY = 0;
	int umX = 0, umY = 0; // distances in micrometers of each axis
	int successX = 0, successY = 0; // variables that will be used to check if the commands were sent correctly to the platform
	struct sp_port *port; // structure for opening the port that will send the commands to the platform

	// computing the distances for the x and y axis
	distanceX = computeDistanceX(pupilX, cameraX);
	distanceY = computeDistanceY(pupilY, cameraY);

	//converting the distances to um so that can be passed as argument to the camera platform
	umX = pixelToMicroMeters(distanceX);
	umY = pixelToMicroMeters(distanceY);

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
	}

	// MOVE DOWN
	// camera center x coordinate is already on the same x coordinate that pupil center and camera y 
	// coordinate is lesser than pupil center y coordinate
	if(distanceX == 0 && distanceY > 0){
		successY = moveYDown(umY, port);
	}

	// MOVE RIGHT
	// camera center y coordinate is already on the same y coordinate that pupil center and camera x
	// coordinate is lesser than pupil center x coordinate
	if(distanceX > 0 && distanceY == 0){
		successX = moveXRight(umX, port);
	}

	// MOVE LEFT
	// camera center y coordinate is already on the same y coordinate that pupil center and camera x 
	// coordinate is higher than pupil center x coordinate
	if(distanceX < 0 && distanceY == 0){
		umX = -umX;
		successX = moveXLeft(umX, port);
	}

	// MOVE UP RIGHT
	// camera center x coordinate is lesser than pupil center x coordinate and camera y coordinate
	// is higher than pupil y coordinate
	if(distanceX > 0 && distanceY < 0){
		umY = -umY;
		successY = moveYUp(umY, port);
		successX = moveXRight(umX, port);
	}

	// MOVE UP LEFT
	// camera center x coordinate is higher than pupil center x coordinate and camera y coordinate
	// is higher than pupil y coordinate
	if(distanceX < 0 && distanceY < 0){
		umX = -umX;
		umY = -umY;
		successY = moveYUp(umY, port);
		successX = moveXLeft(umX, port);
	}

	// MOVE DOWN RIGHT
	// camera center x coordinate is lesser than pupil center x coordinate and camera y coordinate
	// is lesser than pupil y coordinate
	if(distanceX > 0 && distanceY > 0){
		successY = moveYDown(umY, port);
		successX = moveXRight(umX, port);
	}

	// MOVE DOWN LEFT
	// camera center x coordinate is higher than pupil center x coordinate and camera y coordinate
	// is lesser than pupil y coordinate
	if(distanceX < 0 && distanceY > 0){
		umX = -umX;
		successY = moveYDown(umY, port);
		successX = moveXLeft(umX, port);
	}

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