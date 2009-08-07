#include <iostream>

#include "delay.hh"

#include "roombaMonitor.hh"

/* Associate the namespace */
namespace RoombaDriver {

  /*!
   *
   */
  RoombaMonitor::RoombaMonitor() {
    _sensor = new RoombaSensor();
  }

  /*!
   *
   */
  RoombaMonitor::~RoombaMonitor() {
    delete _sensor;
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

    /* Initialize mutex structure */
    if(pthread_mutex_init(&_sensorMutex, NULL) != 0) {
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

    /* Destroy mutex */
    if(pthread_mutex_destroy(&_sensorMutex) != 0) {
    }
  }

  /*!
   *
   */
  void RoombaMonitor::GrabCurrentSensor(RoombaSensor* sensor) {
    
    /* Secure the local copy of sensor */
    _getSensorMutex();

    /* Copy sensor information over */
    *sensor = *_sensor; // see copy assignment operator for documentation
    _sensor->Reset(); 

    /* Release sensor information */
    _releaseSensorMutex();
    
  }

  /*!
   *
   */
  void RoombaMonitor::_getSensorMutex() {
    
    /* Lock thread data mutex */
    if(pthread_mutex_lock(&_sensorMutex) != 0) {
    }
  }

  /*!
   *
   */
  void RoombaMonitor::_releaseSensorMutex() {

    /* Release thread data mutex */
    if(pthread_mutex_unlock(&_sensorMutex) != 0) {
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
  int RoombaMonitor::_readPacket(char *rawBuffer) {

    int num_bytes = 0;
    int total_bytes = 0;
    int req_bytes;
    char *buffer = rawBuffer;
    int csum = 0;

    /* Number of timeouts */
    int timeout = 0;

    /* Grab first bytes */
    while(total_bytes < 2) {
      num_bytes = _serial->Read(buffer+total_bytes, 1);
      total_bytes += num_bytes;
      
      /* Check for bytes */
      if(total_bytes > 0) {
	
	/* Check for stream command */
	if(buffer[0] != 19) {
	  
	  /* If the first byte is not correct, move everything over one and check again*/
	  buffer[0] = buffer[1];
	  buffer[1] = buffer[2];
	  total_bytes -= 1;
	}

      } else {
	/* Timeout has occured */
	timeout++; 
	if(timeout > 3) {
	  return 0;
	}
      }
    }

    /* Second byte tells how many more bytes to read plus one for checksum*/
    req_bytes = buffer[1] + 1;

    /* Reset timeout */
    timeout = 0;

    /* Grab final bytes */
    while(req_bytes > 0) {

      /* Grab bytes */
      num_bytes = _serial->Read(buffer+total_bytes, req_bytes);

      /* Perform timeout handling */
      if(num_bytes == 0) {
	timeout++; 
	if (timeout > 3) {
	  return 0;
	}
      }

      /* Keep stats */
      total_bytes += num_bytes;
      req_bytes -= num_bytes;
    }

    /* Verify checksum */
    for(int i = 0; i < total_bytes; i++) {
      csum += rawBuffer[i];
    }

    if((csum & 0xFF) != 0) {
      std::cerr << "RoombaMonitor::_readPacket: Checksum Failed" << std::endl;
      return 0;
    }

    return total_bytes;
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
	if(monitor->_readPacket(buffer) != 0) {

	  /* Secure the local copy of sensor */
	  monitor->_getSensorMutex();
	  
	  /* Build sensor data */
	  monitor->_sensor->Update(buffer);
	  
	  /* Release sensor information */
	  monitor->_releaseSensorMutex();
	}
	  
	/* Secure thread variables */
	monitor->_getThreadMutex();
	
	/* Check stoppage */
	if(!monitor->_continueRunning) {
	  monitor->_releaseThreadMutex();
	  break;
	}

	/* Release thread variables */
	monitor->_releaseThreadMutex();

	/* Wait it out */
	delay(10);
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
