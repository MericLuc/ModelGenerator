/*!
 * @file   Field.h
 * @brief  Contains the definition of the fields that can
 *         be used by the Headers (\a Header.h) of the finite state machine.
 * @author lhm
 * @date   29/08/2019
 */

#ifndef FIELD_MODELGENERATOR
#define FIELD_MODELGENERATOR

#include <includes.h>

namespace ModGen {

/**
 * @brief The FieldHelper struct contains utility
 *        functions to help transform a \a Field into
 *        a byte message.
 */
struct FieldHelper
{
    /*!
     * \brief int32ToChar converts an int32 to a Byte array
     * \param p_int the integer value to be converted
     * \return the converted value as a byte array
     */
    static std::vector<uint8_t> int32ToChar(int32_t& p_int);

    /*!
     * \brief getMSB returns the position of the most significant
     *               bit of the specified number.
     * \param p_int the specified number
     * \return the MSB position
     */
    static uint32_t getMSB(int64_t p_int);

    /*!
     * \brief addBin adds the value as a binary to the specified
     *        position of the desired byteArray.
     * \param p_value the value to add to the array
     * \param p_pos the position at which the value is added.
     * \param p_mesg the array to add the value to.
     */
    static void addBin(uint8_t               p_value,
                       uint32_t              p_pos,
                       std::vector<uint8_t>& p_mesg);

    /*!
     * \brief printBits prints the desired message as bits array
     * \param p_mesg the message to print.
     * \param p_optTitle An optional title message
     */
    static void printBits(std::vector<uint8_t>& p_mesg, 
                          const std::string&    p_optTitle = "");

    template <typename T> static T swap_endian(T p_value);
};

/**
 * @brief The Field class manages the data from a configuration
 *        file to create the fields of a header.
 */
class Field
{
public:
    /*!
     * Enumerate defining the endianness
     * used to represent the data of the field.
     */
    typedef enum {
        _LITTLE_ENDIAN,
        _BIG_ENDIAN,
        _UNDEFINED
    } _ENDIAN;

    /*!
     * \brief Field default constructor
     */
    Field();

    /*!
     * \brief Field Copy constructor
     * \param p_other
     */
     Field(const Field& p_other);

     /*!
      * \brief operator = Assignement operator
      * \param p_other
      * \return .
      */
     Field& operator = (const Field &p_other);

    /*!
     * \brief ~Field default destructor
     */
    virtual ~Field();

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

    /*!
     * \brief getId
     * \return a std::string describing the ID of this field.
     */
    const std::string& getId() const { return name; }

    /*!
     * \brief addToMessage adds the current field to the specified message data.
     * \param p_mesg the message data to be completed.
     */
    virtual void addToMessage(std::vector<uint8_t>& p_mesg);

    virtual void mesgDataSize(const uint32_t&) {}

    virtual void updateHeaderSize(uint32_t& p_headerSize);

    virtual void setHeaderSize(uint32_t&) {}

protected:
    std::string name;      /*!< The ID of the field               */
    int32_t     pos;       /*!< The position (bits) of the field  */
    int32_t     size;      /*!< Size (bits) of the field          */
    int64_t     value;     /*!< Value of the field                */
    _ENDIAN     endianness;/*!< Endianness of the field           */
    bool        swap;      /*!< Swapping of the data              */
    bool        invert;    /*!< Inversion of the data             */

    static std::map<_ENDIAN, std::string> 
            endianString; /*!< Endianness for std::string outputs */

private:
    Field(const std::string& p_name, 
          int32_t            p_pos, 
          int32_t            p_size,
          int32_t            p_value, 
          _ENDIAN            p_endian, 
          bool               p_swap, 
          bool               p_invert):
        name(p_name), 
        pos(p_pos), 
        size(p_size), 
        value(p_value), 
        endianness(p_endian),
        swap(p_swap), 
        invert(p_invert) {}
};

/*!
 * \brief The Field_Creator class is a parameterized Factory method
 *        implementation used to create the possible \a Field of a \a Header
 */
class Field_Creator {
public:
    static Field* Create(const std::string& p_name);
};

} // namespace ModGen

#endif // FIELD_MODELGENERATOR