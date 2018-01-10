# Scansify
A 3D computational scan tool for designing customized sensors for on-skin interaction

## Project Description

Developer Branch Status:

-- In Progress --

# Changelog
All notable changes to this project will be documented in this file.


### Timeline 2017

| Week   |      Progress      |  Notes |
|----------|-------------|------:|
| 24.dec | - pcl,Qt and kinect sdk. c++|  |
| 31.dec | - qvtk gui running  | - Lots of dependencies issues resolved  |
| 1.jan | - modern openGL <br> - surface reconstruction <br> - basic raytracing | - change from qvtk to native openGL  |
| 7.jan | - filtering/masking noises <br> - integrated config Qt GUI <br> - one euro filter for joints smoothing | - at this point, lots of gui functionality |


## Built With

* [Kinect 2] - Hardware
* [Visual Studio 2017] - IDE
* [Qt 5.9.3] - GUI application used for menu
* [VTK 7.1] - framework for rendering 3D application
* [PCL 1.8.1] - framework for computer vision/imaging processing
* [OpenGL 3] - 
* [Unit Test] - framework for testing c++ apps

## Installation

* [Setting up PCL with VS](https://stackoverflow.com/questions/16514762/setting-up-point-cloud-library-with-visual-studio)
* [Setting up Kinect SDK 2 with VS] - Link inlude and libs
* [DLL] - .dll files have to present on application folder
* [QVTK] - using cmake: build vtk with qt option -> build pcl with qvtk (http://unanancyowen.com/en/pcl-with-qt/)
* [PCL dependencies] - Recommended to use the All-In-One Installer (except for vtk, which should be built from source)

Without CMake. Make sure GlobalPath Variable is loaded for references inc/lib. Restart VS if necessary.

Configuration for Debug/Release separated.

## LICENSE

This project is licensed under Apache 2.0 

## Authors

* **Ba Thinh Tran** - *Ongoing* - [PewhProgrammer](https://github.com/PewhProgrammer)

See also the list of [contributors](https://github.com/PewhProgrammer/StreamPlaysTabu/graphs/contributors) who participated in this project.
