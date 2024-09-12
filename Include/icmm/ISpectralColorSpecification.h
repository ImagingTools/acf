#pragma once


// ACF includes
#include <icmm/IColorSpecification.h>
#include <icmm/ISpectrumInfoProvider.h>


namespace icmm
{


class ISpectralColorSpecification: virtual public IColorSpecification, virtual public ISpectrumInfoProvider
{
public:
	typedef std::shared_ptr<const ISpectralColorSpecification> ConstSpectralColorSpecPtr;
	typedef std::shared_ptr<ISpectralColorSpecification> SpectralColorSpecPtr;

	enum SpectrumType
	{
		Reflective = 1,
		Emissive,
		Transmissive,
		TotalTransmissive
	};

	virtual SpectrumType GetSpectrumType() const = 0;

protected:
	// reimplemented (IColorSpecification)
	virtual SpecType GetSpecificationType() const override;
};


inline IColorSpecification::SpecType ISpectralColorSpecification::GetSpecificationType() const
{
	return SpecType::Spectral;
}


} // namespace icmm


