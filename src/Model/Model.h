/*!
 * @file   Model.h
 * @brief  Contains the model of the finite state machine
 *         modelling the system.
 * @author lhm
 * @date   16/07/2019
 */

#ifndef MODEL_MODELGENERATOR
#define MODEL_MODELGENERATOR

#include <includes.h>

/*!
 * \namespace ModGen is the base namespace
 *            of the <b>ModelGenerator</b> library.
 */
namespace ModGen {

class Header;
class State;
class Message;

/*!
 * \mainpage Introduction
 *
 * This documentation covers the implementation of the static library <b>ModelGenerator</b>
 * used to:
 * <ul>
 *  <li> Load a XML formatted configuration file as specified in the document <em>[Spécifications - fichier de configuration.docx].</em>
 *  <li> Create the corresponding finite-state Automaton (Cf. See the \a Model class.)
 *  <li> Run the Automaton to perform the system's life described in the configuration file.
 * </ul>
 *  \n
 *
 * \section Organisation Sources organisation
 * This software is implemented in seven folders - each linked to a particular aspect: \n
 * <ul>
 * <li>\a <b>Conf</b>: Contains the XML balises names and attributes required to parse the configuration file.
 * <li>\a <b>dependencies</b>: Contains the dependencies of the library such as the library <b>pugixml</b>
 *        for XML parsing utilities, the commonly included headers or some command line parsing functions...
 * <li>\a <b>Exceptions</b>: Contains the implementation of the exception classes used by the library.
 * <li>\a <b>Logger</b>: Contains the implementations of the <em>Logger</em> class used to log informations and debugging messages.
 * <li>\a <b>Model</b>: Contains the classes and structures used to represent the finite-state automaton created by the library.
 * <li>\a <b>include</b>: Contains the interface of the library - this directory should be include by any executable software
 *        willing to use the library.
 * <li>\a <b>Version</b>: Contains the version number of the library, which is updated on build by a <em>Python3 script</em>
 *        using the versionning numbers of the sources.
 * </ul>
 *
 * \section How to use the library
 * This library should be linked against your project and the directory <em>[include]</em> included by the application.
 * The library functions are declared in five subnamespaces of the namespace <b>ModelGeneratorAPI</b>:
 * <ul>
 * <li>\a <b>VERSION</b>: Contains the functions to retrieve the library versions informations.
 * <li>\a <b>EXCEP</b>: Contains the functions relative to Exceptions handling.
 * <li>\a <b>UTIL</b>: Contains utility functions used for default attributes and command line arguments parsing.
 * <li>\a <b>LOGS</b>: Contains the functions necessary to interract with the logs module.
 * <li>\a <b>MODEL</b>: Contains the functions that allow to setup and operate the Model representing
 *                      the finite-state automaton parsed from an XML configuration file.
 * </ul>
 */

/*!
 * \dir Model
 * The \b Model directory holds the source files of the \n
 * \e Model used to represent to finite-state automaton implemented by
 * the <b> ModelGenerator Library </b>.
 */

/*!
 * \brief The parsingHelper struct provides some
 *        utility functions for file parsing.
 */
struct parsingHelper
{
    /*!
     * \brief isEqual
     * \param p_1 the first operande.
     * \param p_2 the second operande.
     * \return true if equal, false otherwise
     */
    static bool isEqual(const char* p_1, const char* p_2);
};

/**
 * @brief The Model class manages the data from a configuration
 *        file to create and run the corresponding finite state
 *        machine.
 */
class Model
{

public:
    /**
     * Enumerate describing the possible states of the model.
     */
    typedef enum {
        NOT_INITIALIZED   = 0, /*!< Not initialized                 */
        INITIALIZED       = 1, /*!< Initialized but not running     */
        RUNNING           = 2, /*!< Running                         */
        STOPPED           = 3  /*!< Stopped                         */
    } MODELSTATE;

    /*!
     * \brief ModelVar all the variables
     *        (and their value) used by the model
     */
    typedef std::map<std::string, int>     ModelVar;

    /*!
     * \brief ModelMesg is a map linking
     *        every message ID to the corresponding message.
     */
    typedef std::map<std::string, Message> ModelMesg;

    /*!
     * \brief ModelHead is a map linking
     *       every header ID to the corresponding header.
     */
    typedef std::map<std::string, Header>  ModelHead;

    /*!
     * \brief ModelState is a map linking
     *       every State ID to the corresponding State.
     */
    typedef std::map<std::string, State>   ModelState;

