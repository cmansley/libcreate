# Libcreate project
project(libcreate)
cmake_minimum_required(VERSION 2.6)

# Add custom module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/modules)

# Find and setup pthreads
find_package(Pthreads)
set(LIBS ${LIBS} ${PTHREADS_LIBRARY})
include_directories(${PTHREADS_INCLUDE_DIR})

# Include the sources 
set(srcs src/roomba.cc src/roombaMonitor.cc src/roombaSensor.cc src/robot.cc)

# conditionally include windows sources
if (WIN32)
  set(srcs ${srcs} src/serial_win.cc src/delay_win.cc)
else (WIN32)
  set(srcs ${srcs} src/serial_lin.cc src/delay_lin.cc)
endif (WIN32)

# Build and link library
add_library(roomba SHARED ${srcs})
target_link_libraries(roomba ${LIBS})

# Create test executable
add_executable(runMe src/main.cc)
target_link_libraries(runMe roomba)

