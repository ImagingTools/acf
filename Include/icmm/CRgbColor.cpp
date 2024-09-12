#include <icmm/CRgbColor.h>


// ACF includes
#include <icmm/CRgb.h>
#include <icmm/CRgbColorModel.h>


namespace icmm
{


CRgbColor::CRgbColor(const icmm::CRgb& rgb, ColorModelPtr modelPtr)
    : m_rgb(rgb),
    m_modelPtr(modelPtr)
{
}

CRgbColor::CRgbColor(const icmm::CRgb& Rgb)
    :m_rgb(Rgb)
{
    m_modelPtr = std::make_shared<icmm::CRgbColorModel>();  // D50 Color Spec
}

void CRgbColor::SetSpecification(const ITristimulusSpecification &spec)
{
    m_modelPtr = std::make_shared<CRgbColorModel>(spec);
}


// reimplemented (icmm::IRgbColor)

const icmm::CRgb& CRgbColor::GetRgb() const
{
    return m_rgb;
}

IColorSpecification::ConstColorSpecPtr CRgbColor::GetSpecification() const
{
	return m_modelPtr->GetSpecification();
}

// reimplemented (icmm::IColorObject)

icmm::CVarColor CRgbColor::GetColor() const
{
	icmm::CVarColor varColor(3);

    varColor.SetElement(0, m_rgb.GetRed());
    varColor.SetElement(1, m_rgb.GetGreen());
    varColor.SetElement(2, m_rgb.GetBlue());

	return varColor;
}


ConstColorModelPtr CRgbColor::GetColorModel() const
{
	return m_modelPtr;
}

std::unique_ptr<IColorObject> CRgbColor::CloneIntoUniquePtr() const
{
    return std::make_unique<CRgbColor>(*this);
}

bool CRgbColor::Serialize(iser::IArchive& /*archive*/)
{
	return false;
}


} // namespace icmm


