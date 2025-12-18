#pragma once


// ACF includes
#include <icmm/CRgb.h>
#include <icmm/IColorModel.h>
#include <icmm/IColorObject.h>


namespace icmm
{


class IRgbColor : virtual public IColorObject
{
public:
	virtual const icmm::CRgb& GetRgb() const = 0;
	virtual IColorSpecification::ConstColorSpecPtr GetSpecification() const = 0;

private:
	icmm::CRgb m_Rgb;
	std::shared_ptr<IColorModel> m_modelPtr;
};


} // namespace icmm


