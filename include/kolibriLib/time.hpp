#ifndef __TIME_HPP__
#define __TIME_HPP__

#include <ctime>
#include <include_ksys.h>

namespace KolibriLib
{
	ksys_time_bcd_t to_bcd_time(ksys_ftime_t t);

	ksys_date_bcd_t to_bcd_date(ksys_fdate_t t);

	std::tm to_tm(ksys_ftime_t t, ksys_fdate_t d);
	std::tm to_tm(ksys_time_bcd_t t, ksys_date_bcd_t d);

} // namespace KolibriLib


#endif // __TIME_HPP__
