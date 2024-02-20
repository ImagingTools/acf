#include <icmm/CSubstractiveColorModel.h>


namespace icmm
{


// public methods

// reimplemented (icmm::IColorModel)

IColorModel::ModelType CSubstractiveColorModel::GetModelType() const
{
	return MT_COLORANTS;
}


IColorModel::ModelClass CSubstractiveColorModel::GetModelClass() const
{
	return MC_DEVICE_DEPENDENT;
}


IColorModel::ColorSpaceClass CSubstractiveColorModel::GetColorSpaceClass() const
{
	return CSC_SUBSRACTIVE;
}


int CSubstractiveColorModel::GetColorSpaceDimensionality() const
{
	return GetColorantIds().count();
}


const imath::IUnitInfo* CSubstractiveColorModel::GetColorSpaceComponentInfo(int /*componentIndex*/) const
{
	return nullptr;
}


QString CSubstractiveColorModel::GetColorSpaceComponentName(int componentIndex) const
{
	QByteArrayList colorantIds = GetColorantIds();

	Q_ASSERT(componentIndex < colorantIds.count());
	Q_ASSERT(componentIndex >= 0);

	return GetColorantName(colorantIds[componentIndex]);
}


const icmm::IColorTransformation* CSubstractiveColorModel::CreateColorTranformation(
	const IColorModel& /*otherColorModel*/,
	const QByteArray& /*transformationId*/) const
{
	return nullptr;
}


} // namespace icmm


