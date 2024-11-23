#include <icmm/CSubstractiveColorModelBase.h>


namespace icmm
{


// public methods

// reimplemented (icmm::IColorModel)

IColorModel::ModelType CSubstractiveColorModelBase::GetModelType() const
{
	return MT_COLORANTS;
}


IColorModel::ModelClass CSubstractiveColorModelBase::GetModelClass() const
{
	return MC_DEVICE_DEPENDENT;
}


IColorModel::ColorSpaceClass CSubstractiveColorModelBase::GetColorSpaceClass() const
{
	return CSC_SUBSTRACTIVE;
}


int CSubstractiveColorModelBase::GetColorSpaceDimensionality() const
{
	return GetColorantIds().count();
}


const imath::IUnitInfo* CSubstractiveColorModelBase::GetColorSpaceComponentInfo(int /*componentIndex*/) const
{
	return nullptr;
}


QString CSubstractiveColorModelBase::GetColorSpaceComponentName(int componentIndex) const
{
	QByteArrayList colorantIds = GetColorantIds();

	Q_ASSERT(componentIndex < colorantIds.count());
	Q_ASSERT(componentIndex >= 0);

	return GetColorantName(colorantIds[componentIndex]);
}


const icmm::IColorTransformation* CSubstractiveColorModelBase::CreateColorTranformation(
	const IColorModel& /*otherColorModel*/,
	const QByteArray& /*transformationId*/) const
{
	return nullptr;
}


} // namespace icmm


