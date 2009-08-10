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
#if defined (WIN32)
  roomba->Initialize("COM1");
#else
  roomba->Initialize("/dev/ttyUSB0");
#endif
    

  /*
   * Run test program
   */
  roomba->Drive(2);
  roomba->Rotate(90);
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
