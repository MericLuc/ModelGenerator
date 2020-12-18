/*!
 * @file   Message.h
 * @brief  Contains the definition of the messages that can
 *         be sent by the States (\a State.h) of the finite state machine.
 * @author lhm
 * @date   17/07/2019
 */

#ifndef MESSAGE_MODELGENERATOR
#define MESSAGE_MODELGENERATOR

#include <includes.h>

namespace ModGen {

class Header;

/**
 * @brief The Message class manages the data from a configuration
 *        file to create the corresponding messages.
 */
class Message
{
public:    
    /*!
     * Enumerate describing how to fill the DATA part of the message.
     */
    typedef enum {
        MESG_FILL_RANDOM, /*!< Fill DATA with random values            */
        MESG_FILL_ZERO,   /*!< Fill DATA with zeros                    */
        MESG_FILL_UNSET   /*!< Value when fill method has not been set */
    } FILL_METHOD;

    /*!
     * \brief Message default constructor
     */
    Message();

    /*!
     * \brief ~Message default destructor
     */
    ~Message();

    /*!
     * \brief isValid check the validity of the current message
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
     * \return a string describing current message parameters.
     */
    std::string getDesc() const;

    /*!
     * \brief setHeader Sets the header of the message
     * \param p_header
     */
    void setHeader(Header* p_header){ header_ptr = p_header; }

    /*!
     * \brief getId
     * \return a string describing the ID of this message.
     */
    const std::string& getId() const { return name; }

    /*!
     * \brief getHeaderName
     * \return the stirng ID of the header referenced by the message.
     */
    const std::string& getHeaderName() const { return header; }

    /*!
     * \brief createMessage
     * \return the corresponding data to be sent.
     */
     std::vector<uint8_t> &createMessage();

     // getters
     const std::string&  getSrcIP(void)   { return src_ip;   }
     const std::string&  getDstIP(void)   { return dst_ip;   }
     const std::string&  getIntface(void) { return interface;}
     const uint32_t&     getSrcPort(void) { return src_port; }
     const uint32_t&     getDstPort(void) { return dst_port; }

private:
    std::string     name;       /*!< Identification of the message (two messages cannot have the same name)           */
    std::string     header;     /*!< Referencing the header to use (this ID should reference one and only one header) */
    Header*         header_ptr; /*!< The pointer to the referenced Header                                             */
    uint32_t        data_size;  /*!< Size of the DATA part of the message                                             */

    uint32_t        src_port;   /*!< Source port                                                                      */
    uint32_t        dst_port;   /*!< Destination port                                                                 */
    std::string     src_ip;     /*!< Source IP address                                                                */
    std::string     dst_ip;     /*!< Destination IP address                                                           */
    std::string     interface;  /*!< Network interface to use to send the message                                     */

    FILL_METHOD     fill;       /*!< Method used to fill the DATA part of the message                                 */

    std::vector<uint8_t>
                    mesgToSend; /*!< Byte arrauy of data representing the message to send (or not to send)            */

    static std::map<FILL_METHOD, std::string>
                    fillString; /*!< Fill methods for string outputs                                                  */
};

} // namespace ModGen

#endif // MESSAGE_MODELGENERATOR
