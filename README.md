# Movaic

Movaic (Movie Mosaic) is a command line application that can be used to create picture mosaics using a source image and a video clip.
 
<p align="center">
  <img width="535" height="800" src="https://github.com/GoldenBug/ReadMe_Asset_Host/blob/master/Movaic_Assets/Output_Example.gif">
</p>

## How it works

Movaic works by taking the source video and calculating the mean color of the individual frames using the K-mean algorithm as part of OpenCV. It then caches these values using the BGR value of the largest K-mean cluster. 

After the frames of the video are analyzed it then starts analyzing subsections of the source image using the same K-mean clustering
algorithm to find the mean of that area. 

Then using a K-D Tree the application searches for the nearest neighbor match of the image subsection and replaces that subsection with the correlating video frame. It does this for every subsection and once finished outputs the newly created image. 

## Building From Source

These instructions detail the required packages and build process for this application.
These instructions were tested on Ubuntu 18.04 "Bionic Beaver".

OpenCV libraries must be installed.

```shell
$ apt-get install libopencv-dev
```

First the progress bar submodule needs to be fetched and updated.

```shell
$ git submodule init
$ git submodule update
```

From the root directory run cmake.
```shell
$ cmake .
```

then run make using the generated cmake files.
```shell
$ make
```


### Required Modules
* CMake version 13.10
* OpenCV version 3.2.0

## Future Features
* Specify Configurations in a File
* Batch Processing
* Multiple Video Sources
* MultiThread Support
* Graphics Hardware Acceleration
