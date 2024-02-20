#pragma once


// ACF includes
#include <imath/CGeneralUnitInfo.h>
#include <icmm/IColorModel.h>


namespace icmm
{


class CRgbColorModel: virtual public IColorModel
{
public:
	CRgbColorModel();

	// reimplemented (IColorModel)
	virtual ModelType GetModelType() const override;
	virtual ModelClass GetModelClass() const override;
	virtual ColorSpaceClass GetColorSpaceClass() const override;
	virtual int GetColorSpaceDimensionality() const override;
	virtual const imath::IUnitInfo* GetColorSpaceComponentInfo(int componentIndex) const override;
	virtual QString GetColorSpaceComponentName(int componentIndex) const override;
	virtual const icmm::IColorTransformation* CreateColorTranformation(const IColorModel& otherColorModel, const QByteArray& transformationId) const override;

protected:
	imath::CGeneralUnitInfo m_unitInfo;
};


} // namespace icmm


