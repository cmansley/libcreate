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
      _generalString(generalString), _detailedString("") { }
    
    /*!
     *
     */
    RoombaException( const std::string generalString, const std::string detailedString ) :
      _generalString(generalString), _detailedString(detailedString) { }

    
    /*!
     *
     */
    virtual const char* what( ) const throw() {
      std::string result = _generalString + " " + _detailedString;
      return result.c_str();
    }

    ~RoombaException() throw() {}

  private:
    
    /** String identifier */
    std::string _generalString;
    
    /** Detailed string for each sub-class */
    std::string _detailedString;

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
      RoombaException("ERROR: I/O exception -",detailedString) { }
    
    /**
     * \brief Destructor
     */
    ~RoombaIOException() throw() { }
    
  };

} // namespace Roomba

#endif // ROOMBA_EXCEPTION_HH
