#include <iostream>

#include "roombaMonitor.hh"

/* Associate the namespace */
namespace RoombaDriver {

  /*!
   *
   */
  RoombaMonitor::RoombaMonitor() {
  }

  /*!
   *
   */
  RoombaMonitor::~RoombaMonitor() {
  }

  /*!
   *
   */
  void RoombaMonitor::StartMonitor() {

    /* Initialize mutex structure */
    if(pthread_mutex_init(&_threadMutex, NULL) != 0) {
    }

    /* Signal thread to run */
    _continueRunning = true;

    /* Start sensor thread */
    if(pthread_create(&_threadID, NULL, RoombaMonitor::_monitorThread, this) != 0) {
    }

  }

  /*!
   *
   */
  void RoombaMonitor::StopMonitor() {

    /* Secure thread variables */
    _getThreadMutex();

    /* Signal thread to stop*/
    _continueRunning = false;

    /* Release thread variables */
    _releaseThreadMutex();
    
    /* Wait for sensor thread to close */
    if(pthread_join(_threadID, NULL) != 0) {
    }

    /* Destroy mutex */
    if(pthread_mutex_destroy(&_threadMutex) != 0) {
    }

  }

  /*!
   *
   */
  void RoombaMonitor::_getThreadMutex() {
    
    /* Lock thread data mutex */
    if(pthread_mutex_lock(&_threadMutex) != 0) {
    }
  }

  /*!
   *
   */
  void RoombaMonitor::_releaseThreadMutex() {

    /* Release thread data mutex */
    if(pthread_mutex_unlock(&_threadMutex) != 0) {
    }
  }


  /*!
   *
   */
  void* RoombaMonitor::_monitorThread(void* thread_arg) {

    RoombaMonitor* monitor = (RoombaMonitor*) thread_arg;

    try {
      for(;;) {

	/* Secure thread variables */
	monitor->_getThreadMutex();

	/* Check stoppage */
	if(!monitor->_continueRunning) {
	  monitor->_releaseThreadMutex();
	  break;
	}

	/* Release thread variables */
	monitor->_releaseThreadMutex();

	// delay?
      }
    }

    /* A failsafe */
    catch(...) {
      std::cerr << "RoombaMonitor::_monitorThread: Unknown exception!" << std::endl;
    }

    return NULL;
  }

} // namespace RoombaDriver
