#pragma once


// ACF includes
#include <icmm/CTristimulusSpecification.h>
#include <icmm/IColorModel.h>
#include <imath/CGeneralUnitInfo.h>


namespace icmm
{


class CRgbColorModel: virtual public IColorModel
{
public:
	CRgbColorModel();

	/**
		Sets a particular TristimulusSpecification,
		for example, if the RGB color is originating from Lab
	*/
	CRgbColorModel(const ITristimulusSpecification& spec);

	// reimplemented (IColorModel)
	virtual ModelType GetModelType() const override;
	virtual ModelClass GetModelClass() const override;
	virtual ColorSpaceClass GetColorSpaceClass() const override;
	virtual int GetColorSpaceDimensionality() const override;
	virtual const imath::IUnitInfo* GetColorSpaceComponentInfo(int componentIndex) const override;
	virtual QString GetColorSpaceComponentName(int componentIndex) const override;
	virtual const icmm::IColorTransformation* CreateColorTranformation(const IColorModel& otherColorModel, const QByteArray& transformationId) const override;
	virtual IColorSpecification::ConstColorSpecPtr GetSpecification() const override;

protected:
	imath::CGeneralUnitInfo m_unitInfo;
	CTristimulusSpecification m_spec;
};


} // namespace icmm


