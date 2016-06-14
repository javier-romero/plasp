#ifndef __PLASP__UTILS__FORMATTING_H
#define __PLASP__UTILS__FORMATTING_H

#include <iostream>

#include <plasp/utils/LogStream.h>

namespace plasp
{
namespace utils
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Formatting
//
////////////////////////////////////////////////////////////////////////////////////////////////////

enum class Color
{
	Black = 0,
	Red = 1,
	Green = 2,
	Yellow = 3,
	Blue = 4,
	Magenta = 5,
	Cyan = 6,
	White = 7
};

////////////////////////////////////////////////////////////////////////////////////////////////////

enum class FontWeight
{
	Normal = 0,
	Bold = 1
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Format
{
	Format(Color color, FontWeight fontWeight = FontWeight::Normal)
	:	m_color{color},
		m_fontWeight{fontWeight}
	{
	}

	Color m_color;
	FontWeight m_fontWeight;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &operator<<(LogStream &stream, const Format &format)
{
	if (!stream.supportsColor())
		return stream;

	const auto fontWeightCode = static_cast<size_t>(format.m_fontWeight);
	const auto colorCode = 30 + static_cast<size_t>(format.m_color);

	return (stream << "\033[" << fontWeightCode << ";" << colorCode << "m");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

class ResetFormat
{
};

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &operator<<(LogStream &stream, const ResetFormat &)
{
	if (!stream.supportsColor())
		return stream;

	return (stream << "\033[0m");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
