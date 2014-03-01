//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWEC_UTILS_HPP
#define TWEC_UTILS_HPP


#include <mlk/tools/stl_string_utl.h>


namespace twec
{
	template<typename Ms_Type>
	constexpr Ms_Type ms_from_time(Ms_Type h = 0, Ms_Type m = 0, Ms_Type s = 0)
	{return (h * 3600 + m * 60 + s) * 1000;}

	template<typename Ms_Type>
	std::string timestr_from_ms(Ms_Type ms)
	{return mlk::stl_string::str_format("%%:%%:%%", (ms / (1000 * 3600)) % 24, (ms / (1000 * 60)) % 60, (ms / 1000) % 60);}
}


#endif // TWEC_UTILS_HPP
