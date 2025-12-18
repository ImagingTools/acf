#pragma once


// ACF includes
#include <icmm/ISpectralColorSpecification.h>
#include <icmm/IIlluminant.h>


namespace icmm
{


class ITristimulusSpecification: virtual public IColorSpecification
{
public:
	virtual std::shared_ptr<IIlluminant> GetIlluminant() const = 0;
	virtual ObserverType GetObserverType() const = 0;
	virtual AstmTableType GetMethod() const = 0;

	/**
		Get the base spectral specification, if exists.
	*/
	virtual std::shared_ptr<ISpectralColorSpecification> GetBaseSpecification() const = 0;

protected:
	// reimplemented (IColorSpecification)
	virtual SpecType GetSpecificationType() const override;
};


inline IColorSpecification::SpecType ITristimulusSpecification::GetSpecificationType() const
{
	return SpecType::Tristimulus;
}

typedef std::shared_ptr<ITristimulusSpecification> TristimulusPtr;
typedef std::shared_ptr<const ITristimulusSpecification> ConstTristimulusPtr;

} // namespace icmm


