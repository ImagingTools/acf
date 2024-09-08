#pragma once


// ACF includes
#include <icmm/IColorObject.h>
#include <icmm/CLab.h>


namespace icmm
{


class ICieLabColor: virtual public IColorObject
{
public:
	virtual const icmm::CLab& GetLab() const = 0;

	virtual IColorSpecification::ConstColorSpecPtr GetSpecification() const = 0;
};


} // namespace icmm


