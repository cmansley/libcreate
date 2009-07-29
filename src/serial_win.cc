/*
 *
 * Code by Chris Mansley
 *
 */

#include "serial_win.hh"

/* Associate the namespace */
namespace RoombaDriver {

  /*!
   *
   */
  Serial::Serial() {
    _oldDCB.DCBlength = sizeof(_oldDCB);
  }

  /*!
   *
   */
  Serial::~Serial() {

  }

  /*!
   *
   */
  void Serial::Open( const std::string path ) {

    /* Open the device */
    _portH = CreateFile(path.c_str(), GENERIC_READ | GENERIC_WRITE, 
			0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  
    /* Check error status */
    if(_portH == INVALID_HANDLE_VALUE){
    
      //if(GetLastError()==ERROR_FILE_NOT_FOUND){
      //serial port does not exist. Inform user.
      //}

      throw RoombaIOException("Serial::Open: Unable to open serial port");    
    }

    /* Backup original term settings */
    if(!GetCommState(_portH, &_oldDCB)) {
      throw RoombaIOException("Serial::Open: GetCommState() failed");    
    }

    /* Backup original timeout settings */
    if(!GetCommTimeouts(_portH, &_oldTimeouts)) {
      throw RoombaIOException("Serial::Open: GetCommTimeouts() failed");    
    }

    /* Setup serial timeouts */

  }

  /*!
   *
   */
  void Serial::Flush() {
    
    /* Flush the serial port */
    if(!PurgeComm(_portH, PURGE_RXCLEAR | PURGE_TXCLEAR)) {
      throw RoombaIOException("Serial::Flush: PurgeComm() failed");
    }
    
  }

  /*!
   *
   */
  void Serial::Close() {

    /* Restore serial timeouts */
    if(!SetCommTimeouts(_portH, &_oldTimeouts)) {
      throw RoombaIOException("Serial::Close: SetCommTimeouts() failed");
    }

    /* Restore serial settings */
    if(!SetCommState(_portH, &_oldDCB)){
      throw RoombaIOException("Serial::Close: SetCommState() failed");
    }

    /* Close handle */
    if(!CloseHandle(_portH)) {
      throw RoombaIOException("Serial::Close: CloseHandle() failed");
    }

  }

  /*!
   *
   */
  int Serial::Read(const void *buffer, size_t num) {
  
    char szBuff[n + 1] = {0};
    DWORD dwBytesRead = 0;

    /* Read bytes */
    if(!ReadFile(_portH, szBuff, n, &dwBytesRead, NULL)){
      /* TESTME: Verify that a timeout does not throw error */
      throw RoombaIOException("Serial::Read: ReadFile() failed");
    }

    return (int)dwBytesRead;
  }

  /*!
   *
   */
  void Serial::Write(const void *buffer, size_t num) {

    DWORD dwBytesRead = 0;
    
    /* Write bytes */
    if(!WriteFile(_portH, buffer, num, &dwBytesRead, NULL)) {
      throw RoombaIOException("Serial::Write: WriteFile() failed");
    }
  }

  /*!
   *
   */
  void Serial::SetBaud( unsigned int baud ) {

    DCB dcb;

    /* Get current serial settings */
    if(!GetCommState(_portH, &dcb)) {
      throw RoombaIOException("Serial::SetBaud: GetCommState() failed");
    }
 
    /* Set baud rate */
    dcb.BaudRate = baud;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity = NOPARITY;
  
    /* Set the serial settings */
    if(!SetCommState(_portH, &dcb)){
      throw RoombaIOException("Serial::SetBaud: SetCommState() failed");
    }


    /* Flush old data */
    Flush();    

  }

  /*!
   *
   */
  void Serial::_setTimeout() {
    
    COMMTIMEOUTS timeouts;
    
    /* Setup timeouts struct */
    timeouts.ReadIntervalTimeout = 0; // Delay between the arrival of bytes is irrelevant

    timeouts.ReadTotalTimeoutMultiplier = 0; // Timeout is not dependent on number of bytes
    timeouts.ReadTotalTimeoutConstant = 200; // Wait 200ms, then timeout, regardless of whether all the bytes have been read

    timeouts.WriteTotalTimeoutMultiplier = 0; // Do not timeout on writes
    timeouts.WriteTotalTimeoutConstant = 0; // Do not timeout on writes

    /* Set serial timeout */
    if(!SetCommTimeouts(_portH, &timeouts)){
      throw RoombaIOException("Serial::_setTimeout: SetCommTimeout() failed");
    }
  }


  /*!
   *
   */
  void _getError() {
    char lastError[1024];
  
    FormatMessage(
		  FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		  NULL,
		  GetLastError(),
		  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		  lastError,
		  1024,
		  NULL);
  }

}
