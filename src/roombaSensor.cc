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

    // thread safety?

    /* Reset internal integration */
    _distanceTraveled = 0;
    _angleTraveled = 0;
    
    /* Data is now stale */
    _stale = true;

    // thread safety?
  }


  /*!
   *
   */
  void RoombaSensor::Update(char* raw) {

    // thread safety

    /* Date is now fresh */
    _stale = false;

    // thread saftey
  }

}
