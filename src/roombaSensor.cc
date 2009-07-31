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
  RoombaSensor& RoombaSensor::operator=(const RoombaSensor& rhs) {

    /* Always check self assignment */
    if(this != &rhs) {

      /* If the incoming data is stale, do nothing */
      if(!rhs._stale) {

	/* 
	 * If the current data is stale, 
	 *   overwrite
	 * Else, 
	 *   integrate 
	 */
	if(_stale) {
	  _distanceTraveled = rhs._distanceTraveled;
	  _angleTraveled = rhs._angleTraveled;
	} else {
	  _distanceTraveled += rhs._distanceTraveled;
	  _angleTraveled += rhs._angleTraveled;
	}
	  
	/* Update state of bumpers */
	_leftBumper = rhs._leftBumper;
	_rightBumper = rhs._rightBumper;
	
	/* Copy freshness */
	_stale = rhs._stale;
      }
    }

    return *this;
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
