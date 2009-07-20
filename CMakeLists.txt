project(create)
cmake_minimum_required(VERSION 2.6)

set(srcs src/main.cc src/roomba.cc src/roombaMonitor.cc src/serial_lin.cc)
add_executable(roomba ${srcs})
