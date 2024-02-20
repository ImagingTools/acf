#pragma once


// ACF includes
#include <icmm/IColorantList.h>
#include <icmm/IColorModel.h>


namespace icmm
{


class CSubstractiveColorModel: virtual public icmm::IColorModel, virtual public icmm::IColorantList
{
public:
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
};


} // namespace icmm


