/*
 *
 * Code by Chris Mansley
 *
 */

#ifndef SERIAL_HH
#define SERIAL_HH


#if defined (WIN32)
#include "serial_win.hh"
#else
#include "serial_lin.hh"
#endif

#endif // SERIAL_HH
