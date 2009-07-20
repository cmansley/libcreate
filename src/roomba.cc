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
  void Roomba::Initialize( ) throw( RoombaIOException ) {
  
    try {
      /* Initialize serial connection */
      _setupConnection();
      
      /* Send start command*/
      _setRoombaStart();
      
      /* Enter safe mode */
      _setRoombaPassiveToSafeMode();
      
      /* Enter full mode */
      _setRoombaFullMode();
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
  void Roomba::Uninitialize() throw( RoombaIOException ) {

    try {

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
  void Roomba::_setupConnection() throw( RoombaIOException ) { 
    
    try {

      /* Open the device */
      _serial.Open(_roombaPath);

      /* Set serial baud rate */
      _serial.SetBaud(Serial::BAUD_56K);
      
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
  void Roomba::_teardownConnection() throw( RoombaIOException ) {

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
  void Roomba::_flushSerialBuffer() throw( RoombaIOException ) {
    
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
  void Roomba::_setRoombaStart() throw( RoombaIOException ) {

    /* Start byte*/
    unsigned char message = 128; 

    /* Write message */
    _serial.Write(&message, 1);

    /* Recommended sleep time */
    // _sleep(20)
    usleep(20000);
  }

  /*!
   *
   */
  void Roomba::_setRoombaFullMode() throw( RoombaIOException ) {

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
  void Roomba::_setRoombaPassiveToSafeMode() throw( RoombaIOException ) {

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
  void Roomba::Drive(signed short velocity, signed short radius) throw( RoombaIOException ) {
    
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
