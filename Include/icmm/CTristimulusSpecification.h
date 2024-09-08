#pragma once


// ACF includes
#include <icmm/ITristimulusSpecification.h>
#include <icmm/ISpectralColorSpecification.h>
#include <icmm/CIlluminant.h>


namespace icmm
{


class CTristimulusSpecification: virtual public ITristimulusSpecification
{
public:
	CTristimulusSpecification(
				ObserverType observerType,
				AstmTableType method,
				std::shared_ptr<IIlluminant> illuminantPtr,
				std::shared_ptr<ISpectralColorSpecification> baseSpec = nullptr);

	CTristimulusSpecification(const ITristimulusSpecification& other);

	// reimplemented (ITristimulusSpecification)
	virtual std::shared_ptr<IIlluminant> GetIlluminant() const override;
	virtual ObserverType GetObserverType() const override;
	virtual AstmTableType GetMethod() const override;
	virtual std::shared_ptr<ISpectralColorSpecification> GetBaseSpecification() const override;

	// reimplemented (istd::IChangeable)
	virtual bool IsEqual(const istd::IChangeable& other) const override;

private:
	ObserverType m_observerType;
	AstmTableType m_method;
	std::shared_ptr<IIlluminant> m_illuminant;
	std::shared_ptr<ISpectralColorSpecification> m_baseSpec;
};


} // namespace icmm


