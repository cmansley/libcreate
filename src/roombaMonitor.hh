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

#include "roombaException.hh"

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
    void StartMonitor();

    /** Stop monitor for roomba */
    void StopMonitor();

  private:

    /** Thread structure */
    pthread_t _threadID;

    /** Monitor thread */
    static void* _monitorThread(void* thread_id);
  };

} // namespace RoombaDriver

#endif // ROOMBA_SENSOR_HH
    
