#pragma once


// ACF includes
#include <icmm/ICieLabColor.h>
#include <icmm/CCieLabColorModel.h>


namespace icmm
{


class CCieLabColor: virtual public ICieLabColor
{
public:
	explicit CCieLabColor(const CCieLabColorModel* modelPtr);
	CCieLabColor(const icmm::CLab& lab, const ITristimulusSpecification& spec);
	CCieLabColor(const CCieLabColor& color);

	// reimplemented (icmm::ICieLabColor)
	virtual bool Initialize(const icmm::CLab& value, const ITristimulusSpecification& spec) override;
	virtual const icmm::CLab& GetLab() const override;
	virtual IColorSpecification::ConstColorSpecPtr GetSpecification() const override;

	// reimplemented (icmm::IColorObject)
	virtual icmm::CVarColor GetColor() const override;
	virtual ConstColorModelPtr GetColorModel() const override;
	virtual std::unique_ptr<IColorObject> CloneIntoUniquePtr() const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	icmm::CLab m_lab;
	std::shared_ptr<CCieLabColorModel> m_modelPtr;
};


} // namespace icmm


