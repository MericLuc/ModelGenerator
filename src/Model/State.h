/*!
 * @file   State.h
 * @brief  Contains the State class of the finite state machine
 *         modelling the possible states of the model.
 * @author lhm
 * @date   17/07/2019
 */

#ifndef STATE_MODELGENERATOR
#define STATE_MODELGENERATOR

#include <includes.h>

namespace ModGen {

class State;
class Message;

/*!
 * \brief The Transition class is the base abstract class
 *        of every classes representing a transition.
 */
class Transition
{
public:
    virtual ~Transition() = 0;

    /*!
     * \brief isValid() checks the validity of the transition.
     */
    virtual bool isValid() const = 0;

    /*!
     * \brief getDesc
     * \return a std::string description of the transition.
     */
    virtual std::string getDesc() const = 0;

    /*!
     * \brief setParam Sets the desired param with the specified value.
     * \param p_name the name of the parameter to be set.
     * \param p_value the value to give to the parameter.
     */
    virtual void setParam(const std::string& p_name, const std::string& p_value) = 0;

    /*!
     * \brief getDestState
     * \return the destination state of the transition.
     */
    State* getDestState() const {return dest_state; }

    /*!
     * \brief setDestState sets the destination state of the transition
     * \param p_state
     */
    void setDestState(State* p_state) { dest_state = p_state; }

    /*!
     * \brief getDestStateName returns the name ID of the destination
     *                         state as it was parsed.
     * \return the destination state sring ID.
     */
    const std::string& getDestStateName() const { return dest_name; }

    /*!
     * \brief getVar
     * \return the name of the referenced variable of the condition
     */
    virtual std::string getVar() { return ""; }

    virtual void setVar(int&) { /* DUMMY */ }

    /*!
     * \brief run
     * \return true if the transition is to be made, false otherwise.
     */
    virtual bool run() = 0;

protected:
    State*      dest_state;  /*!< The destination state of the transition */
    std::string dest_name;   /*!< The name of the destination state       */
};

/*!
 * \brief The LoopTransition class describes the transitions
 *        associated to loops (i.e. The transitions which have
 *        destination State equals initial State).
 */
class LoopTransition : public Transition
{
public:
    LoopTransition();
    virtual ~LoopTransition();

    virtual bool isValid() const;

    virtual std::string getDesc() const;

    virtual void setParam(const std::string& p_name, const std::string& p_value);

    virtual bool run();

private:
    int32_t delay;      /*!< Delay in microseconds between each loop */
    int32_t times;      /*!< The number of times to loop             */
    int32_t cur_cmpt;   /*!< The current iteration of the loop       */
};

/*!
 * \brief The VarConditionTransition class describes the
 *        transitions that happen after a test on a variable
 *        of the \a Model (Cf. Model.h)
 */
class VarConditionTransition : public Transition
{
public:
    /*!
     * Enumerate defining the possible conditions
     * of test for the variable condition.
     */
    typedef enum {
        OP_OVER,    /*!< Test >     */
        OP_UNDER,   /*!< Test <     */
        OP_EQUAL,   /*!< Test ==    */
        UNKNOWN     /*!< UNDEFINED  */
    } OPERANDE;

    VarConditionTransition();
    virtual ~VarConditionTransition();

    virtual bool isValid() const;

    virtual std::string getDesc() const;

    virtual void setParam(const std::string& p_name, const std::string& p_value);

    virtual void setVar(int& p_var) { var = &p_var; }

    virtual std::string getVar() { return var_name; }

    virtual bool run();

private:
    int32_t*     var;        /*!< The variable to be tested                 */
    std::string  var_name;   /*!< The variable name                         */
    int32_t      value;      /*!< The value to test the variable with       */
    OPERANDE     operande;   /*!< The performed test                        */
    bool         defaut;     /*!< Defines if the condition is a default one */

    static std::map<OPERANDE, std::string>
                 operandeString; /*!< Test methods for std::string outputs  */
};

/*!
 * \brief The DelayConditionTransition class describes the
 *        transitions that happen after a specified delay
 *        has been reached.
 */
class DelayConditionTransition : public Transition
{
public:
    DelayConditionTransition();
    virtual ~DelayConditionTransition();

    virtual bool isValid() const;

    virtual std::string getDesc() const;

    virtual void setParam(const std::string& p_name, 
                          const std::string& p_value);

    virtual bool run();

private:
    int32_t  delay_value;     /*!< The delay value in microseconds */
};

/*!
 * \brief The Operation class contains the possible operations
 *        on the \a Model (Cf. Model.h) variables.
 */
class Operation
{
public:
    /*!
     * Enumerate defining the possible operations on the variable
     */
    typedef enum {
        OP_ADD,    /*!< Performs addition on the variable     */
        OP_SUB,    /*!< Performs substraction on the variable */
        OP_DEL,    /*!< Deletes the variable                  */
        OP_ASSIGN, /*!< Assigns value to the variable         */
        OP_UNKNOWN /*!< Unknown operation                     */
    } OPERANDE;

