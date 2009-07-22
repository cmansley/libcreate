/*!
 *
 * Chris Mansley
 */

#ifndef ROOMBA_EXCEPTION_HH
#define ROOMBA_EXCEPTION_HH

#include <exception>

/* Associate namespace */
namespace RoombaDriver {

  /*!
   *
   */
  class RoombaException : std::exception {

  public:

    /*!
     *
     */
    RoombaException( const std::string generalString ) :
      _what(generalString) { }
    
    /*!
     *
     */
    RoombaException( const std::string generalString, const std::string detailedString ) :
      _what(generalString + detailedString) { }

    
    /*!
     *
     */
    virtual const char* what( ) const throw() {
      return _what.c_str();
    }

    ~RoombaException() throw() {}

  private:

    /** Full error message */
    std::string _what;

  };

  /**
   * \class RoombaIOException
   * \brief Thrown instance where the driver can't
   *        read,write,drain,flush,... the buffers
   */
  class RoombaIOException : public RoombaException {

  public:

    /**
     * \brief Constructor
     */
    RoombaIOException() :
      RoombaException("ERROR: I/O exception") { }

    /**
     * \brief Constructor
     * \param detailedString A more detailed description
     */
    RoombaIOException( const std::string detailedString ) :
      RoombaException("ERROR: I/O exception - ",detailedString) { }
    
    /**
     * \brief Destructor
     */
    ~RoombaIOException() throw() { }
    
  };

} // namespace Roomba

#endif // ROOMBA_EXCEPTION_HH
