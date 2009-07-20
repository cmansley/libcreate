/*
 *
 * Code by Chris Mansley
 *
 */

#ifndef ROOMBA_HH
#define ROOMBA_HH

/* Implementation dependencies */
#include <string>
#include <termios.h>

#include "roombaException.hh"

/* Associate namespace */
namespace RoombaDriver {

  /*!
   * \brief A class for interfacing with the Roomba
   *
   * Class Description
   */
  class Roomba
  {
  public:
    
    /** Constructor */
    Roomba( const std::string path );

    /** Destructor */
    ~Roomba( );

    /** Initialize the Roomba */
    void Initialize( ) throw( RoombaIOException );

    /** Uninitialize the Roomba */
    void Uninitialize( ) throw( RoombaIOException );

    /** Send drive command */
    void Drive(signed short velocity, signed short radius) throw( RoombaIOException );

  protected:

    /** Device path for serial port */
    std::string _roombaPath;

    /** Device file descriptor */
    int _roombaFD;

    /** Original serial settings */
    struct termios _oldTerm;

    /** Status of communications */
    bool _roombaInitialized;

    /** Opens serial communications */
    void _setupConnection() throw( RoombaIOException );

    /** Closes serial communication */
    void _teardownConnection() throw( RoombaIOException );

    /** Sets serial baud rate */
    void _setSerialBaud() throw( RoombaIOException );

    /** Flush serial buffer */
    void _flushSerialBuffer() throw( RoombaIOException ) ;

    /** Send start command */
    void _setRoombaStart() throw( RoombaIOException );

    /** Send full mode command */
    void _setRoombaFullMode() throw( RoombaIOException );

    /** Send control command */
    void _setRoombaPassiveToSafeMode() throw( RoombaIOException );
  };


} // namespace Roomba

#endif // ROOMBA_HH
