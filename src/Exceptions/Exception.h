/*!
 * @file   Exception.h
 * @brief  Contains the different exceptions thrown by the library
 * @author lhm
 * @date   16/07/2019
 */

#ifndef EXCEPTION_MODELGENERATOR
#define EXCEPTION_MODELGENERATOR

#include <exception>
#include <typeinfo>
#include <string>
#include <cstring>

namespace ModGen {

/*!
 * \dir Exceptions
 * The \b Exceptions directory holds the header file defining the possible \n
 * \e Exception used by the <b> ModelGenerator Library </b>.
 */

namespace Exception{

/**
 * @brief The MissingParameter struct is thrown if no parameter is found 
 *        for the corresponding name.
 */
struct MissingParameter : std::exception
{
    MissingParameter(const std::string& p_name, const char * p_type): 
        message("Missing parameter " + p_name + " of type " + p_type) {}

    /**
     * @brief ~MissingParameter default destructor
     */
    virtual ~MissingParameter() noexcept {}

    /**
     * @brief what returns a char string describing the exception
     * @return the message to display
     */
    const char* what() const noexcept { return message.c_str(); }

    std::string message; /*!< message to display */
};

/**
 * @brief The WrongTypeParameter struct is thrown if the parameter cannot be 
 *        casted to the right type.
 */
struct WrongTypeParameter : std::exception
{
    WrongTypeParameter(const std::string& p_name, const char * p_type): 
        message("Parameter " + p_name + " is not of type " + p_type) {}

    /**
     * @brief ~WrongTypeParameter  default destructor
     */
    virtual ~WrongTypeParameter() noexcept {}

    /**
     * @brief what returns a char string describing the exception
     * @return the message to display
     */
    const char* what() const noexcept { return message.c_str(); }

    std::string message; //!< message to display
};

/**
 * @brief The UnimplementedElement struct is thrown if an element 
 *        is missing in an enumeration
 */
template<class T> struct UnimplementedElement : std::exception
{
    UnimplementedElement(int p_element): 
        message("Unimplemented element " + std::to_string(p_element) + " of type " + typeid(type).name()) {}

    UnimplementedElement(const std::string& p_element): 
        message("Unimplemented element " + p_element + " of type " + typeid(type).name()) {}

    /**
     * @brief ~UnimplementedElement default destructor
     */
    virtual ~UnimplementedElement() noexcept {}

    /**
     * @brief what returns a char string describing the exception
     * @return message to display
     */
    const char* what() const noexcept { return message.c_str(); }

    std::string  message; /*!< message to display                         */
    typedef T    type;    /*!< type of the enumeration missing an element */
};

/**
 * @brief The ParsingFileError struct is thrown if data from a file 
 *        cannot be converted correctly
 */
struct ParsingFileError : std::exception
{
    ParsingFileError(const std::string& p_msg, int p_position = 0):
        message(p_msg),
        position(p_position) { message.append("(position " + std::to_string(position) + ")"); }

    /**
     * @brief ~ParsingFileError default destructor
     */
    virtual ~ParsingFileError() noexcept {}

    /**
     * @brief what returns a char string describing the exception
     * @return message to display
     */
    const char* what() const noexcept { return message.c_str(); }

    std::string message;  /*!< message to display                */
    int         position; /*!< position of the error in the file */
};

/**
 * @brief The ParsingFileParamError struct is thrown if an incorrect
 *        parameter is found during structure parsing.
 */
template<class T> struct ParsingFileParamError : std::exception
{
    ParsingFileParamError(int p_element):
        message("Unrecognized parameter " + std::to_string(p_element) ) {}

    ParsingFileParamError(const std::string& p_element):
        message("Unrecognized parameter " + p_element) {}

    /**
     * @brief ~ParsingFileParamError default destructor
     */
    virtual ~ParsingFileParamError() noexcept {}

    /**
     * @brief what returns a char string describing the exception
     * @return message to display
     */
    const char* what() const noexcept { return message.c_str(); }

    std::string  message; /*!< message to display                                  */
    typedef T    type;    /*!< type of the structure parsed with incorrect element */
};

/**
 * @brief The ParsingFileBaliseError struct is thrown if an incorrect
 *        balise is found during structure parsing.
 */
template<class T> struct ParsingFileBaliseError : std::exception
{
    ParsingFileBaliseError(int p_element):
        message("Unrecognized balise " + std::to_string(p_element) ) {}

    ParsingFileBaliseError(const std::string& p_element):
        message("Unrecognized balise " + p_element) {}

    /**
     * @brief ~ParsingFileParamError default destructor
     */
    virtual ~ParsingFileBaliseError() noexcept {}

    /**
     * @brief what returns a char string describing the exception
     * @return message to display
     */
    const char* what() const noexcept { return message.c_str(); }

    std::string message; /*!< message to display                                  */
    typedef T   type;    /*!< type of the structure parsed with incorrect element */
};

/**
 * @brief The IntegrityCheckException struct is thrown if the
 *        integrity check of the model failed.
 */
template<class T> struct IntegrityCheckException : std::exception
{
    IntegrityCheckException(int p_element):
        message("Integrity check failed : " + std::to_string(p_element) ) {}

    IntegrityCheckException(const std::string& p_element):
        message("Integrity check failed : " + p_element) {}

    /**
     * @brief ~ParsingFileParamError default destructor
     */
    virtual ~IntegrityCheckException() noexcept {}

    /**
     * @brief what returns a char string describing the exception
     * @return message to display
     */
    const char* what() const noexcept { return message.c_str(); }

    std::string message; /*!< message to display                                  */
    typedef T   type;    /*!< type of the structure parsed with incorrect element */
};

/**
 * @brief The CommandLineArgsError struct is thrown if requiered arguments
 *        from the command line could not be parsed correctly
 */
struct CommandLineArgsError : std::exception
{
    CommandLineArgsError(const std::string& p_msg): message(p_msg) {}

    /**
     * @brief ~ParsingFileError default destructor
     */
    virtual ~CommandLineArgsError() noexcept {}

    /**
     * @brief what returns a char string describing the exception
     * @return message to display
     */
    const char* what() const noexcept { return message.c_str(); }

    std::string message; /*!< message to display */
};

/**
 * @brief The LoggerError struct is thrown if the logger
 *        encountered an error.
 */
struct LoggerError : std::exception
{
    LoggerError(const std::string& p_msg): message(p_msg) {}

    /**
     * @brief ~ParsingFileError default destructor
     */
    virtual ~LoggerError() noexcept {}

    /**
     * @brief what returns a char string describing the exception
     * @return message to display
     */
    const char* what() const noexcept { return message.c_str(); }

    std::string message; /*!< message to display */
};

/**
 * @brief The UnknownFileType struct is thrown if no reader or loader
 *        has been found for the file.
 */
struct UnknownFileType : std::exception
{
    UnknownFileType(const std::string& p_msg): message(p_msg) {}

    /**
     * @brief ~UnknownFileType default destructor
     */
    virtual ~UnknownFileType() noexcept {}

    /**
     * @brief what returns a char string describing the exception
     * @return message to display
     */
    const char* what() const noexcept { return message.c_str(); }

    std::string message; /*!< message to display */
};

} // namespace Exception

} // namespace ModGen

#endif // EXCEPTION_MODELGENERATOR
