#include "icmm/CVarColor.h"


#include "iser/CArchiveTag.h"


namespace icmm
{


CVarColor::CVarColor()
{
}


CVarColor::CVarColor(int componentsCount, double value)
:	BaseClass(componentsCount, value)
{
}


CVarColor::CVarColor(const CVarColor& color)
:	BaseClass(color)
{
}


void CVarColor::GetRounded(const imath::IDoubleManip& manipulator, CVarColor& result)
{
	int elementsCount = GetElementsCount();

	result.SetElementsCount(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		result.m_elements[i] = manipulator.GetRounded(m_elements[i]);
	}
}


bool CVarColor::IsRoundedEqual(const CVarColor& color, const imath::IDoubleManip& manipulator) const
{
	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		if (!manipulator.IsEqual(m_elements[i], color.m_elements[i])){
			return false;
		}
	}

	return true;
}


void CVarColor::Normalize()
{
	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		if (m_elements[i] < 0.0){
			m_elements[i] = 0.0;
		}

		if (m_elements[i] > 1.0){
			m_elements[i] = 1.0;
		}
	}
}


void CVarColor::GetNormalized(CVarColor& result) const
{
	result = *this;

	result.Normalize();
}


// reimplemented (iser::ISerializable)

bool CVarColor::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag colorComponentsTag("ColorComponents", "List of color components");
	static iser::CArchiveTag componentTag("Component", "Single component");

	int elementsCount = GetElementsCount();

	retVal = retVal && archive.BeginMultiTag(colorComponentsTag, componentTag, elementsCount);

	if (!retVal){
		return false;
	}

	if (!archive.IsStoring()){
		m_elements.resize(elementsCount);
	}

    for (int i = 0; i < elementsCount; ++i){
		retVal = retVal && archive.BeginTag(componentTag);
		retVal = retVal && archive.Process(m_elements[i]);
		retVal = retVal && archive.EndTag(componentTag);
	}

	retVal = retVal && archive.EndTag(colorComponentsTag);

	return retVal;
}


const CVarColor& CVarColor::operator=(const CVarColor& color)
{
	int elementsCount = color.GetElementsCount();

	SetElementsCount(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] = color.m_elements[i];
	}

	return *this;
}


const CVarColor& CVarColor::operator+=(const CVarColor& color)
{
	int elementsCount = std::min(GetElementsCount(), color.GetElementsCount());
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] += color.m_elements[i];
	}

	return *this;
}


const CVarColor& CVarColor::operator-=(const CVarColor& color)
{
	int elementsCount = std::min(GetElementsCount(), color.GetElementsCount());
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] -= color.m_elements[i];
	}

	return *this;
}


const CVarColor& CVarColor::operator*=(const CVarColor& color)
{
	int elementsCount = std::min(GetElementsCount(), color.GetElementsCount());
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] *= color.m_elements[i];
	}

	return *this;
}


const CVarColor& CVarColor::operator/=(const CVarColor& color)
{
	int elementsCount = std::min(GetElementsCount(), color.GetElementsCount());
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] /= color.m_elements[i];
	}

	return *this;
}


} // namespace icmm


