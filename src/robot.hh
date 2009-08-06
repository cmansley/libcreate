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
  
  /** Initialize the robot */
  virtual void Initialize( const std::string path ) = 0;
  
  /** Uninitialize the robot */
  virtual void Uninitialize( ) = 0;
  
  /** Drive the specified distance */
  virtual void Drive(float distance) = 0;
  
  /** Rotate by the specified angle */
  virtual void Rotate(float degrees) = 0;
};

extern "C" DLL_EXPORT Robot* CreateRobot();
extern "C" DLL_EXPORT void DestroyRobot(Robot* robot);

#endif // ROBOT_HH
