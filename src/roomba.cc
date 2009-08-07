
/* Implementation dependencies */
#include <iostream>
#include <cmath>

#include "roomba.hh"
#include "roombaException.hh"
#include "delay.hh"

/* Associate the namespace */
namespace RoombaDriver {

  /*!
   *
   */
  Roomba::Roomba(): _roombaInitialized(false)
  {
    /* Create serial */
    _serial = new Serial();
    
    /* Create monitor */
    _monitor = new RoombaMonitor();

    /* Create sensor */
    _sensor = new RoombaSensor();
  }

  /*!
   *
   */
  Roomba::~Roomba() {
    
    try {
      /* Stop serial connection gracefully */
      _teardownConnection();

      /* Free up dynamic memory*/
      delete _serial;
      delete _monitor;
      delete _sensor;
    }

    catch(...) {
      std::cerr << "Roomba::~Roomba: Unknown exception" << std::endl;
    }
  }

  /*!
   *
   *
   */
  void Roomba::Initialize( const std::string path ) {

    /* Copy over serial path */
    _roombaPath = path;
  
    try {
      /* Initialize serial connection */
      _setupConnection();
      
      /* Send start command*/
      _setRoombaStart();
      
      /* Enter safe mode */
      _setRoombaPassiveToSafeMode();
      
      /* Enter full mode */
      _setRoombaFullMode();

      /* Spawn roomba monitor thread */
      _monitor->StartMonitor(_serial);

      /* Start sensor stream */
      _startRoombaStream();

    } //try
    
    catch(...) {
      std::cerr << "Roomba::Initialize: Unknown exception" << std::endl;
      throw;
    }
      
    /* Roomba is now initialized */
    _roombaInitialized = true;
  }

  /*!
   *
   *
   */
  void Roomba::Uninitialize() {

    try {

      /* Stop sensor stream */
      _stopRoombaStream();

      /* Stop roomba monitor thread */
      _monitor->StopMonitor();

      /* Enter passive mode */
      _setRoombaStart();

      /* Flush remaining */
      _flushSerialBuffer();

      /* Attempt to deactivate connection */ 
      _teardownConnection();

    } // try
      
    /* Handle I/O exception */
    catch(RoombaIOException &e) {
      std::cerr << e.what() << std::endl;
      throw;
    }

    /* Handle unknown exceptions */
    catch(...) {
      std::cerr << "Roomba::Uninitialize: Unknown exception" << std::endl;
      throw;
    }

    /* Roomba unintialized */
    _roombaInitialized = false;
  }


  /*!
   *
   */
  void Roomba::_setupConnection() { 
    
    try {

      /* Open the device */
      _serial->Open(_roombaPath);

      /* Set serial baud rate */
      _serial->SetBaud(Serial::RBAUD_56K);

      /* Flush any existing data */
      _flushSerialBuffer();
      
    } // try

    /* Handle I/O exceptions */
    catch(RoombaIOException &e) {
      std::cerr << e.what() << std::endl;
      throw;
    }

    /* Handle unknown exceptions */
    catch(...) {
      std::cerr << "Roomba::_setupConnection: Unknown exception" << std::endl;
      throw;
    }

  }

  /*!
   *
   *
   */
  void Roomba::_teardownConnection() {

    /* Check comunication status */
    if (_roombaInitialized == false) {
      return;
    }

    /* Close serial port */
    _serial->Close();
  }

  /*!
   *
   */
  void Roomba::_flushSerialBuffer() {
    
    try {
      
      /* Flush buffer */
      _serial->Flush();

    } // try
    
    /* Catch I/O exceptions */
    catch(RoombaIOException &e) {
      std::cerr << e.what() << std::endl;
      throw;
    }

    /* Catch unknown exceptions */
    catch(...) {
      std::cerr << "Roomba::_setSerialBaud: Unknown Exception" << std::endl;
      throw;
    }
  }


