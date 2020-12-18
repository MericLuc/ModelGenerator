/*!
 * @file   Field_size.h
 * @brief  Contains the definition of the size fields that can
 *         be used by the Headers (\a Header.h) of the finite state machine.
 * @author lhm
 * @date   29/08/2019
 */

#ifndef FIELD_SIZE_MODELGENERATOR
#define FIELD_SIZE_MODELGENERATOR

#include <includes.h>
#include <Field.h>

namespace ModGen {

/**
 * @brief The Field_size class manages the data from a configuration
 *        file to create the special field 'Field_size' of a header.
 */
class Field_size: public Field
{
public:
    /*!
     * Enumerate describing the unit in which the size of
     * the field is described.
     */
    typedef enum {
        SIZE_FORMAT_U8,   /*!< bytes           */
        SIZE_FORMAT_U16,  /*!< 16 bits words   */
        SIZE_FORMAT_U32,  /*!< 32 bits words   */
        SIZE_FORMAT_UNDEF /*!< Undefined value */
    } FIELD_FORMAT;

    /*!
     * Enumerate describing which part of the field
     * is used to compute de size.
     */
    typedef enum {
        SIZE_INCLUDING_HEADER,  /*!< DATA + HEADER */
        SIZE_EXCLUDING_HEADER,  /*!< DATA only     */
        SIZE_PART_UNDEF         /*!< Undefined     */
    } FIELD_SIZE_PART;

    /*!
     * \brief Field default constructor
     */
    Field_size();

    /*!
     * \brief ~Field default destructor
     */
    virtual ~Field_size();

    /*!
     * \brief Field_size Copy constructor
     * \param p_other
     */
    Field_size(const Field_size& p_other);

    /*!
     * \brief operator = Assignement operator
     * \param p_other
     * \return .
     */
    Field_size& operator = (const Field_size &p_other);

    /*!
     * \brief isValid check the validity of the current field
     * \return true if valid, false otherwise.
     */
    virtual bool isValid() const;

    /*!
     * \brief setParam Sets the desired param with the specified value.
     * \param p_name the name of the parameter to be set.
     * \param p_value the value to give to the parameter.
     */
    virtual void setParam(const std::string& p_name, 
                          const std::string& p_value);

    /*!
     * \brief getDesc
     * \return a std::string describing current field parameters.
     */
    virtual std::string getDesc() const;

    virtual void addToMessage(std::vector<uint8_t>& p_mesg);

    virtual void mesgDataSize(const uint32_t& p_sizeValue);

    virtual void setHeaderSize(uint32_t& p_headerSize);

private:
    FIELD_FORMAT    format;         /*!< Unit used to evaluate the size value (8, 16 or 32 bits).  */
    FIELD_SIZE_PART size_part;      /*!< Part of the message used to compute the size              */
    uint32_t        mesg_data_size; /*!< Size of the DATA (in bytes) in the message.               */
    uint32_t        header_size;    /*!< Size of the HEADER (in BITS) in the message.              */

    static std::map<FIELD_FORMAT, std::string>    
                    formatString;   /*!< Formats for std::string outputs                           */
    static std::map<FIELD_SIZE_PART, std::string> 
                    partString;     /*!< size parts for std::string outputs                        */
};

} // namespace ModGen

#endif // FIELD_SIZE_MODELGENERATOR