/*!
 * @file   Conf_format.h
 * @brief  Contains the different definitions of the configuration
 *         format (Cf. "Specifications - fichier de configuration.word")
 * @author lhm
 * @date   16/07/2019
 */

#ifndef CONF_FORMAT_MODELGENERATOR
#define CONF_FORMAT_MODELGENERATOR

#include <string>

namespace ModGen {

/*!
 * \dir Conf
 * The \b Conf directory holds the source files defining the \n
 * names of the xml balises used by the \e configuration file parsed by the
 * <b> ModelGenerator Library </b>.
 */

#define MAX_NAME_SIZE 255 /*!< Maximal size of a parameter in the configuration file */

struct Conf_format {
    static const std::string root; /*!< string used for the root tag                           */
};

struct Variables_format {
    static const std::string balise; /*!< string used for the balise tag                     */
    static const std::string name;   /*!< string used as name for the balise tag             */
    static const std::string init;   /*!< string used as init value name for the balise tag  */
};

struct Messages_format {
    static const std::string balise;   /*!< string used for the balise tag                       */
    static const std::string name;     /*!< string used as name for the balise tag               */
    static const std::string header;   /*!< string used as header name                           */
    static const std::string size;     /*!< string used as size name                             */
    static const std::string src_ip;   /*!< string used as source ip name                        */
    static const std::string dst_ip;   /*!< string used as destination ip name                   */
    static const std::string src_port; /*!< string used as source port name                      */
    static const std::string dst_port; /*!< string used as destination port name                 */
    static const std::string fill;     /*!< string used as name for the filling method           */
    static const std::string interface;/*!< string used as name for the interface param          */
};

struct Headers_format {
    static const std::string balise;   /*!< string used for the balise tag                       */
    static const std::string name;     /*!< string used as name for the balise tag               */
};

struct Field_format {
    static const std::string balise;   /*!< string used for the balise tag                       */
    static const std::string name;     /*!< string used as name for the balise tag               */
    static const std::string pos;      /*!< string used as name for the position param           */
    static const std::string size;     /*!< string used as name for the size param               */
    static const std::string value;    /*!< string used as name for the value param              */
    static const std::string endian;   /*!< string used as name for the endianness param         */
    static const std::string swap;     /*!< string used as name for the swap param               */
    static const std::string invert;   /*!< string used as name for the invert param             */
    static const std::string part;     /*!< string used as name for the part param               */
    static const std::string format;   /*!< string used as name for the format param             */
};

struct Field_time_format {
    static const std::string balise;   /*!< string used for the balise tag                       */
};

struct Field_size_format {
    static const std::string balise;   /*!< string used for the balise tag                       */
};

struct Field_id_format {
    static const std::string balise;   /*!< string used for the balise tag                       */
};

struct State_format {
    static const std::string balise;   /*!< string used for the balise tag                       */
    static const std::string balise_2; /*!< string used for the balise tag                       */
    static const std::string name;     /*!< string used for the id param                         */
};

struct StateOp_format {
    static const std::string balise;   /*!< string used for the balise tag                       */
    static const std::string balise_2; /*!< string used for the balise tag                       */
    static const std::string var;      /*!< string used for the var param                        */
    static const std::string operande; /*!< string used for the operande param                   */
    static const std::string value;    /*!< string used for the value param                      */
};

struct StateMessage_format {
    static const std::string balise;   /*!< string used for the balise tag                       */
    static const std::string balise_2; /*!< string used for the balise tag                       */
    static const std::string name;     /*!< string used for the id param                         */
};

struct StateTransition_format {
    static const std::string balise;   /*!< string used for the balise tag                       */
    static const std::string balise_2; /*!< string used for the balise tag                       */
    static const std::string dest;     /*!< string used for the dest_state param                 */
};

struct StateTransitionCondVar_format {
    static const std::string balise;   /*!< string used for the balise tag                       */
    static const std::string name;     /*!< string used for the id param                         */
    static const std::string value;    /*!< string used for the value param                      */
    static const std::string operande; /*!< string used for the operande param                   */
};

struct StateTransitionLoop_format {
    static const std::string balise;     /*!< string used for the balise tag                     */
    static const std::string times;      /*!< string used for the loop number param              */
    static const std::string delay;      /*!< string used for the loop delay param               */
};

struct StateTransitionCondDelay_format {
    static const std::string balise;     /*!< string used for the balise tag                     */
    static const std::string value;      /*!< string used for the delay cond param               */
};

} // namespace ModGen

#endif //CONF_FORMAT_MODELGENERATOR
