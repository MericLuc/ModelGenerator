/*!
 * @file   Logger.h
 * @brief  Contains the logger managing the data logged and displayed
 *         by the library.
 * @author lhm
 * @date   16/07/2019
 */

#ifndef LOGGER_MODELGENERATOR
#define LOGGER_MODELGENERATOR

#include <fstream>
#include <vector>

namespace ModGen {

/*!
 * \dir Logger
 * The \b Logger directory holds the source files of the \n
 * \e Logger used to log system informations and debugging messages
 * for the <b> ModelGenerator Library </b>.
 */

void ERROR(const std::string& p_msg);
void INFO (const std::string& p_msg);
void DEBUG(const std::string& p_msg);
void DEBUG(const std::vector<uint8_t> &p_byteArray);
void WRITE(const std::string& p_msg, const int32_t& p_traceLevel);

/**
 * @brief The Logger class manages the data to log and to display.
 */
class Logger
{

public:
    /**
     * Enumerate describing the possible logging modes.
     */
    typedef enum {
        NO_LOGS           = -1,/*!< No logs at all                  */
        ERRORS_ONLY       = 0, /*!< Log errors only                 */
        ERRORS_INFO       = 1, /*!< Log errors and info             */
        ERRORS_INFO_DEBUG = 2  /*!< Log errors, info and debug data */
    } TRACELEVELS;

    /**
     * @brief Logger::setLogFile set a new output file for the log
     * @param p_filePath path of the log file
     */
    static void setLogFile(const std::string& p_filePath);

    /**
     * @brief setTraceLevel set the level of data to be traced
     * <ul>
     * <li> ERRORS_ONLY       (0 = error only)
     * <li> ERRORS_INFO       (1 = error and info only)
     * <li> ERRORS_INFO_DEBUG (2 = error,info and debug)
     * </ul>
     * @param p_traceLevel the level of trace to set
     */
    static void setTraceLevel(TRACELEVELS p_traceLevel);

    /**
     * @brief isDebugEnabled
     * @return true if debug are logged.
     */
    static bool isDebugEnabled();

    /**
     * @brief isOpened
     * @return true if the log file is opened.
     */
    static bool isOpened();

    /**
     * @brief getLogFile
     * @return the name of the logfile (if set), 
     *         an empty string otherwise.
     */
    static std::string getLogFile();

    /**
     * @brief Logger::setup sets the value of the logger if existing
     * @param p_filePath the path of the log file
     * @param p_traceLevel the level of the data to be traced
     */
    static void setup(const std::string& p_filePath, TRACELEVELS p_traceLevel);

    /**
     * @brief close closes the current log file.
     */
    static void closeLogs();
    
private:
    /**
     * @brief Logger Default constructor
     * NB : Private because of singleton
     */
    Logger();

    /**
     * @brief ~Logger Default destructor
     */
    ~Logger();

    /**
     * @brief getInstance returns the singleton instance of the logger
     * @return the unique instance of the logger
     */
    static Logger& getInstance();

    /**
     * @brief append adds the desired message with specified
     *        trace level to the stream if existing.
     * @param p_msg the message to add.
     * @param p_traceLevel the desired level of trace to give to the message.
     */
    static void append(const std::string& p_msg, TRACELEVELS p_traceLevel);

    /**
     * @brief append adds the desired message with specified
     *        trace level to the stream if existing.
     * @param p_byteArray the byteArray message to add.
     * @param p_traceLevel the desired level of trace to give to the message.
     */
    static void append(const std::vector<uint8_t>& p_byteArray, 
                       TRACELEVELS                 p_traceLevel);

    /**
     * @brief ERROR displays errors only 
     * @param p_msg error message to display
     */
    friend void ERROR(const std::string& p_msg);

    /**
     * @brief INFO displays information and errors
     * @param p_msg message to display
     */
    friend void INFO(const std::string& p_msg);

    /**
     * @brief DEBUG displays debug informations
     * @param p_msg message to display
     */
    friend void DEBUG(const std::string& p_msg);

    /**
     * @brief DEBUG displays debug informations
     * @param p_msg byteArray message to display
     */
    friend void DEBUG(const std::vector<uint8_t>& p_byteArray);

    /**
     * @brief DEBUG displays default informations
     * @param p_msg byteArray message to display
     */
    friend void WRITE(const std::string& p_msg, 
                      const int32_t&     p_traceLevel);

private:
    std::ofstream* logFile;    /*!< The stream used for the logs.  */
    std::string*   logName;    /*!< The file name used for logs.   */
    TRACELEVELS    traceLevel; /*!< current saved level of trace.  */
};

} // namespace ModGen

#endif // LOGGER_MODELGENERATOR
