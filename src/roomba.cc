#include <iostream>
#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <cstring>

#include "roomba.hh"
#include "roombaException.hh"

/* Associate the namespace */
namespace RoombaDriver {

  /*!
   *
   */
  Roomba::Roomba(const std::string path): _roombaPath(path),
					  _roombaFD(0),
					  _roombaInitialized(false)
					  
  {
    memset(&_oldTerm,0,sizeof(struct termios));
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
    
    }

    /* Handle unknown exceptions */
    catch(...) {
      std::cerr << "Roomba::_setupConnection: Unknown exception" << std::endl;
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
      if((_roombaFD = open(_roombaPath.c_str(), O_RDWR | O_NOCTTY)) < 0) {
	throw RoombaIOException("Roomba::_setupConnection: Unable to open serial port");
      }

      /* Backup original term settings */
      if(tcgetattr(_roombaFD, &_oldTerm) < 0) {
	throw RoombaIOException("Roomba::_setupConnection: tcgetattr() failed");
      }

      /* Set serial baud rate */
      _setSerialBaud();
      
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
    
    /* Restore serial settings */
    if (tcsetattr(_roombaFD, TCSANOW, &_oldTerm) < 0) {
      throw RoombaIOException("Roomba::_teardownConnection: tcsetattr() failed");      
    }

    /* Close file descriptor */
    if(close(_roombaFD) != 0) {
      throw RoombaIOException("Roomba::_teardownConnection: close() failed");
    }

  }

  /*!
   *
   */
  void Roomba::_setSerialBaud() throw( RoombaIOException ) {

    struct termios term;
    
    try {
      /* Get current serial settings */
      if(tcgetattr(_roombaFD, &term) < 0) {
	throw RoombaIOException("Roomba::_setSerialBaud: tcgetattr() failed");
      }
      
      /* Set baud rate */
      cfmakeraw(&term);
      cfsetispeed(&term,B57600);
      cfsetospeed(&term,B57600);
      
      // thread safety 
      
      /* Set the serial settings */
      if(tcsetattr(_roombaFD, TCSAFLUSH, &term) < 0 ) {
	throw RoombaIOException("Roomba::_setSerialBaud: tcsetattr() failed");
      }

      // thread safety

      _flushSerialBuffer();
      
    } // try
    
    
    /* Catch I/O exception */
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
  void Roomba::_flushSerialBuffer() throw( RoombaIOException ) {
    
    try {

      // thread safety

      /* Flush the serial port */
      if (tcflush(_roombaFD, TCIOFLUSH) != 0) {
	throw RoombaIOException("Roomba::_flushSerialBuffer: tcflush() failed");
      }

      // thread safety 
      
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
    
    if ((unsigned int)write(_roombaFD, &message, 1) != 1) {      
      throw RoombaIOException("Roomba::_setRoombaStart: write() failed");
    }

    /* Recommended sleep time */
    usleep(20000);
  }

  /*!
   *
   */
  void Roomba::_setRoombaFullMode() throw( RoombaIOException ) {

    /* Start byte*/
    unsigned char message = 132; 
    
    if ((unsigned int)write(_roombaFD, &message, 1) != 1) {      
      throw RoombaIOException("Roomba::_setRoombaStart: write() failed");
    }

    /* Recommended sleep time */
    usleep(20000);
  }

  /*!
   *
   */
  void Roomba::_setRoombaPassiveToSafeMode() throw( RoombaIOException ) {

    /* Start byte*/
    unsigned char message = 130; 
    
    if ((unsigned int)write(_roombaFD, &message, 1) != 1) {      
      throw RoombaIOException("Roomba::_setRoombaStart: write() failed");
    }

    /* Recommended sleep time */
    usleep(20000);
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

    if ((unsigned int)write(_roombaFD, &message, 5) != 5) {      
      throw RoombaIOException("Roomba::_setRoombaStart: write() failed");
    }

    /* Recommended sleep time */
    usleep(20000);
  }
} // namespace Roomba
