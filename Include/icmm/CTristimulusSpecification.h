#pragma once


// ACF includes
#include <icmm/ITristimulusSpecification.h>
#include <icmm/ISpectralColorSpecification.h>
#include <icmm/CIlluminant.h>


namespace icmm
{


class CTristimulusSpecification: virtual public ITristimulusSpecification, virtual public iser::ISerializable
{
public:
	CTristimulusSpecification(
				ObserverType observerType = ObserverType::TwoDegree,
				AstmTableType method = AstmTableType::Unknown,
				std::shared_ptr<IIlluminant> illuminantPtr = nullptr,
				std::shared_ptr<ISpectralColorSpecification> baseSpec = nullptr);

	CTristimulusSpecification(const ITristimulusSpecification& other);

	// reimplemented (ITristimulusSpecification)
	virtual std::shared_ptr<IIlluminant> GetIlluminant() const override;
	virtual ObserverType GetObserverType() const override;
	virtual AstmTableType GetMethod() const override;
	virtual std::shared_ptr<ISpectralColorSpecification> GetBaseSpecification() const override;

	// reimplemented (istd::IChangeable)
	virtual bool IsEqual(const istd::IChangeable& other) const override;
	
	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	bool operator==(const CTristimulusSpecification& other) const;

	// Return instance of commonly used specification
	static const icmm::CTristimulusSpecification& GetD50TwoDegree();

private:
	ObserverType m_observerType;
	AstmTableType m_method;
	std::shared_ptr<IIlluminant> m_illuminantPtr;
	std::shared_ptr<ISpectralColorSpecification> m_baseSpecPtr;
};


} // namespace icmm


