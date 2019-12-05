# Movaic

Movaic (Movie Mosaic) is a command-line application that can be used to create picture mosaics using a source image and a video clip.
 
<p align="center">
  <img width="535" height="800" src="https://github.com/GoldenBug/ReadMe_Asset_Host/blob/master/Movaic_Assets/Output_Example.gif">
</p>

## How it works

Movaic works by taking the source video and calculating the mean color of the individual frames using the K-mean algorithm as part of OpenCV. It then caches these values using the BGR value of the largest K-mean cluster. 

After the frames of the video are analyzed it then starts analyzing subsections of the source image using the same K-mean clustering
algorithm to find the mean of that area. 

Then using a K-D Tree the application searches for the nearest neighbor match of the image subsection and replaces that subsection with the correlating video frame. It does this for every subsection and once finished outputs the newly created image. 

## Using the application

Using the application from the command line requires the 3 (mandatory) arguments below.

```
Options:
  -h [ --help ]         Print help messages
  -p [ --poster ] arg   Path of the input poster that will be used as reference
                        to create the output
  -v [ --video ] arg    Path of movie that will be used as a source of 
                        replacement frames
  -o [ --output ] arg   Path and filename of the output poster
```

## Building From Source

These instructions detail the required packages and build process for this application.
These instructions were tested on Ubuntu 18.04 "Bionic Beaver".

### Required Dependencies (External)
* CMake version 13.10
* OpenCV version 3.2.0

### Downloaded Dependencies (Internal)
These dependencies are required but should be downloaded automatically as git submodules linked by cmake.

* HealthBar
* Boost (C++)

### Build Commands

The progress bar submodule and boost-fetching submodule needs to be fetched and updated.

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

## Future Features
* Specify Configurations in a File
* Batch Processing
* Multiple Video Sources
* MultiThread Support
* Graphics Hardware Acceleration
