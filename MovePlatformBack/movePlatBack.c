#include <stdio.h>
#include <libserialport.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

// Code that reads the file generated by the main program and moves the platform to the original position
// Receives one argument: the port device name
int main(int argsc, char *argsv[]){
	FILE *fileDistances = NULL; // file descriptor
	struct sp_port *port;
	int amountCommands = 0, distanceToMove1 = 0, distanceToMove2 = 0, numBytes = 0;
	int numBytes2 = 0;
	uint8_t platCommands[4], platCommX[4], platCommY[4]; // commands that going to be sent to the platform
	char inpString[50];

	if(argsc < 2){
		printf("At least 2 arguments must be passed!\n");
		exit(1);
	}

	// opening the file that contains the commands for being done
	fileDistances = fopen("../distanceAndDirection.txt", "r");
	if(!fileDistances){
		printf("Unable to open the file!\n");
		exit(1);
	}

	// port necessary configurations
	// finding the port device
	int getPort = sp_get_port_by_name(argsv[1], &port);
	if(getPort != SP_OK){
		printf("Failed to find the device with this port name!\n");
		exit(1);
	}

	// opening the port device
	if(sp_open(port, SP_MODE_READ_WRITE) == SP_OK){
	  	sp_set_baudrate(port, 250000); // default for the platform
	}else{
		printf("Failed to open the port!\n");
		exit(1);
	}  

	// reading the first line of the file and discovering the amount of commands
	fscanf(fileDistances, "%d\n", &amountCommands);
	if(amountCommands == 1){
		fscanf(fileDistances, "%hhx %hhx %d\n", &platCommands[0], &platCommands[1], &distanceToMove1);

		// the maximum distance for each direction of axis x is 3,75 cm and for axis z is 1,25 cm, so if 
		// the distance to move is higher than that distance to move must be changed to 37500 or 12500
		if(platCommands[0] == 0x12 && distanceToMove1 > 37500) distanceToMove1 = 37500;
		if(platCommands[0] == 0x22 && distanceToMove1 > 12500) distanceToMove1 = 12500; // TODO: CHANGE FOR THE Z LOGIC

		platCommands[2] = (distanceToMove1 >> 8) & 0xFF;
		platCommands[3] = distanceToMove1 & 0x000000FF;

		printf("Command %x %x %x %x\n", platCommands[0], platCommands[1], platCommands[2], platCommands[3]);

		//send the write command to the port
		numBytes = sp_nonblocking_write(port, platCommands, sizeof(platCommands));
		if(numBytes != sizeof(platCommands)){
			printf("Unable to send the commands to the platform!\n");
		}

	}else{
		fscanf(fileDistances, "%hhx %hhx %d\n", &platCommX[0], &platCommX[1], &distanceToMove1);
		fscanf(fileDistances, "%hhx %hhx %d\n", &platCommY[0], &platCommY[1], &distanceToMove2);

		// the maximum distance for each direction of axis x is 3,75 cm and for axis z is 1,25 cm, so if 
		// the distance to move is higher than that distance to move must be changed to 37500 or 12500
		if(distanceToMove1 > 37500) distanceToMove1 = 37500;
		if(distanceToMove2 > 12500) distanceToMove2 = 12500; // TODO: CHANGE FOR THE Z LOGIC

		platCommX[2] = (distanceToMove1 >> 8) & 0xFF;
		platCommX[3] = distanceToMove1 & 0x000000FF;
		platCommY[2] = (distanceToMove2 >> 8) & 0xFF;
		platCommY[3] = distanceToMove2 & 0x000000FF;

		printf("Command X %x %x %x %x\n", platCommX[0], platCommX[1], platCommX[2], platCommX[3]);
		printf("Command Y %x %x %x %x\n", platCommY[0], platCommY[1], platCommY[2], platCommY[3]);
		// send the write command to the port
		numBytes = sp_nonblocking_write(port, platCommX, sizeof(platCommX));
		numBytes2 = sp_nonblocking_write(port, platCommY, sizeof(platCommY));
		if(numBytes != sizeof(platCommX) || numBytes2 != sizeof(platCommY)){
			printf("Unable to send the commands to the platform!\n");
		}
	}

	//closing the file
	fclose(fileDistances);

	// port freeing and closing
	if(sp_close(port) != SP_OK){
		printf("Failed to close the port!\n");
	    exit(1);
	}
	printf("Port closed successfully!\n");
	sp_free_port(port);

	return 0;
}