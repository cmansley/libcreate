/*
 *
 * Code by Chris Mansley
 *
 */

#ifndef ROOMBA_HH
#define ROOMBA_HH

/* Definition dependencies */
#include <string>

#include "dll.hh"

#include "serial.hh"
#include "roombaMonitor.hh"
#include "roombaSensor.hh"

/* Associate namespace */
namespace RoombaDriver {

  /*!
   * \brief A class for interfacing with the Roomba
   *
   * Class Description
   */
  class DLL_EXPORT Roomba
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

    /** Send roomba drive command */
    void RDrive(signed short velocity, signed short radius);

    /** Drive the specified distance */
    void Drive(float distance);

    /** Rotate by the specified angle */
    void Rotate(float degrees);

  protected:

    /** Device path for serial port */
    std::string _roombaPath;

    /** Status of communications */
    bool _roombaInitialized;

    /** Serial port */
    Serial* _serial;

    /** Monitor object */
    RoombaMonitor* _monitor;

    /** Sensor object */
    RoombaSensor* _sensor;

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

  private:
    static const unsigned short _STRAIGHT = 0x8000;
    static const unsigned short _CW = 0xFFFF;
    static const unsigned short _ANTICW = 0x0001;
  };


} // namespace Roomba

#endif // ROOMBA_HH
