/*
 *
 * Code by Chris Mansley
 *
 */

#ifndef ROOMBA_SENSOR_HH
#define ROOMBA_SENSOR_HH

/* Implementation dependencies */
#include <string>

#include "roombaException.hh"

/* Associate namespace */
namespace RoombaDriver {

  /*!
   * \brief A class for storing the Roomba sensors
   *
   * Class Description
   */
  class RoombaSensor
  {
  public:

    /** Get distance traveled */
    double getDistance() { return _distanceTraveled; } 

    /** Get angle traveled */
    double getAngle() { return _angleTraveled; }

    /** Get left wall */
    bool leftHit() { return _leftBumper; }

    /** Get right wall */
    bool rightHit() { return _rightBumper; }

    /** Get wall hit */
    bool hitWall() { return (_rightBumper && _leftbumper); }

  private:
    /** Total distance traveled since reset */
    double _distanceTraveled;
    
    /** Total angle traveled since reset */
    double _angleTraveled;

    /** Current left wall sensor */
    bool _leftBumper;

    /** Current right wall sensor */
    bool _rightBumper;
  };

} // namespace RoombaDriver

#endif // ROOMBA_SENSOR_HH
    