    /*!
     * \brief getState
     * \return the current state of the model.
     */
    static const std::string& getStateString();

    /*!
     * \brief getVariables
     * \return the model variables and their values.
     */
    static const std::map<std::string, int>&     getVariables();

    /*!
     * \brief getMessages
     * \return the model messages.
     */
    static const std::map<std::string, Message>& getMessages();

    /*!
     * \brief getHeaders
     * \return the model headers.
     */
    static const std::map<std::string, Header>&  getHeaders();

    /*!
     * \brief getStates
     * \return the model states.
     */
    static const std::map<std::string, State>&   getStates();


    /**
     * @brief log Writes the complete model to the logs
     *        using the \a Logger (Cf. Logger.h)
     */
    static void log();

    /**
     * @brief setup sets the values of the model if existing
     * @param p_filePath the path of the configuration file
     */
    static void setup(const std::string& p_filePath);

    /**
     * @brief nextState makes the Model go into its next State.
     */
    static void nextState(void) { getInstance().curState = getInstance().nexState; }

    /**
     * \brief getNextState returns the next state of the model
     */
    static State* getNextState();

    /**
     * \brief getNextState returns the current state of the model
     */
    static State* getCurrState();

    static void setNextState(State* p_state) { getInstance().nexState = p_state; }

    static void setCurrState(State* p_state) { getInstance().curState = p_state; }

private:
    /**
     * Enumerate of the possible operations to perform
     * on the model variables.
     */
    typedef enum {
        ADD,    /*!< Addition       */
        SUB,    /*!< Substraction   */
        DEL     /*!< Suppresion     */
    } OPERATION;

    /**
     * @brief Model Default constructor
     * NB : Private because of singleton
     */
    Model();

    /**
     * @brief ~Model Default destructor
     */
    ~Model();

    /**
     * @brief getInstance returns the singleton instance of the model
     * @return the unique instance of the model
     */
    static Model& getInstance();

    /**
     * @brief addVariable Add the desired variable with the specified value
     *        to the model variables.
     * @param p_name The name of the variable to be added.
     * @param p_val The value to give to the variable.
     */
    static void addVariable(const std::string& p_name, int p_val);

    /**
     * @brief operateVariable Modify the specified variable.
     * @param p_name The name of the variable to modify.
     * @param p_operation The operation to perform.
     * @param p_value The value of the second operande.
     */
    static void operateVariable(const std::string&  p_name,
                                const OPERATION&    p_operation,
                                int                 p_value);

    /**
     * @brief initializeVariables Initializes the model variables
     * @param p_doc XML configuration document loaded by the pugixml lib
     */
    static void initializeVariables(const pugi::xml_document& p_doc);

    /**
     * @brief initializeMessages Initializes the model messages
     * @param p_doc XML configuration document loaded by the pugixml lib
     */
    static void initializeMessages(const pugi::xml_document& p_doc);

    /**
     * @brief initializeHeaders Initializes the model headers
     * @param p_doc XML configuration document loaded by the pugixml lib
     */
    static void initializeHeaders(const pugi::xml_document& p_doc);

    /**
     * @brief initializeStates Initializes the model states
     * @param p_doc XML configuration document loaded by the pugixml lib
     */
    static void initializeStates(const pugi::xml_document& p_doc);

    /**
     * @brief checkIntegrity Performs model integrity verifications
     *        after the configuration document has been parsed.
     *        This function is needed because some parameters cannot be
     *        checked at construction (especially when referencing structures
     *        that were not parsed at that time).
     */
    static void checkIntegrity();

    /**
     * \brief clear clears the data of the model
     */
    static void clear();

private:
    std::string* confFile;        /*!< The file used for the configurations.   */
    State *      curState;        /*!< Current state of the model.             */
    State *      nexState;        /*!< Next state of the model.                */
    MODELSTATE   currentStateStr; /*!< Current state string of the model.      */

    ModelVar     modelVar;        /*!< Variables used by the model.            */
    ModelMesg    modelMes;        /*!< Messages defined in the model.          */
    ModelHead    modelHead;       /*!< Headers defined in the model.           */
    ModelState   modelState;      /*!< States defined in the model.            */

    static std::map<MODELSTATE, std::string>
                 stateString;     /*!< States of the model for string outputs  */
};

} // namespace ModGen

#endif // MODEL_MODELGENERATOR
