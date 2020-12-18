/*!
 * @file   Header.h
 * @brief  Contains the definition of the headers that can
 *         be used by the Messages (\a Message.h) of the finite state machine.
 * @author lhm
 * @date   17/07/2019
 */

#ifndef HEADER_MODELGENERATOR
#define HEADER_MODELGENERATOR

#include <includes.h>

namespace ModGen {

class Field;

/**
 * @brief The Header class manages the data from a configuration
 *        file to create the corresponding headers.
 */
class Header
{
public:    
    /*!
     * \brief Header default constructor
     */
    Header();

    /*!
     * \brief ~Message default destructor
     */
    ~Header();

    /*!
     * \brief isValid check the validity of the current header
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

    void addField(Field* p_field);

    /*!
     * \brief getDesc
     * \return a std::string describing current header parameters.
     */
    std::string getDesc() const;

    /*!
     * \brief getId
     * \return a std::string describing the ID of this header.
     */
    const std::string& getId() const { return name; }

    /*!
     * \brief addToMessage Adds the Header data to the specified message
     * \param p_mesg the message to complete
     */
    void addToMessage(std::vector<uint8_t>& p_mesg) const;

    void mesgSizeParams(const uint32_t& p_sizeValue);

private:
    std::string                   name;    /*!< The ID of the Header     */
    std::map<std::string, Field*> fields;  /*!< The fields of the Header */
};

} // namespace ModGen

#endif // HEADER_MODELGENERATOR
