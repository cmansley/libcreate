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

    /* Parse data and integrate */
    _parsePacket(raw);

    /*for(int i=0; i<raw[1]+2; i++) {
      printf("%d ",raw[i]);
    }
    printf("\n");*/

    /* Date is now fresh */
    _stale = false;

  }

  void RoombaSensor::_parsePacket(char *raw) {

    signed short distance;
    signed short angle;
    unsigned char bools;

    int index = 2;
    int total_bytes = 0;

    if(raw[0] != 19) {
      // fail
      return;
    }

    /* Grab number of bytes following start */
    total_bytes = raw[1];
    
    while(total_bytes > 0) {
        
      switch(raw[index]) {
      case 19:
	distance = raw[index+1]<<8 | raw[index+2];
	total_bytes -= 3;
	index += 3;
      case 20:
	angle = raw[index+1]<<8 | raw[index+2];
	total_bytes -= 3;
	index += 3;
      case 7:
	bools = raw[index+1];
	total_bytes -= 2;
	index += 2;
      }
    }

    /* Add estimate to current */
    if(!isStale()) {
      _distanceTraveled += distance;
      _angleTraveled += angle;
    } else {
      _distanceTraveled = distance;
      _angleTraveled = angle;
    }

    _leftBumper = bools & 0x2;
    _rightBumper = bools & 0x1;
    
  }

} // namespace RoombaDriver
