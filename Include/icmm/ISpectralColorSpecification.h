#pragma once


// ACF includes
#include <icmm/IColorSpecification.h>
#include <icmm/ISpectrumInfoProvider.h>


namespace icmm
{


class ISpectralColorSpecification: virtual public IColorSpecification, virtual public ISpectrumInfoProvider
{
protected:
	// reimplemented (IColorSpecification)
	virtual SpecType GetSpecificationType() const override;
};


inline IColorSpecification::SpecType ISpectralColorSpecification::GetSpecificationType() const
{
	return SpecType::Spectral;
}


} // namespace icmm


