/*!
 * @file   modelGenerator_interface.h
 * @brief  Contains the interface of library.
 *		   It is therefore the necessary include file when
 *		   linking against the library.
 * @author lhm
 * @date   23/07/2019
 */

#ifndef MODELGENERATOR_INTERFACE_MODELGENERATOR
#define MODELGENERATOR_INTERFACE_MODELGENERATOR

#include <stdbool.h>
#include <string>
#include <vector>

//! Library access interface
namespace ModelGeneratorAPI
{
    //! Version informations access interface
    namespace VERSION
    {
        /*!
         * \brief VersionCommit
         * \return the git hash commit used to build the ModelGenerator library
         */
        std::string VersionCommit(void);

        /*!
         * \brief VersionBranch
         * \return the git branch used to build the ModelGenerator library
         */
        std::string VersionBranch(void);

        /*!
         * \brief Version
         * \return the concatenated version numbers of the ModelGenerator library
         */
        std::string Version(void);
    }

    //! Library utility functions access interface
    namespace UTIL
    {
        /*!
         * \brief getDefaultConfFile
         * \return the default filePath for the conf file
         */
        std::string getDefaultConfFile(void);

        /*!
         * \brief getDefaultLogsFile
         * \return the default filePath for the log file
         */
        std::string getDefaultLogsFile(void);

        /*!
         * \brief getDefaultTraceValue
         * \return the default value for the trace val
         */
        int32_t getDefaultTraceValue(void);

        /*!
         * \brief displayHelp displays the available options of the command
         *        line regarding the modelGenerator library.
         */
        void displayHelp(void);

        /*!
         * \brief getcmd
         * \param p_narg Number of args
         * \param p_nargv Values of the args
         * \param p_options options
         * \return the parsed value
         */
        int getcmd(int          p_narg,
                   char* const* p_nargv,
                   const char*  p_options);

        char* getoptArg(void);
    }

    //! Exception management interface
    namespace EXCEP
    {
        /*!
         *\brief CommandLineError throws an exception when command line
         *       parsing could not be done.
         */
        [[ noreturn ]] void CommandLineError(const std::string& p_mesg);
    }

    //! Version informations access interface
    namespace LOGS
    {
        /*!
         * \brief disable disables the logging module.
         */
        void disable(void);

        /*!
         * \brief setup
         * \param p_logFile the file path to log to
         * \param p_logLevel the trace level of the logs:
         * <ul>
         * <li> -1 : No logs at all
         * <li>  0 : Errors only
         * <li>  1 : Errors and infos
         * <li>  2 : Errors, infos and debug infos
         * </ul>
         */
        void setup(const std::string& p_logFile,
                   const int32_t&     p_logLevel);

        /*!
         * \brief write writes the specified string with the
         *        desired trace level to the log files.
         * \param p_toLog the string to be logged.
         * \param p_logLevel the trace level of the logs:
         * <ul>
         * <li> -1 : No logs at all
         * <li>  0 : Errors only
         * <li>  1 : Errors and infos
         * <li>  2 : Errors, infos and debug infos
         * </ul>
         * By default, uses the highest trace level.
         */
        void write(const std::string& p_toLog,
                   const int32_t&     p_logLevel = 2);

        /*!
         * \brief returns true if the log file is opened
         *        and false otherwise.
         */
        bool isOpened(void);

        /*!
         * \brief returns the name of the log file currently
         *        in use.
         */
        std::string getLogFile(void);

        /*!
         * \brief closes the log file.
         *        Should not be called directly as Logger destructor
         *        already handles it.
         */
        void close(void);
    }

    //! Model informations access interface
    namespace MODEL
    {
        /*!
         * \brief create creates a model from an xml configuration file
         * \param p_confFile the xml conf file path
         */
        void create(const std::string& p_confFile);

        /*!
         * \brief log logs the created Model using the \a Logger
         *        NB: This will log the state of the message only if
         *        the Logger is in INFO (1) mode or higher.
         */
        void log(void);

        /*!
         * \brief getMessages returns the messages associated to
         *        the current state of the model.
         * \return the messages associated to the current state of the model.
         */
        std::vector< std::vector<unsigned char> > getMessages(void);

        /*!
         * \brief getMessagesSrcIp returns the list of the source ips
         *       associated to the messages to be sent in the current state of the model.
         * \return the list of the source ips associated to the current state of the model.
         */
        std::vector< std::string > getMessagesSrcIp(void);

        /*!
         * \brief getMessagesDstIp returns the list of the destination ips
         *       associated to the messages to be sent in the current state of the model.
         * \return the list of the destination ips associated to the current state of the model.
         */
        std::vector< std::string > getMessagesDstIp(void);

        /*!
         * \brief getMessagesIntface returns the list of the network interfaces
         *       associated to the messages to be sent in the current state of the model.
         * \return the list of the network interfaces associated to the current state of the model.
         */
        std::vector< std::string > getMessagesIntface(void);

        /*!
         * \brief getMessagesSrcPort returns the list of the source ports
         *       associated to the messages to be sent in the current state of the model.
         * \return the list of the source ports associated to the current state of the model.
         */
        std::vector< uint32_t > getMessagesSrcPort(void);

        /*!
         * \brief getMessagesDstPort returns the list of the destination ports
         *       associated to the messages to be sent in the current state of the model.
         * \return the list of the destination ports associated to the current state of the model.
         */
        std::vector< uint32_t > getMessagesDstPort(void);

        /*!
         * \brief runOperations runs the operations of the current state of
         *        the model. Operations modify the variables of the model.
         */
        void runOperations(void);

        /*!
         * \brief runTransitions runs the transitions of the current state of
         *        the model. If the transition condition is filled, it changes the
         *        next state of the model.
         */
        void runTransitions(void);

        /*!
         * \brief nextState go to the next state of the model.
         */
        void nextState(void);

        /*!
         * \brief currentStateString
         * \return the current state name of the model.
         */
        const std::string& currentStateString(void);
    }

    /*!
     * \namespace TIME Mainly used for testing purposes.
     *            You will probably not use any of these functions directly.
     */ 
    namespace TIME 
    {
        /*!
         * \brief getDIANE
         * \param p_time_today_us the time of the current day,
         *        filled by this function.
         * \return the HHR DIANE format of the day time as an 64-bit word.
         */
        uint64_t getDIANE(uint64_t &p_time_today_us);

        /*!
         * \brief getDIANEValue
         * \param p_time_us a duration in us to convert into HHR DIANE format.
         * \return the HHR DIANE format of the specified duration p_time_us.
         */
        uint64_t getDIANEValue(const uint64_t p_time_us);
    }
}

#endif // MODELGENERATOR_INTERFACE_MODELGENERATOR
