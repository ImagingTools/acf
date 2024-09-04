#include <icmm/CCieLabColor.h>


// ACF includes
#include <icmm/CCieLabColorModel.h>


namespace icmm
{


CCieLabColor::CCieLabColor(ColorModelPtr modelPtr)
{
	m_modelPtr = modelPtr;
}


CCieLabColor::CCieLabColor(const icmm::CLab& lab, const ITristimulusSpecification& spec)
	:m_lab(lab)
{
	m_modelPtr = std::make_shared<icmm::CCieLabColorModel>(spec);
}


// reimplemented (icmm::ICieLabColor)

const icmm::CLab& CCieLabColor::GetLab() const
{
	return m_lab;
}


// reimplemented (icmm::IColorObject)

icmm::CVarColor CCieLabColor::GetColor() const
{
	icmm::CVarColor varColor(3);

	varColor.SetElement(0, m_lab.GetL());
	varColor.SetElement(1, m_lab.GetA());
	varColor.SetElement(2, m_lab.GetB());

	return varColor;
}


ConstColorModelPtr CCieLabColor::GetColorModel() const
{
	return m_modelPtr;
}


bool CCieLabColor::Serialize(iser::IArchive& /*archive*/)
{
	return false;
}


} // namespace icmm


