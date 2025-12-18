#pragma once


// ACF includes
#include <istd/IChangeable.h>
#include <icmm/CVarColor.h>


namespace icmm
{


class IWhitePointProvider: virtual public istd::IChangeable
{
public:
	virtual icmm::CVarColor GetWhitePoint() const = 0;
};


} // namespace icmm


