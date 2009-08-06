
#include "robot.hh"
#include "roomba.hh"

using namespace RoombaDriver;

/*
 *
 */
Robot* CreateRobot() {
  return (new Roomba());
}

/*
 *
 */ 
void DestroyRobot(Robot* robot) {
  delete robot;
}
