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

    /** Constructor */
    RoombaSensor();

    /** Copy assignment operator */
    RoombaSensor& operator=(const RoombaSensor& rhs);

    /** Get distance traveled */
    double GetDistance() { return _distanceTraveled; } 

    /** Get angle traveled */
    double GetAngle() { return _angleTraveled; }

    /** Get left wall */
    bool LeftHit() { return _leftBumper; }

    /** Get right wall */
    bool RightHit() { return _rightBumper; }

    /** Get wall hit */
    bool HitWall() { return (_rightBumper && _leftBumper); }

    /** Reset current data */
    void Reset();

    /** Update internal model of roomba */
    void Update(char *raw);

    /** Return freshness of data */
    bool isStale() { return _stale; }

  private:
    /** Total distance traveled since reset */
    double _distanceTraveled;
    
    /** Total angle traveled since reset */
    double _angleTraveled;

    /** Current left wall sensor */
    bool _leftBumper;

    /** Current right wall sensor */
    bool _rightBumper;

    /** Has update been called since reset */
    bool _stale;
  };

} // namespace RoombaDriver

#endif // ROOMBA_SENSOR_HH
    
