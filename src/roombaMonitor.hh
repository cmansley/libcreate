/*
 *
 * Code by Chris Mansley
 *
 */

#ifndef ROOMBA_MONITOR_HH
#define ROOMBA_MONITOR_HH

/* Implementation dependencies */
#include <string>
#include <pthread.h>

#include "serial.hh"

#include "roombaException.hh"
#include "roombaSensor.hh"

/* Associate namespace */
namespace RoombaDriver {

  /*!
   * \brief A class for storing the Roomba sensors
   *
   * Class Description
   */
  class RoombaMonitor
  {
  public:

    /** Constructor */
    RoombaMonitor();

    /** Destructor */
    ~RoombaMonitor();

    /** Start monitor for roomba */
    void StartMonitor(Serial* serial);

    /** Stop monitor for roomba */
    void StopMonitor();

    /** Grab current sensor object */
    void GrabCurrentSensor(RoombaSensor* sensor);

  private:

    /** Thread structure */
    pthread_t _threadID;

    /** Thread data mutex */
    pthread_mutex_t _threadMutex;

    /** Sensor data mutex */
    pthread_mutex_t _sensorMutex;

    /** Thread running status */
    bool _continueRunning;

    /** Serial object pointer */
    Serial* _serial;

    /** Monitor sensor object */
    RoombaSensor* _sensor;

    /** */
    void _getSensorMutex();
    
    /** */
    void _releaseSensorMutex();

    /** */
    void _getThreadMutex();
    
    /** */
    void _releaseThreadMutex();

    /** Read sensor packet */
    void _readPacket(char *buffer);
    
    /** Monitor thread */
    static void* _monitorThread(void* thread_id);
  };

} // namespace RoombaDriver

#endif // ROOMBA_SENSOR_HH
    
