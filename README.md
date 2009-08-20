Overview
--------

This package attempts to be a cross platform, easy to use, basic
functionality software library for interfacing with iRobot's Create
platform. While other packages may provide more functionality, this
package has native implementations on both Windows and Linux for
commanding and sensing from the Create platform and a fairly modular
C++ coding style.

Requirements
------------

* cmake 
* pthreads

Windows Build
-------------

Download pthreads for windows from [here][1]. This project was built
with the "prebuilt" release 2.8.0. Make sure you have cmake for
windows installed from [here][2]. Download and unzip libcreate. Start
the GUI for cmake, click on browse source and choose the path of the
libcreate source. Next, add the paths of pthreads by clicking on "Add
Entry" and entering `CMAKE_INCLUDE_PATH` and choosing the path to the
pthread headers. Repeat to add `CMAKE_LIBRARY_PATH` and choose the
path to the pthread libraries. Run configure (you may have to run this
twice), then run generate. Visual Studio solution files should have
been built (or whatever the native compiler choosen was). Make sure to
build the final copy with release mode.

[1]: http://sourceware.org/pthreads-win32/
[2]: http://www.cmake.org/cmake/resources/software.html

Linux Build
-----------

Download the source code and unzip it to a directory. In that
directory, make a "build" folder. In the build folder, run `cmake`.

    tar xvzf libcreate.tar.gz
    cd libcreate
    mkdir build
    cd build
    cmake ..
    make

This will build the test code and the library. 
