/*
 * Chris Mansley
 */

#include <iostream>

#include "robot.hh"

using namespace std;

int main(int argc, char* argv[])
{
  /*
   * Instantiate an instance
   */
  Robot* roomba = CreateRobot();

  /*
   * Initialize the roomba
   */
  roomba->Initialize("/dev/ttyUSB0");

  /*
   * Run test program
   */
  roomba->Drive(500);
  roomba->Rotate(90);
  roomba->Drive(300);
  roomba->Rotate(-90);

  /*
   * Uninitialize the device
   */
  roomba->Uninitialize();

  /*
   * Free up memory 
   */
  DestroyRobot(roomba);
  
  /* Success! */
  return 0;
}
