/*
 *
 * Code by Chris Mansley
 *
 */

#ifndef DELAY_HH
#define DELAY_HH


#if defined (WIN32)
#include "delay_win.hh"
#else
#include "delay_lin.hh"
#endif

#endif // DELAY_HH
