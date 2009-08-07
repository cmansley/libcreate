/*
 *
 * Code by Chris Mansley
 *
 */

#ifndef ROBOT_HH
#define ROBOT_HH

/* Definition dependencies */
#include <string>

#include "dll.hh"

/*!
 * \brief A interface class for interfacing with robots
 *
 * Class Description
 */
class Robot
{
 public:

  /** Constructor */
  Robot() { }
  
  /** Destructor */
  virtual ~Robot() { }
  
  /** 
   * Initialize and setup the robot 
   *
   * @param path Path to communications port, for example on windows
   * this would be "COM1", or on linux "/dev/ttyUSB0"
   */
  virtual void Initialize( const std::string path ) = 0;
  
  /** 
   * Uninitialize the robot
   */
  virtual void Uninitialize( ) = 0;
  
  /** 
   * Drive the specified distance.
   *
   * Note, this function will block until the distance has been
   * successfully travelled
   *
   * @param distance Distance to travel in feet (English Units!)
   */
  virtual void Drive(float distance) = 0;
  
  /** 
   * Rotate by the specified angle 
   *
   * Note, this function will block until the distance has been
   * successfully travelled
   *
   * @param degrees Number of degrees to rotate, positive represents
   * clockwise, negative represents anti-clockwise
   */
  virtual void Rotate(float degrees) = 0;
};

/**
 * Creates a pointer to the derived class
 *
 * Note, this "factory" was designed to make exporting of C++ classes
 * in DLLs easier for portability reasons
 */
extern "C" DLL_EXPORT Robot* CreateRobot();

/**
 * Destroys pointer/memory created with CreateRobot()
 *
 * Note, this "factory" was designed to make exporting of C++ classes
 * in DLLs easier for portability reasons
 *
 * @param robot Pointer to a Robot object created with CreateRobot()
 */
extern "C" DLL_EXPORT void DestroyRobot(Robot* robot);

#endif // ROBOT_HH
