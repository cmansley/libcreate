#include <iostream>

#include "roomba.hh"
#include "roombaException.hh"

/* Associate the namespace */
namespace RoombaDriver {

  /*!
   *
   */
  Roomba::Roomba(const std::string path): _roombaPath(path),					  
					  _roombaInitialized(false)
  {
  }

  /*!
   *
   */
  Roomba::~Roomba() {
    
    try {
      _teardownConnection();
    }

    catch(...) {
      std::cerr << "Roomba::~Roomba: Unknown exception" << std::endl;
    }
  }

  /*!
   *
   *
   */
  void Roomba::Initialize( ) {
  
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

      /* Start sensor stream */
      _startRoombaStream();
    }
    
    catch(...) {
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
      // throw; // throw ????
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
      _serial.Open(_roombaPath);

      /* Set serial baud rate */
      _serial.SetBaud(Serial::RBAUD_56K);
      
    } // try

    /* Handle I/O exceptions */
    catch(RoombaIOException &e) {
      std::cerr << e.what() << std::endl;
      throw;
    }

    /* Handle unknown exceptions */
    catch(...) {
      std::cerr << "Roomba::_setupConnection: Unknown exception" << std::endl;
      // throw; // throw ???
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
    _serial.Close();
  }

  /*!
   *
   */
  void Roomba::_flushSerialBuffer() {
    
    try {
      
      /* Flush buffer */
      _serial.Flush();

    } // try
    
    /* Catch I/O exceptions */
    catch(RoombaIOException &e) {
      std::cerr << e.what() << std::endl;
      throw;
    }

    /* Catch unknown exceptions */
    catch(...) {
      std::cerr << "Roomba::_setSerialBaud: Unknown Exception" << std::endl;
      // throw; // throw ????
    }
  }


  /*!
   *
   */
  void Roomba::_setRoombaStart() {

    /* Start byte*/
    unsigned char message = 128; 

    /* Write message */
    _serial.Write(&message, 1);

    /* Recommended sleep time */
    // _sleep(20)
    //usleep(20000);
  }

  /*!
   *
   */
  void Roomba::_setRoombaFullMode() {

    /* Start byte*/
    unsigned char message = 132; 
    
    /* Write message */
    _serial.Write(&message, 1);

    /* Recommended sleep time */
    // _sleep(20);
    //usleep(20000);
  }

  /*!
   *
   */
  void Roomba::_setRoombaPassiveToSafeMode() {

    /* Start byte*/
    unsigned char message = 130; 
    
    /* Write message */
    _serial.Write(&message, 1);

    /* Recommended sleep time */
    //usleep(20000);
  }

  /*!
   *
   */
  void Roomba::_startRoombaStream() {

    /* Message bytes*/
    unsigned char message[5]; 

    message[0] = 148;
    message[1] = 2;
    message[2] = 19; // distance
    message[3] = 20; // angle
    message[4] = 7;  // bump sensor

    /* Write message */
    _serial.Write(&message, 5);

    /* Recommended sleep time */
    // _sleep(20);
    //usleep(20000);
    
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
    _serial.Write(&message, 2);

    /* Recommended sleep time */
    // _sleep(20);
    //usleep(20000);
  }

  /*!
   *
   */
  void Roomba::Drive( signed short velocity,  signed short radius) {
    
    /* Start byte*/
    unsigned char message[5]; 

    message[0] = 137;
    message[1] = (velocity >> 8) & 255;
    message[2] = velocity & 255;
    message[3] = (radius >> 8) & 255;
    message[4] = radius & 255;

    /* Write message */
    _serial.Write(&message, 5);

    /* Recommended sleep time */
    // _sleep(20);
    //usleep(20000);
  }
} // namespace Roomba
