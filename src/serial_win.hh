/*
 *
 * Code by Chris Mansley
 *
 */

#ifndef SERIAL_LIN_HH
#define SERIAL_LIN_HH

/* Implementation dependencies */
#include <windows.h>
#include <string>

#include "roombaException.hh"

/* Associate namespace */
namespace RoombaDriver {

  /*!
   * \brief A class for interfacing with the serial port
   *
   * Class Description
   */
  class Serial
  {
  public:
 
    /** Constructor */
    Serial( );

    /** Destructor */
    ~Serial( );

    /** Open serial communications */
    void Open( const std::string path ) throw( RoombaIOException );

    /** Sets serial baud rate */
    void SetBaud( unsigned int baud ) throw( RoombaIOException );

    /** Write serial port */
    void Write(const void *buffer, unsigned int num) throw( RoombaIOException );

    /** Read serial port */
    void Read(const void *buffer, unsigned int num) throw( RoombaIOException );

    /** Flush (remove) data from buffers */
    void Flush() throw( RoombaIOException );

    /** Close serial communication */
    void Close() throw( RoombaIOException );
  
    /** Constants for generic baud rates */
    static const unsigned int BAUD_56K = 57600;
    static const unsigned int BAUD_19K = 19200;

  protected:

  private:

    /** Device handle */
    HANDLE _portH;

    /** Original serial settings */
    DCB _oldDCB;
  };

} // namespace RoombaDriver

#endif // SERIAL_LIN_HH