    /*!
     * \brief Operation default constructor.
     */
    Operation();

    /*!
     * \brief ~Operation() default destructor.
     */
    ~Operation();

    /*!
     * \brief run Performs the operation.
     */
    void run();

    /*!
     * \brief isValid() checks the validity of the operation.
     */
    bool isValid() const;

    /*!
     * \brief getVar returns the variable operated
     *        by this operation.
     */
    const std::string& getVar() const { return var_name; }

    /*!
     * \brief getDesc
     * \return a std::string description of the operation.
     */
    std::string getDesc() const;

    void setVariable(int32_t& p_variable) { var = &p_variable; }

    /*!
     * \brief setParam Sets the desired param with the specified value.
     * \param p_name the name of the parameter to be set.
     * \param p_value the value to give to the parameter.
     */
    void setParam(const std::string& p_name, 
                  const std::string& p_value);

private:
    std::string  var_name; /*!< The ID of the operated variable   */
    int32_t*     var;      /*!< The operated variable             */
    OPERANDE     operande; /*!< Operation to perform              */
    int32_t      value;    /*!< Value of the operation            */

    static std::map<OPERANDE, std::string>
    operandeString;        /*!< Operandes for std::string outputs */
};

/**
 * @brief The State class manages the data from a configuration
 *        file to create the states of the corresponding finite
 *        state machine.
 */
class State
{
public:
    /*!
     * \brief State default constructor
     */
    State();

    /*!
     * \brief ~State default destructor
     */
    ~State();

    /*!
     * \brief isValid check the validity of the current state
     * \return true if valid, false otherwise.
     */
    bool isValid() const;

    /*!
     * \brief setParam Sets the desired param with the specified value.
     * \param p_name the name of the parameter to be set.
     * \param p_value the value to give to the parameter.
     */
    void setParam(const std::string& p_name, 
                  const std::string& p_value);

    /*!
     * \brief getDesc
     * \return a std::string describing current state parameters.
     */
    std::string getDesc() const;

    /*!
     * \brief getId
     * \return a std::string describing the ID of this message.
     */
    const std::string& getId() const { return name; }

    /*!
     * \brief addMessage adds a \a Message to the current state.
     * \param p_message
     */
    void addMessage(Message* p_message)               { messages.push_back(p_message);        }

    /*!
     * \brief addTransition adds a \a Transition to the current state.
     * \param p_transition
     */
    void addTransition(Transition& p_transition)      { transitions.push_back(&p_transition); }

    /*!
     * \brief addOperation adds an \a Operation to the current state.
     * \param p_operation
     */
    void addOperation(const Operation& p_operation)   { operations.push_back(p_operation);    }

    /*!
     * \brief getTransitions
     * \return the possible \a Transition (s) of the current state.
     */
    std::vector<Transition*>& getTransitions()        { return transitions;                   }

    /*!
     * \brief getMessages
     * \return the list of messages to be sent
     */
    std::vector< std::vector<unsigned char> > getMessages();

    /*!
     * \brief getMessagesSrcIP
     * \return the list of ip_src used for the messages to be sent
     */
    std::vector<std::string>     getMessagesSrcIP(void);

    /*!
     * \brief getMessagesDstIP
     * \return the list of ip_dst used for the messages to be sent
     */
    std::vector< std::string >   getMessagesDstIP(void);

    /*!
     * \brief getMessagesIntface
     * \return the list of network interfaces used for the messages to be sent
     */
    std::vector< std::string >   getMessagesIntface(void);

    /*!
     * \brief getMessagesSrcPort
     * \return the list of port_src used for the messages to be sent
     */
    std::vector< uint32_t > getMessagesSrcPort(void);

    /*!
     * \brief getMessagesDstPort
     * \return the list of port_dst used for the messages to be sent
     */
     std::vector< uint32_t > getMessagesDstPort(void);

    /*!
     * \brief run Performs the operations asociated with this state
     */
    void runOperations();

    /*!
     * \brief run Performs the transitions asociated with this state
     */
    void runTransitions();

private:
    std::string              name;        /*!< Id of the state                              */
    std::vector<Message*>    messages;    /*!< The associated message(s)                    */
    std::vector<Transition*> transitions; /*!< The possible transitions                     */
    std::vector<Operation>   operations;  /*!< The operations to perform on model variables */
};

} // namespace ModGen

#endif // STATE_MODELGENERATOR
