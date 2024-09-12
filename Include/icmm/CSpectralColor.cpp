#include <icmm/CSpectralColor.h>


// ACF includes
#include <icmm/CSpectralColorModel.h>


namespace icmm
{


CSpectralColor::CSpectralColor(ColorModelPtr modelPtr)
{
	m_modelPtr = modelPtr;
}


CSpectralColor::CSpectralColor(std::shared_ptr<ISpectralColorSpecification> spec)
{
	if (spec == nullptr) {
		m_modelPtr = nullptr;
		return;
	}

	m_modelPtr = std::make_shared<icmm::CSpectralColorModel>(spec);
}


bool CSpectralColor::SetColor(const CVarColor& otherSpectrals)
{
	if (m_modelPtr == nullptr || m_modelPtr->GetColorSpaceDimensionality() != otherSpectrals.GetElementsCount()) {
		return false;
	}

	m_spectrumValues = otherSpectrals;

	return true;
}

ISpectralColorSpecification::ConstSpectralColorSpecPtr CSpectralColor::GetSpecification() const
{
	auto spectralColorDef = std::dynamic_pointer_cast<const ISpectralColorSpecification>(m_modelPtr->GetSpecification());
	return spectralColorDef;
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

std::unique_ptr<IColorObject> CSpectralColor::CloneIntoUniquePtr() const
{
	return std::make_unique<CSpectralColor>(*this);
}

bool CSpectralColor::Serialize(iser::IArchive& /*archive*/)
{
	return false;
}


} // namespace icmm


