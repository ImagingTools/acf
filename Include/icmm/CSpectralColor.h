#pragma once


// ACF includes
#include <icmm/IColorObject.h>
#include <icmm/ISpectralColorSpecification.h>


namespace icmm
{


class CSpectralColor: virtual public IColorObject
{
public:
	CSpectralColor(ColorModelPtr modelPtr);
	CSpectralColor(std::shared_ptr<ISpectralColorSpecification> spec);

	bool SetColor(const icmm::CVarColor& values);
	ISpectralColorSpecification::ConstSpectralColorSpecPtr GetSpecification() const;

	// reimplemented (icmm::IColorObject)
	virtual icmm::CVarColor GetColor() const override;	
	virtual ConstColorModelPtr GetColorModel() const override;
	virtual std::unique_ptr<IColorObject> CloneIntoUniquePtr() const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	icmm::CVarColor m_spectrumValues;
	std::shared_ptr<IColorModel> m_modelPtr;
};


} // namespace icmm


