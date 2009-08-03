project(libcreate)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/modules)

find_package(Pthreads)
set(LIBS ${LIBS} ${PTHREADS_LIBRARY})

set(srcs src/main.cc src/roomba.cc src/roombaMonitor.cc src/roombaSensor.cc)

if (WIN32)
  set(srcs ${srcs} src/serial_win.cc src/delay_win.cc)
else (WIN32)
  set(srcs ${srcs} src/serial_lin.cc src/delay_lin.cc)
endif (WIN32)

add_executable(roomba ${srcs})
target_link_libraries(roomba ${LIBS})


