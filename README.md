# eyefinder / camera platform controlling

Camera platform control for finding eye in a videosequence

Project being developed in STRaDe research group in the Faculty of Information Techonogy in VUT (Brno University of Technology).

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for running purposes

### Prerequisites

Dependencies, libraries, softwares and OS used in this project.

Not necessary
```
Ubuntu 16.04 (Not necessary)
```

Necessary
```
CMake - at least version 2.8
```
```
OpenCV - version 3.3.0
```
```
GCC - version 5.4.0
```
```
libserialport - version 0.1.1
```

### Installing

A step by step series of examples that tell you have to get a development env running

Say what the step will be

The CMake file is already done, it's only necessary to follow the next steps for compiling the code

```
cd eyefinder
```
```
cmake .
```
```
make
```

## Running the code

The algorithm receives one argument, which is the port device (port_name) of the platform.

For running the code, there are two methods.

## First method
```
sudo ./eyefinder /dev/port_name
```
Usually, the port_name is /dev/ttyUSB0, but not necessarily.

## Second method

If you don't want to run as sudo, you must change the permission for the port device

```
sudo chmod 666 /dev/port_name
```

```
./eyefinder /dev/port_name
```
## Built With

* [OpenCV](https://opencv.org/) - Computer Vision library
* [libserialport](https://sigrok.org/wiki/Libserialport) - Library used for serial port communication
* [CMake](https://cmake.org/) - Used for building/compiling/linking C/C++ code


## Authors

* **Victor Araujo Vieira** - [IceVct](https://github.com/IceVct)


## Acknowledgments

* Algorithm to detect the pupil center was adapted from a translation of this https://www.mathworks.com/matlabcentral/fileexchange/15652-iris-segmentation-using-daugman-s-integrodifferential-operator?focused=3889799&tab=function matlab code to C++ using OpenCV


## After running the eyefinder algorithm
For returning the platform for the initial position, one code was developed. Follow the steps bellow for compiling and running this code.

### Compiling
In the main folder of the project "./eyefinder", run the following commands

```
cd MovePlatformBack
```

```
gcc movePlatBack.c -lserialport -o movePlatBack
```

### Running
For running this code, it is required one argument: the port device name

```
sudo ./movePlatBack /dev/port_name
```

or changing the permissions, as explained above and running normally.

