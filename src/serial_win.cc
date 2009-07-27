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
  void Serial::Read(const void *buffer, size_t num) {
  
    char szBuff[n + 1] = {0};
    DWORD dwBytesRead = 0;

    /* Read bytes */
    if(!ReadFile(_portH, szBuff, n, &dwBytesRead, NULL)){
      throw RoombaIOException("Serial::Read: ReadFile() failed");
    }

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
    dcb.BaudRate=baud;
    dcb.ByteSize=8;
    dcb.StopBits=ONESTOPBIT;
    dcb.Parity=NOPARITY;
  
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
