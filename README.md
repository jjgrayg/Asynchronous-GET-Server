# Asynchronous-GET-Server
<br>

## Overview
This is a basic asynchronous server utilizing Boost's ASIO to respond to GET requests from a browser.
The code may be compiled using the following command:
	`clang++ main.cpp server.cpp -o server`
The code may also be openned as a Visual Studio project and built from there. <br><br>

The server will respond with either a 200 response, in which case the response contains the attached file, or a 404 response containing a default 404 error page meaning the requested file cannot be found.
Every request is logged in a local *log.txt* file that is in the same location as your executable. 
**New:** New version comes with a basic GUI utilizing Qt tools.


## Required Library
This program requires the Boost library at version 1.78.0. If you're not using Visual Studio this should be as simple as having the *boost_1_78_0* folder in the same directory as the .cpp and .hpp files and changing the Boost include lines in connection.hpp to include the *boost_1_78_0/* prefix. For example `#include <boost/asio.hpp>` becomes `#include <boost_1_78_0/boost/asio.hpp>`, or to make it easier you could rename *boost_1_78_0* to *boost* and `#include <boost/asio.hpp>` instead becomes `#include <boost/boost/asio.hpp>`.<br><br> 

If you decide to use Visual Studio you must edit the VS_SETUP.bat file and change the path after BOOST178_DIR to the path to your Boost 1.78.0 installation. As of V2.0 there is an additional version requiring the Qt environment to compile. In order to get Boost to properly link I'd suggest using the *vcpkg* tool or other similar tool to install Boost on your system.


## Project structure
All files that can be accessed by the server *must* be kept in a folder called "html" in the same directory as the server executable. The folder structure should look like:

    server/
    ├── server.exe
    └── html/
        ├── index.html
        └── *other files*
	
This repository comes with a folder containing an executable and sample website (my current portfolio site) for those who just want to see it in action.
