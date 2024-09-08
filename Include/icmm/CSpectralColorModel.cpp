#include <icmm/CSpectralColorModel.h>


namespace icmm
{


// public methods

CSpectralColorModel::CSpectralColorModel(const ISpectralColorSpecification& spec)
{
	m_spec->CopyFrom(spec);
}


// reimplemented (icmm::IColorModel)

IColorModel::ModelType CSpectralColorModel::GetModelType() const
{
	return MI_SPECTRAL;
}


IColorModel::ModelClass CSpectralColorModel::GetModelClass() const
{
	return MC_DEVICE_INDEPENDENT;
}


IColorModel::ColorSpaceClass CSpectralColorModel::GetColorSpaceClass() const
{
	return CSC_PERCEPTUAL;
}


int CSpectralColorModel::GetColorSpaceDimensionality() const
{
	int componentCount = 0;

	const ISpectrumInfo* infoPtr = m_spec->GetSpectrumInfo();
	if (infoPtr != nullptr){
		int step = infoPtr->GetStep();
		if (step > 0) {
			istd::CIntRange range = infoPtr->GetSpectralRange();

			componentCount = 1 + range.GetLength() / step;
		}
	}

	return componentCount;
}


const imath::IUnitInfo* CSpectralColorModel::GetColorSpaceComponentInfo(int /*componentIndex*/) const
{
	return nullptr;
}


QString CSpectralColorModel::GetColorSpaceComponentName(int /*componentIndex*/) const
{
	return QString();
}


const icmm::IColorTransformation* CSpectralColorModel::CreateColorTranformation(
	const IColorModel& /*otherColorModel*/,
	const QByteArray& /*transformationId*/) const
{
	return nullptr;
}


IColorSpecification::ConstColorSpecPtr CSpectralColorModel::GetSpecification() const
{
	return m_spec;
}


} // namespace icmm


