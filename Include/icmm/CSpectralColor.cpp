#include <icmm/CSpectralColor.h>


// ACF includes
#include <icmm/CSpectralColorModel.h>


namespace icmm
{


CSpectralColor::CSpectralColor(ColorModelPtr modelPtr)
{
	m_modelPtr = modelPtr;
}


CSpectralColor::CSpectralColor(const ISpectralColorSpecification& spec)
{
	m_modelPtr = std::make_shared<icmm::CSpectralColorModel>(spec);
}



bool CSpectralColor::SetColor(const CVarColor& otherSpectrals)
{
	if(m_modelPtr->GetColorSpaceDimensionality() != otherSpectrals.GetElementsCount()) {
		return false;
	}

	m_spectrumValues.CopyFrom(otherSpectrals);

	return true;
}


// reimplemented (icmm::IColorObject)

icmm::CVarColor CSpectralColor::GetColor() const
{
	return m_spectrumValues;
}


ConstColorModelPtr CSpectralColor::GetColorModel() const
{
	return m_modelPtr;
}


bool CSpectralColor::Serialize(iser::IArchive& /*archive*/)
{
	return false;
}


} // namespace icmm


