/*!
 * @file   Field_id.h
 * @brief  Contains the definition of the ID fields that can
 *         be used by the Headers (\a Header.h) of the finite state machine.
 * @author lhm
 * @date   29/08/2019
 */

#ifndef FIELD_ID_MODELGENERATOR
#define FIELD_ID_MODELGENERATOR

#include <includes.h>
#include <Field.h>

namespace ModGen {

/**
 * @brief The Field_id class manages the numeric ID field of a Header.
 */
class Field_id: public Field
{
public:
    /*!
     * \brief Field_time default constructor
     */
    Field_id();

    /*!
     * \brief ~Field_time default destructor
     */
    virtual ~Field_id();

    /*!
     * \brief Field_time Copy constructor
     * \param p_other
     */
    Field_id(const Field_id& p_other);

    virtual bool isValid() const;

    virtual std::string getDesc() const;

    virtual void addToMessage(std::vector<uint8_t>& p_mesg);
};

} // namespace ModGen

#endif // FIELD_ID_MODELGENERATOR