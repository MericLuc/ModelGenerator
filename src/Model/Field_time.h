/*!
 * @file   Field_time.h
 * @brief  Contains the definition of the horodating fields that can
 *         be used by the Headers (\a Header.h) of the finite state machine.
 * @author lhm
 * @date   29/08/2019
 */

#ifndef FIELD_TIME_MODELGENERATOR
#define FIELD_TIME_MODELGENERATOR

#include <includes.h>
#include <Field.h>

namespace ModGen {

/**
 * @brief The Field_time class manages the horodating field of a Header.
 */
class Field_time: public Field
{
public:
    /*!
     * Enumerate used to define the format of the timestamp Field.
     */
    typedef enum {
        TIME_UNDEFINED,
        MILLISECONDS,
        MICROSECONDS,
        HHR_DIANE
    } TIME_FORMAT;

    /*!
     * \brief Field_time default constructor
     */
    Field_time();

    /*!
     * \brief ~Field_time default destructor
     */
    virtual ~Field_time();

    /*!
     * \brief Field_time Copy constructor
     * \param p_other
     */
    Field_time(const Field_time& p_other);

    Field_time& operator = (const Field_time &p_other);

    virtual std::string getDesc() const;

    virtual bool isValid() const;

    void setParam(const std::string& p_name, 
                  const std::string& p_value);

    virtual void addToMessage(std::vector<uint8_t>& p_mesg);

private:
    TIME_FORMAT format;

    static std::map<TIME_FORMAT, std::string> 
                timeString; /*!< time formats for std::string outputs */
};

} // namespace ModGen

#endif // FIELD_TIME_MODELGENERATOR