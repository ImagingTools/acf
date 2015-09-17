#include "icmm/CFastColor.h"


// Qt includes
#include<QtCore/QHash>

// ACF includes
#include "iser/CArchiveTag.h"


namespace icmm
{


uint qHash(const icmm::CFastColor& color, uint seed)
{
	quint64 retVal = seed;

	union{
		double value;
		quint64 raw;
	} element;
	element.raw = 0;

	int elementsCount = color.GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		element.value = color[i];

		retVal = (retVal >> 1) ^ (element.raw + 1);
	}

	return uint(retVal);
}


CFastColor::CFastColor(const icmm::CVarColor& color)
{
	m_elementsCount = qMin(int(MAX_ELEMENTS_COUNT), color.GetElementsCount());

	for (int i = 0; i < m_elementsCount; ++i){
		m_elements[i] = color[i];
	}

}


icmm::CLab CFastColor::GetAsLab() const
{
	if (m_elementsCount >= 3){
		return icmm::CLab(
					(m_elements[0] - 0.5) * 2,
					(m_elements[1] - 0.5) * 2,
					(m_elements[2] - 0.5) * 2);
	}
	else{
		return icmm::CLab(0, 0, 0);
	}
}


void CFastColor::SetAsLab(const icmm::CLab& lab)
{
	m_elementsCount = 3;

	m_elements[0] = lab.GetL() * 0.5 + 0.5;
	m_elements[1] = lab.GetA() * 0.5 + 0.5;
	m_elements[2] = lab.GetB() * 0.5 + 0.5;
}


void CFastColor::GetRounded(const imath::IDoubleManip& manipulator, CFastColor& result)
{
	int elementsCount = GetElementsCount();

	result.SetElementsCount(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		result.m_elements[i] = manipulator.GetRounded(m_elements[i]);
	}
}


bool CFastColor::IsRoundedEqual(const CFastColor& color, const imath::IDoubleManip& manipulator) const
{
	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		if (!manipulator.IsEqual(m_elements[i], color.m_elements[i])){
			return false;
		}
	}

	return true;
}


void CFastColor::Normalize()
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


void CFastColor::GetNormalized(CFastColor& result) const
{
	result = *this;

	result.Normalize();
}


// not reimplemented (iser::ISerializable)

bool CFastColor::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag colorComponentsTag("ColorComponents", "List of color components", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag componentTag("Component", "Single component", iser::CArchiveTag::TT_LEAF, &colorComponentsTag);

	int elementsCount = GetElementsCount();

	retVal = retVal && archive.BeginMultiTag(colorComponentsTag, componentTag, elementsCount);

	if (!retVal){
		return false;
	}

	if (!archive.IsStoring()){
		if (!SetElementsCount(elementsCount)){
			return false;
		}
	}

	for (int i = 0; i < elementsCount; ++i){
		retVal = retVal && archive.BeginTag(componentTag);
		retVal = retVal && archive.Process(m_elements[i]);
		retVal = retVal && archive.EndTag(componentTag);
	}

	retVal = retVal && archive.EndTag(colorComponentsTag);

	return retVal;
}


} // namespace icmm


