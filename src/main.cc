/*
 * Chris Mansley
 */

#include <iostream>

#include "roomba.hh"

using namespace std;
using namespace RoombaDriver;


int main(int argc, char* argv[])
{
  /*
   * Instantiate an instance
   */
  Roomba roomba("/dev/ttyUSB0");

  /*
   * Initialize the roomba
   */
  try {
    roomba.Initialize();
  }

  catch(...) {
    cerr << "You failed" <<endl;
    return -1;
  }


  // do stuff with roomba
  roomba.Drive(-300,32768);

  sleep(2);

  /*
   * Uninitialize the device
   */
  try {
    roomba.Uninitialize();
  }

  catch(...) {
    cerr << "Teardown failed" << endl;
    return -1;
  }

  /* Success! */
  return 0;
}
