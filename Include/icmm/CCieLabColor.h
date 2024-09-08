#pragma once


// ACF includes
#include <icmm/ICieLabColor.h>
#include <icmm/ITristimulusSpecification.h>


namespace icmm
{


class CCieLabColor: virtual public ICieLabColor
{
public:
	CCieLabColor(ColorModelPtr modelPtr = ColorModelPtr());
	CCieLabColor(const icmm::CLab& lab, const ITristimulusSpecification& spec);

	// reimplemented (icmm::ICieLabColor)
	virtual const icmm::CLab& GetLab() const override;
	virtual IColorSpecification::ConstColorSpecPtr GetSpecification() const override;

	// reimplemented (icmm::IColorObject)
	virtual icmm::CVarColor GetColor() const override;
	virtual ConstColorModelPtr GetColorModel() const override;

	// reimplemented (iser::IObject)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	icmm::CLab m_lab;
	std::shared_ptr<IColorModel> m_modelPtr;
};


} // namespace icmm


