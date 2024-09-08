#pragma once


// ACF includes
#include <icmm/IColorModel.h>
#include <icmm/CTristimulusSpecification.h>


namespace icmm
{


class CCieLabColorModel: virtual public icmm::IColorModel
{
public:
	CCieLabColorModel(const ITristimulusSpecification& spec);

	// reimplemented (icmm::IColorModel)
	virtual ModelType GetModelType() const override;
	virtual ModelClass GetModelClass() const override;
	virtual ColorSpaceClass GetColorSpaceClass() const override;
	virtual int GetColorSpaceDimensionality() const override;
	virtual const imath::IUnitInfo* GetColorSpaceComponentInfo(int componentIndex) const override;
	virtual QString GetColorSpaceComponentName(int componentIndex) const override;
	virtual const icmm::IColorTransformation* CreateColorTranformation(
				const IColorModel& otherColorModel,
				const QByteArray& transformationId = QByteArray()) const override;
	virtual IColorSpecification::ConstColorSpecPtr GetSpecification() const override;

private:
	CTristimulusSpecification m_spec;
};


} // namespace icmm


