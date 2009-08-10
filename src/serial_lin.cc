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
    
    /* Flush the serial port */
    if (tcflush(_portFD, TCIOFLUSH) != 0) {
      throw RoombaIOException("Serial::Flush: tcflush() failed");
    }

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
  int Serial::Read(void *buffer, size_t num) {

    struct timeval timeout;
    fd_set fids;
    int num_active;
    int numBytes = 0;

    /* Initialize and set the file descriptor set for select */
    FD_ZERO(&fids);
    FD_SET(_portFD, &fids);

    /* Setup the timeout structure */
    memset(&timeout, 0, sizeof(timeout));   
    timeout.tv_usec = 200000; // 200ms

    /* Wait for data or timeout*/
    num_active = select(_portFD+1, &fids, 0, 0, &timeout);

    /* Check for some active fids */
    if(num_active > 0) {

      /* Read bytes */
      numBytes = read(_portFD, buffer, num);
    }

    return numBytes;

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
    
    /* Set the serial settings */
    if(tcsetattr(_portFD, TCSAFLUSH, &term) < 0 ) {
      throw RoombaIOException("Serial::SetBaud: tcsetattr() failed");
    }

  }

} // namespace RoombaDriver
