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
	return 265*pixelValue;
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
void moveXLeft(int distanceToMove){
	uint8_t thrdByte = 0, fourthByte = 0; // the bytes that consists on the distance that the axis will be shifted

	printf("Moving x axis to left by %d(0x%X) um\n", distanceToMove, distanceToMove);
	thrdByte = (distanceToMove >> 8) & 0xFF;
	fourthByte = distanceToMove & 0x000000FF;
	printf("Command that will be passed = 0x%X%X%X%X\n", XAXIS, LEFT, thrdByte, fourthByte);
}

// Function that moves the platform x axis right
void moveXRight(int distanceToMove){
	uint8_t thrdByte = 0, fourthByte = 0; // the bytes that consists on the distance that the axis will be shifted

	printf("Moving x axis to right by %d(0x%X) um\n", distanceToMove, distanceToMove);
	thrdByte = (distanceToMove >> 8) & 0xFF;
	fourthByte = distanceToMove & 0x000000FF;
	printf("Command that will be passed = 0x%X%X%X%X\n", XAXIS, RIGHT, thrdByte, fourthByte);
}

// Function that moves the platform y axis up
void moveYUp(int distanceToMove){
	uint8_t thrdByte = 0, fourthByte = 0; // the bytes that consists on the distance that the axis will be shifted
	
	printf("Moving y axis up by %d(0x%X) um\n", distanceToMove, distanceToMove);
	thrdByte = (distanceToMove >> 8) & 0xFF;
	fourthByte = distanceToMove & 0x000000FF;
	printf("Command that will be passed = 0x%X%X%X%X\n", YAXIS, RIGHT, thrdByte, fourthByte);
}

// Function that moves the platform y axis down
void moveYDown(int distanceToMove){
	uint8_t thrdByte = 0, fourthByte = 0; // the bytes that consists on the distance that the axis will be shifted
	
	printf("Moving y axis down by %d(0x%X) um\n", distanceToMove, distanceToMove);
	thrdByte = (distanceToMove >> 8) & 0xFF;
	fourthByte = distanceToMove & 0x000000FF;
	printf("Command that will be passed = 0x%X%X%X%X\n", YAXIS, LEFT, thrdByte, fourthByte);
}

/* Function for implementing the platform moving logic
 It has as inputs the pupil center coordinates and
 the camera center coordinates */
void controlPlatform(int pupilX, int pupilY, int cameraX, int cameraY){
	int distanceX = 0, distanceY = 0;
	int umX = 0, umY = 0; // distances in micrometers of each axis

	// computing the distances for the x and y axis
	distanceX = computeDistanceX(pupilX, cameraX);
	distanceY = computeDistanceY(pupilY, cameraY);

	//converting the distances to um so that can be passed as argument to the camera platform
	umX = pixelToMicroMeters(distanceX);
	umY = pixelToMicroMeters(distanceY);

	cout << distanceX << ", " << distanceY << endl;
	cout << umX << ", " << umY << endl;

	// MOVE UP
	// camera center x coordinate is already on the same x coordinate that pupil center and camera y 
	// coordinate is higher than pupil center y coordinate
	if(distanceX == 0 && distanceY < 0){
		umY = -umY;
		moveYUp(umY);
	}

	// MOVE DOWN
	// camera center x coordinate is already on the same x coordinate that pupil center and camera y 
	// coordinate is lesser than pupil center y coordinate
	if(distanceX == 0 && distanceY > 0){
		moveYDown(umY);
	}

	// MOVE RIGHT
	// camera center y coordinate is already on the same y coordinate that pupil center and camera x
	// coordinate is lesser than pupil center x coordinate
	if(distanceX > 0 && distanceY == 0){
		moveXRight(umX);
	}

	// MOVE LEFT
	// camera center y coordinate is already on the same y coordinate that pupil center and camera x 
	// coordinate is higher than pupil center x coordinate
	if(distanceX < 0 && distanceY == 0){
		umX = -umX;
		moveXLeft(umX);
	}

	// MOVE UP RIGHT
	// camera center x coordinate is lesser than pupil center x coordinate and camera y coordinate
	// is higher than pupil y coordinate
	if(distanceX > 0 && distanceY < 0){
		umY = -umY;
		moveYUp(umY);
		moveXRight(umX);
	}

	// MOVE UP LEFT
	// camera center x coordinate is higher than pupil center x coordinate and camera y coordinate
	// is higher than pupil y coordinate
	if(distanceX < 0 && distanceY < 0){
		umX = -umX;
		umY = -umY;
		moveYUp(umY);
		moveXLeft(umX);
	}

	// MOVE DOWN RIGHT
	// camera center x coordinate is lesser than pupil center x coordinate and camera y coordinate
	// is lesser than pupil y coordinate
	if(distanceX > 0 && distanceY > 0){
		moveYDown(umY);
		moveXRight(umX);
	}

	// MOVE DOWN LEFT
	// camera center x coordinate is higher than pupil center x coordinate and camera y coordinate
	// is lesser than pupil y coordinate
	if(distanceX < 0 && distanceY > 0){
		umX = -umX;
		moveYDown(umY);
		moveXLeft(umX);
	}

}