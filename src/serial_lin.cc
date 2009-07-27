/*
 *
 * Code by Chris Mansley
 *
 */

#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <cstring>

#include "serial_lin.hh"

/* Associate the namespace */
namespace RoombaDriver {

  /*!
   *
   */
  Serial::Serial() {
    memset(&_oldTerm,0,sizeof(struct termios));
  }

  /*!
   *
   */
  Serial::~Serial() {

  }

  /*!
   *
   */
  void Serial::Open( std::string path ) throw( RoombaIOException ) {

    /* Open the device */
    if((_portFD = open(path.c_str(), O_RDWR | O_NOCTTY)) < 0) {
      throw RoombaIOException("Serial::Open: Unable to open serial port");
    }

    /* Backup original term settings */
    if(tcgetattr(_portFD, &_oldTerm) < 0) {
      throw RoombaIOException("Serial::Open: tcgetattr() failed");
    }
  }

  /*!
   *
   */
  void Serial::Flush() throw( RoombaIOException ) {
    
    // thread safety
  
    /* Flush the serial port */
    if (tcflush(_portFD, TCIOFLUSH) != 0) {
      throw RoombaIOException("Serial::Flush: tcflush() failed");
    }
  
    // thread safety 
  }

  /*!
   *
   */
  void Serial::Close() throw( RoombaIOException ) {

    /* Restore serial settings */
    if (tcsetattr(_portFD, TCSANOW, &_oldTerm) < 0) {
      throw RoombaIOException("Serial::Close: tcsetattr() failed");      
    }

    /* Close file descriptor */
    if(close(_portFD) != 0) {
      throw RoombaIOException("Serial::Close: close() failed");
    }

  }

  /*!
   *
   */
  void Serial::Read(void *buffer, size_t num) throw( RoombaIOException ) {

    /* Read bytes */
    if (read(_portFD, buffer, num) != num) {      
      throw RoombaIOException("Serial::Read: read() failed");
    }
  }

  /*!
   *
   */
  void Serial::Write(const void *buffer, size_t num) throw( RoombaIOException ) {

    /* Write bytes */
    if (write(_portFD, buffer, num) != num) {      
      throw RoombaIOException("Serial::Write: write() failed");
    }
  }

  /*!
   *
   */
  void Serial::SetBaud( unsigned int baud ) throw( RoombaIOException ) {

    struct termios term;
    
    /* Get current serial settings */
    if(tcgetattr(_portFD, &term) < 0) {
      throw RoombaIOException("Serial::SetBaud: tcgetattr() failed");
    }
    
    /* Set baud rate */
    cfmakeraw(&term);
    cfsetispeed(&term,baud);
    cfsetospeed(&term,baud);
    
    // thread safety 
    
    /* Set the serial settings */
    if(tcsetattr(_portFD, TCSAFLUSH, &term) < 0 ) {
      throw RoombaIOException("Serial::SetBaud: tcsetattr() failed");
    }

    // thread safety

    /* Flush old data */
    Flush();    

  }

} // namespace RoombaDriver
