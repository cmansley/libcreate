project(libcreate)
cmake_minimum_required(VERSION 2.6)

find_package(Threads)
set(LIBS ${LIBS} ${CMAKE_THREAD_LIBS_INIT})

set(srcs src/main.cc src/roomba.cc src/roombaMonitor.cc)

if (WIN32)
  set(srcs ${srcs} src/serial_win.cc)
else (WIN32)
  set(srcs ${srcs} src/serial_lin.cc)
endif (WIN32)

add_executable(roomba ${srcs})
target_link_libraries(roomba ${LIBS})


