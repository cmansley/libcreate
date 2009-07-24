#include <iostream>

#include "roombaMonitor.hh"
#include "roombaException.hh"

/* Associate the namespace */
namespace RoombaDriver {

  /*!
   *
   */
  void RoombaMonitor::StartMonitor() {
    
    /* Start sensor thread */
    if(pthread_create(&_threadID, NULL, RoombaMonitor::_monitorThread, NULL) != 0) {
    }

  }

  /*!
   *
   */
  void RoombaMonitor::StopMonitor() {

    /* Wait for sensor thread to close */
    if (pthread_join(_threadID, NULL) != 0) {
    }

  }


  /*!
   *
   */
  void* RoombaMonitor::_monitorThread(void* thread_id) {

    try {
      for(;;) {
	
      }
    }

    /* A failsafe */
    catch(...) {
      std::cerr << "RoombaMonitor::_monitorThread: Unknown exception!" << std::endl;
    }

    return NULL;
  }

} // namespace RoombaDriver
