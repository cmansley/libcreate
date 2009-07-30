/*
 *
 * Code by Chris Mansley
 *
 */

#include "roombaSensor.hh"

/* Associate namespace */
namespace RoombaDriver {

  /*!
   *
   */
  RoombaSensor::RoombaSensor() : _stale(true) {
  }

  /*!
   *
   */
  void RoombaSensor::Reset() {

    /* Reset internal integration */
    _distanceTraveled = 0;
    _angleTraveled = 0;
    
    /* Data is now stale */
    _stale = true;
  }


  /*!
   *
   */
  void RoombaSensor::Update(char* raw) {

    /* Date is now fresh */
    _stale = false;

    /* Parse data and integrate */

  }

}