  /*!
   *
   */
  void Roomba::_setRoombaStart() {

    /* Start byte*/
    unsigned char message = 128; 

    /* Write message */
    _serial->Write(&message, 1);

    /* Recommended sleep time */
    delay(20);
  }

  /*!
   *
   */
  void Roomba::_setRoombaFullMode() {

    /* Start byte*/
    unsigned char message = 132; 
    
    /* Write message */
    _serial->Write(&message, 1);

    /* Recommended sleep time */
    delay(20);
  }

  /*!
   *
   */
  void Roomba::_setRoombaPassiveToSafeMode() {

    /* Start byte*/
    unsigned char message = 130; 
    
    /* Write message */
    _serial->Write(&message, 1);

    /* Recommended sleep time */
    delay(20);
  }

  /*!
   *
   */
  void Roomba::_startRoombaStream() {

    /* Message bytes*/
    unsigned char message[5]; 

    message[0] = 148;
    message[1] = 3;
    message[2] = 19; // distance
    message[3] = 20; // angle
    message[4] = 7;  // bump sensor

    /* Write message */
    _serial->Write(&message, 5);

    /* Recommended sleep time */
    delay(20);
    
  }

  /*!
   *
   */
  void Roomba::_stopRoombaStream() {

    /* Message bytes*/
    unsigned char message[2]; 

    message[0] = 150;
    message[1] = 0;

    /* Write message */
    _serial->Write(&message, 2);

    /* Recommended sleep time */
    delay(20);
  }

  /*!
   *
   */
  void Roomba::RDrive( signed short velocity,  signed short radius) {
    
    /* Start byte*/
    unsigned char message[5]; 

    message[0] = 137;
    message[1] = (velocity >> 8) & 255;
    message[2] = velocity & 255;
    message[3] = (radius >> 8) & 255;
    message[4] = radius & 255;

    /* Write message */
    _serial->Write(&message, 5);

    /* Recommended sleep time */
    delay(20);
  }

  /*!
   *
   */
  void Roomba::Drive(float distance) {

    /* English units :( */
    /* 1 foot = 304.8 mm */
    distance = distance * 304.8;

    /* Reset internal model */
    _sensor->Reset();

    /* Drive forward at 200 mm/s */
    RDrive(200, _STRAIGHT);

    /* Block waiting for roomba to complete task */
    for(;;) {

      /* FIXME: Add some sort of timeout for broken forward travel */

      /* Grab data from monitor */
      _monitor->GrabCurrentSensor(_sensor);

      /* Check for fresh data */
      if(!_sensor->isStale()) {
	
	/* If the angle is greater than the desired break */
	if(_sensor->GetDistance() > distance) {
	  break;
	}	
      }

      /* Wait it out */
      delay(20);
    }

    /* Stop moving */
    RDrive(0, _STRAIGHT);
  }

  /*!
   *
   */
  void Roomba::Rotate(float degrees) {

    /* Reset internal model */
    _sensor->Reset();

    /*  
     * Note: Apparently any speeds at or less than 100 cause ill
     *  effects in angle measurement.
     */
       
    /* Rotate at 150 mm/s */
    if(degrees > 0) {
      RDrive(150, _CW);
    } else {
      RDrive(150, _ANTICW);
    }

    /* Movement is correct, just check magnitude */
    degrees = fabs(degrees);

    /* Block waiting for roomba to complete task */
    for(;;) {

      /* FIXME: Add some sort of timeout for broken rotations */

      /* Grab data from monitor */
      _monitor->GrabCurrentSensor(_sensor);

      /* Check for fresh data */
      if(!_sensor->isStale()) {
	
	/* If the angle is greater than the desired break */
	if(fabs(_sensor->GetAngle()) > degrees) {
	  break;
	}	
      }

      /* Wait it out */
      delay(20);
    }

    /* Stop moving */
    RDrive(0, _STRAIGHT);
  }
} // namespace Roomba
