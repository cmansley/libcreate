project(create)
cmake_minimum_required(VERSION 2.6)

# add_subdirectory(src)
set(srcs src/main.cc src/roomba.cc src/roombaMonitor.cc)
add_executable(roomba ${srcs})
