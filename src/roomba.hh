/*
 *
 * Code by Chris Mansley
 *
 */

#ifndef ROOMBA_HH
#define ROOMBA_HH

/* Implementation dependencies */
#include <string>

#include "serial.hh"

#include "roombaMonitor.hh"
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
    void Initialize( );

    /** Uninitialize the Roomba */
    void Uninitialize( );

    /** Send drive command */
    void Drive(signed short velocity, signed short radius);

  protected:

    /** Device path for serial port */
    std::string _roombaPath;

    /** Status of communications */
    bool _roombaInitialized;

    /** Serial port */
    Serial _serial;

    /** Monitor object */
    RoombaMonitor* _monitor;

    /** Opens serial communications */
    void _setupConnection();

    /** Closes serial communication */
    void _teardownConnection();

    /** Flush serial buffer */
    void _flushSerialBuffer();

    /** Send start command */
    void _setRoombaStart();

    /** Send full mode command */
    void _setRoombaFullMode();

    /** Send control command */
    void _setRoombaPassiveToSafeMode();

    /** Start sensor stream */
    void _startRoombaStream();

    /** Stop sensor stream */
    void _stopRoombaStream();
  };


} // namespace Roomba

#endif // ROOMBA_HH
