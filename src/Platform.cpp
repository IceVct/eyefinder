#include "Platform.hpp"

// Functions that implements the platform related functions


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
void moveXLeft(){
	cout << "Moving x axis to left by 1mm" << endl;
}

// Function that moves the platform x axis right
void moveXRight(){
	cout << "Moving x axis to right by 1mm" << endl;
}

// Function that moves the platform y axis up
void moveYUp(){
	cout << "Moving y axis up by 1mm" << endl;
}

// Function that moves the platform y axis down
void moveYDown(){
	cout << "Moving y axis down by 1mm" << endl;
}

/* Function for implementing the platform moving logic
 It has as inputs the pupil center coordinates and
 the camera center coordinates */
bool controlPlatform(int pupilX, int pupilY, int cameraX, int cameraY){
	int distanceX = 0, distanceY = 0;

	// computing the distances for the x and y axis
	distanceX = computeDistanceX(pupilX, cameraX);
	distanceY = computeDistanceY(pupilY, cameraY);

	cout << distanceX << ", " << distanceY << endl;

	// CAMERA CENTER ON THE SAME COORDINATE THAT PUPIL CENTER
	// return true
	if(distanceX == 0 && distanceY == 0) return REACHED;

	// MOVE UP
	// camera center x coordinate is already on the same x coordinate that pupil center and camera y 
	// coordinate is higher than pupil center y coordinate
	if(distanceX == 0 && distanceY < 0){
		moveYUp();
	}

	// MOVE DOWN
	// camera center x coordinate is already on the same x coordinate that pupil center and camera y 
	// coordinate is lesser than pupil center y coordinate
	if(distanceX == 0 && distanceY > 0){
		moveYDown();
	}

	// MOVE RIGHT
	// camera center y coordinate is already on the same y coordinate that pupil center and camera x
	// coordinate is lesser than pupil center x coordinate
	if(distanceX > 0 && distanceY == 0){
		moveXRight();
	}

	// MOVE LEFT
	// camera center y coordinate is already on the same y coordinate that pupil center and camera x 
	// coordinate is higher than pupil center x coordinate
	if(distanceX < 0 && distanceY == 0){
		moveXLeft();
	}

	// MOVE UP RIGHT
	// camera center x coordinate is lesser than pupil center x coordinate and camera y coordinate
	// is higher than pupil y coordinate
	if(distanceX > 0 && distanceY < 0){
		moveYUp();
		moveXRight();
	}

	// MOVE UP LEFT
	// camera center x coordinate is higher than pupil center x coordinate and camera y coordinate
	// is higher than pupil y coordinate
	if(distanceX < 0 && distanceY < 0){
		moveYUp();
		moveXLeft();
	}

	// MOVE DOWN RIGHT
	// camera center x coordinate is lesser than pupil center x coordinate and camera y coordinate
	// is lesser than pupil y coordinate
	if(distanceX > 0 && distanceY > 0){
		moveYDown();
		moveXRight();
	}

	// MOVE DOWN LEFT
	// camera center x coordinate is higher than pupil center x coordinate and camera y coordinate
	// is lesser than pupil y coordinate
	if(distanceX < 0 && distanceY > 0){
		moveYDown();
		moveXLeft();
	}

	return NOTREACHED; // camera center still not in the same coordinate that pupil center

}