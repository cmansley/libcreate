/*
 *
 * Code by Chris Mansley
 *
 */

#ifndef DLL_HH
#define DLL_HH

/*
 * Note: CMAKE will define libcreate_EXPORTS if it is compiling for a
 * library, but not for an executable. So, this allows us to use the
 * same file for library compiling and executable compiling
 */
#if defined (_WIN32) 
  #if defined(libcreate_EXPORTS)
    #define  DLL_EXPORT __declspec(dllexport)
  #else
    #define  DLL_EXPORT __declspec(dllimport)
  #endif 
#else 
  #define DLL_EXPORT
#endif


#endif // DLL_HH
