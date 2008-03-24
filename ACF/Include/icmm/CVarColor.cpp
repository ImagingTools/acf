#include "icmm/CVarColor.h"


namespace icmm
{


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


} // namespace icmm


