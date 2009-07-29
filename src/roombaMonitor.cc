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
  void RoombaMonitor::StartMonitor(Serial *serial) {

    /* Copy pointer */
    _serial = serial;

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
  void RoombaMonitor::_readPacket(char *rawBuffer) {

    int num_bytes = 0;
    int total_bytes = 0;
    int req_bytes = 11;
    char *buffer = rawBuffer;

    while(req_bytes > 0) {

      /* Grab bytes */
      num_bytes = _serial->Read(buffer, req_bytes);

      /* Keep stats */
      total_bytes += num_bytes;
      req_bytes -= num_bytes;

      /* Move pointer */
      buffer = buffer + num_bytes;
    }

    /* Verify checksum */
    int csum = 0;
    for(int i = 0; i < total_bytes; i++) {
      csum += rawBuffer[i];
    }

    if((csum & 0xFF) == 0) {
      printf("Checksum verified\n");
    }

    for(int i=0; i<total_bytes; i++) {
      printf("%d ",rawBuffer[i]);
    }
    printf("\n");
  }

  /*!
   *
   */
  void* RoombaMonitor::_monitorThread(void* thread_arg) {

    RoombaMonitor* monitor = (RoombaMonitor*) thread_arg;

    char buffer[255];
    
    try {
      for(;;) {

	/* Grab raw packet */
	monitor->_readPacket(buffer);

	/* Build sensor data */

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

    /* Grab roomba IO exceptions */
    catch(RoombaIOException &e) {
      std::cerr << e.what() << std::endl;
      throw;
    }

    /* A failsafe */
    catch(...) {
      std::cerr << "RoombaMonitor::_monitorThread: Unknown exception!" << std::endl;
    }

    return NULL;
  }

} // namespace RoombaDriver
