/*!
 * @file   time_util.h
 * @brief  Contains the class responsible for
 *         every time related functions of the library.
 * @author lhm
 * @date   22/07/2019
 */

#ifndef TIME_UTIL_MODELGENERATOR
#define TIME_UTIL_MODELGENERATOR

#include <cstdint>
#include <chrono>

#include "includes.h"

namespace ModGen {

typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;

/*!
 * \brief The HHR_DIANE_Time clas implements the DIANE horodating format
 *        as it is described in the document <em>Dev 109357 Specification
 *        du format Diane </em>.
 */
class HHR_DIANE_Time {

public:

    /*!
     * \brief HHR_DIANE_Time default constructor.
     */
    HHR_DIANE_Time();

    /*!
     * \brief HHR_DIANE_Time constructor.
     * \param p_day_ellapsed_us the time ellapsed in usec
     *        mod 1 day.
     */
    HHR_DIANE_Time(uint64_t p_day_ellapsed_us);

    /*! 
     * \brief Copy Constructor
     */
    HHR_DIANE_Time(const HHR_DIANE_Time &);

    /*! 
     * \brief Assignment operator.
     */
    HHR_DIANE_Time& operator=(const HHR_DIANE_Time &);

    /*!
     * \brief Equality operator.
     */
    bool operator==(const HHR_DIANE_Time &);

    /*!
     * \brief ~HHR_DIANE_Time destructor.
     */
    ~HHR_DIANE_Time();

    /*!
     * \brief get_HPF
     * \return the HPF value of the DIANE horodating format.
     */
    uint16_t get_HPF() const { return m_HPF; }

    /*!
     * \brief get_Hpf
     * \return the Hpf value of the DIANE horodating format.
     */
    uint16_t get_Hpf() const { return m_Hpf; }

    /*!
     * \brief get_HHR
     * \return the HHR value of the DIANE horodating format.
     */
    uint16_t get_HHR() const { return m_HHR; }

    /*!
     * \brief toInt
     * \return the HHR_DIANE_Time to a single 64-bits word.
     */
    uint64_t toInt() const;

private:
    /*!
     * \brief compute_parity computes the parity bit of 
     *        the requested 16-bits value.
     * \param p_value the value on which the parity is computed.
     */
    static void compute_parity(uint16_t& p_value);

    static uint64_t day_microsec; /*!< Number of microseconds in a day       */
    static uint64_t HPF_div;      /*!< Divisor to use to convert us into HPF */
    static uint64_t Hpf_div;      /*!< Divisor to use to convert us into Hpf */

    uint16_t m_HPF; /*!< "Heure poids fort"       - resolution 10s */
    uint16_t m_Hpf; /*!< "Heure poids faible"     - resolution 1ms */
    uint16_t m_HHR; /*!< "Heure haute resolution" - resolution 1us */
};

/*!
 * \brief The TimeUtil class contains utility methods
 *        for every time-related aspects of the library.
 */
class TimeUtil {

public:
    /*!
     * \brief day_milliseconds
     * \return the day horodating in milliseconds.
     */
    static uint64_t day_milliseconds();

    /*!
     * \brief day_microseconds
     * \return the day horodating in microseconds.
     */
    static uint64_t day_microseconds();

    /*!
     * \brief milliseconds
     * \return the horodating in milliseconds.
     */
    static uint64_t milliseconds();

    /*!
     * \brief microseconds
     * \return the horodating in microseconds.
     */
    static uint64_t microseconds();

    /*!
     * \brief ellapsed_seconds
     * \return the number of seconds since the program started.
     */
    static uint64_t ellapsed_seconds();

    /*!
     * \brief ellapsed_milliseconds
     * \return the number of milliseconds since the program started.
     */
    static uint64_t ellapsed_milliseconds();

    /*!
     * \brief ellapsed_microseconds
     * \return the number of microseconds since the program started.
     */
    static uint64_t ellapsed_microseconds();

    /*!
     * \brief hhr_diane
     * \return the HHR DIANE horodating format (of the current day).
     */
    static uint64_t hhr_diane();

    /*!
     * \brief hhr_diane_since_start
     * \return the HHR DIANE horodating format 
     *         (counted since programm started).
     */
    static uint64_t hhr_diane_since_start();

    static void setUTC(int p_value) { utc_hours       = p_value;
                                      utc_coeff       = hour_millisec * utc_hours;
                                      utc_coeff_micro = utc_coeff*1000; }

    /*!
     * \brief currentTime
     * \return the current day time in the format %H %M %S %MS.
     */
    static std::string currentTime();

private:
    static TimePoint time_ref;       /*!< Software time reference (initialized at start of the software)              */
    static int64_t  utc_hours;       /*!< Number of hours of offset in UTC time (Ex/ France is UTC -1)                */
    static uint64_t hour_millisec;   /*!< Number of milliseconds in an hour                                           */
    static uint64_t utc_coeff;       /*!< Coeff to apply to epoch() to adapt to system UTC time in milliseconds       */
    static uint64_t utc_coeff_micro; /*!< Coeff to apply to epoch() to adapt to system UTC time in microseconds       */
    static uint64_t day_millisec;    /*!< Number of milliseconds in a day                                             */
    static uint64_t day_microsec;    /*!< Number of microseconds in a day                                             */
};  

} // namespace ModGen

#endif  /* TIME_UTIL_MODELGENERATOR */
