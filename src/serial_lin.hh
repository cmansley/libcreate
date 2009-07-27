/*
 *
 * Code by Chris Mansley
 *
 */

#ifndef SERIAL_LIN_HH
#define SERIAL_LIN_HH

/* Implementation dependencies */
#include <string>
#include <termios.h>

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
    void Read(void *buffer, unsigned int num) throw( RoombaIOException );

    /** Flush (remove) data from buffers */
    void Flush() throw( RoombaIOException );

    /** Close serial communication */
    void Close() throw( RoombaIOException );
  
    /** Constants for generic baud rates */
    static const unsigned int RBAUD_56K = B57600; 
    static const unsigned int RBAUD_19K = B19200; 

  protected:

  private:

    /** Device file descriptor */
    int _portFD;

    /** Original serial settings */
    struct termios _oldTerm;
  };

} // namespace RoombaDriver

#endif // SERIAL_LIN_HH
