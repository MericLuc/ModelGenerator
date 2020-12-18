/*!
 * @file   time_util.cpp
 * @brief  Contains the implementation of the strucures defined
 *         in \a time_util.h
 * @author lhm
 * @date   22/07/2019
 */

#include <iomanip>
#include <iostream>

#include "time_util.h"

namespace ModGen {

uint64_t HHR_DIANE_Time::day_microsec = 86400000000;
uint64_t HHR_DIANE_Time::HPF_div      = 10000000;
uint64_t HHR_DIANE_Time::Hpf_div      = 1000;

////////////////////////////////////////////////////////////////////////
HHR_DIANE_Time::HHR_DIANE_Time():
  m_HPF(0),
  m_Hpf(0),
  m_HHR(0)
{}

////////////////////////////////////////////////////////////////////////
HHR_DIANE_Time::HHR_DIANE_Time(uint64_t p_day_ellapsed_us)
{
  uint64_t l_tmp = 0;

  // Check if the value is over 24h
  if( p_day_ellapsed_us > day_microsec ) 
  {
    p_day_ellapsed_us %= day_microsec;
  }

  // Fill the members (HPF, Hpf, HHR)
  m_HPF = p_day_ellapsed_us / HPF_div;
  l_tmp = p_day_ellapsed_us % HPF_div;

  m_Hpf = l_tmp / Hpf_div;
  m_HHR = l_tmp % Hpf_div;

  // Compute the parity bits
  HHR_DIANE_Time::compute_parity(m_HPF);
  HHR_DIANE_Time::compute_parity(m_Hpf);
}

////////////////////////////////////////////////////////////////////////
HHR_DIANE_Time::HHR_DIANE_Time(const HHR_DIANE_Time &p_rhs)
{
  if (this == &p_rhs)
  {
    return;
  }

  m_HPF  = p_rhs.get_HPF();
  m_Hpf  = p_rhs.get_Hpf();
  m_HHR  = p_rhs.get_HHR();
}

////////////////////////////////////////////////////////////////////////
HHR_DIANE_Time& HHR_DIANE_Time::operator=(const HHR_DIANE_Time &p_rhs)
{
  if (this == &p_rhs)
  {
    return *this;
  }

  m_HPF  = p_rhs.get_HPF();
  m_Hpf  = p_rhs.get_Hpf();
  m_HHR  = p_rhs.get_HHR();

  return *this;
}

////////////////////////////////////////////////////////////////////////
bool HHR_DIANE_Time::operator==(const HHR_DIANE_Time &p_rhs)
{
  return (this == &p_rhs);
}

////////////////////////////////////////////////////////////////////////
HHR_DIANE_Time::~HHR_DIANE_Time()
{}

////////////////////////////////////////////////////////////////////////
uint64_t HHR_DIANE_Time::toInt() const
{
  // static_cast pour eviter le warning lorsque
  // l'on effecue un decalage >= taille de l'objet
  // (32 == 8*sizeof(uint16) )
  return ( (static_cast<uint64_t>(m_HPF) << 32) | 
           (static_cast<uint64_t>(m_Hpf) << 16) |
            static_cast<uint64_t>(m_HHR) );
}

////////////////////////////////////////////////////////////////////////
void HHR_DIANE_Time::compute_parity(uint16_t& p_value)
{
  uint16_t l_tmp = 0;

  for(unsigned int i = 0; i < 8 * sizeof(p_value); i++)
  {
      if (p_value & (1 << i)) { l_tmp++; }
  }
    
  if( l_tmp & 1 )
  {
      p_value |= (1 << 15);
  }
}

TimePoint TimeUtil::time_ref       = std::chrono::high_resolution_clock::now();
uint64_t TimeUtil::day_millisec    = 86400000;
uint64_t TimeUtil::day_microsec    = 86400000000;
int64_t  TimeUtil::utc_hours       = 2;
uint64_t TimeUtil::hour_millisec   = 3600000;
uint64_t TimeUtil::utc_coeff       = 7200000;
uint64_t TimeUtil::utc_coeff_micro = 7200000000;

////////////////////////////////////////////////////////////////////////
uint64_t TimeUtil::day_milliseconds()
{
    return (TimeUtil::milliseconds() + utc_coeff)       % day_millisec;
}

////////////////////////////////////////////////////////////////////////
uint64_t TimeUtil::day_microseconds()
{
    return (TimeUtil::microseconds() + utc_coeff_micro) % day_microsec;
}

////////////////////////////////////////////////////////////////////////
uint64_t TimeUtil::milliseconds()
{
    return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>
              (std::chrono::high_resolution_clock::now().time_since_epoch()).count());
}

////////////////////////////////////////////////////////////////////////
uint64_t TimeUtil::microseconds()
{
    return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>
              (std::chrono::high_resolution_clock::now().time_since_epoch()).count());
}

////////////////////////////////////////////////////////////////////////
uint64_t TimeUtil::ellapsed_seconds()
{
  TimePoint l_end = std::chrono::high_resolution_clock::now();
  return  std::chrono::duration_cast<std::chrono::seconds>(l_end-time_ref).count();
}

////////////////////////////////////////////////////////////////////////
uint64_t TimeUtil::ellapsed_milliseconds()
{
  TimePoint l_end = std::chrono::high_resolution_clock::now();
  return  std::chrono::duration_cast<std::chrono::milliseconds>(l_end-time_ref).count();
}

////////////////////////////////////////////////////////////////////////
uint64_t TimeUtil::ellapsed_microseconds()
{
  TimePoint l_end = std::chrono::high_resolution_clock::now();
  return  std::chrono::duration_cast<std::chrono::microseconds>(l_end-time_ref).count();
}

////////////////////////////////////////////////////////////////////////
uint64_t TimeUtil::hhr_diane()
{
  HHR_DIANE_Time l_diane(TimeUtil::day_microseconds());
  return l_diane.toInt();
}

////////////////////////////////////////////////////////////////////////
uint64_t TimeUtil::hhr_diane_since_start()
{
  HHR_DIANE_Time l_diane(TimeUtil::ellapsed_microseconds());
  return l_diane.toInt();
}

////////////////////////////////////////////////////////////////////////
std::string TimeUtil::currentTime()
{
    std::stringstream ss;

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) -
              std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());

    ss << std::put_time(std::localtime(&time), "%Hh:%Mm:%Ss:");
    ss << ms.count() << "ms";

    return ss.str();
}

} // namespace ModGen
