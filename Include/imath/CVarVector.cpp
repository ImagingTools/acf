#include "imath/CVarVector.h"


// STL includes
#include <cmath>

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace imath
{


void CVarVector::SetElementsFrom(const CVarVector& vector, double expansionValue)
{
	int elementsCount = GetElementsCount();
	int minElements = istd::Min(elementsCount, vector.GetElementsCount());

	for (int i = 0; i < minElements; ++i){
		SetElement(i, vector[i]);
	}

	for (int i = minElements; i < elementsCount; ++i){
		SetElement(i, expansionValue);
	}
}


double CVarVector::GetElementsSum() const
{
	int elementsCount = GetElementsCount();

	double retVal = 0;

	for (int i = 0; i < elementsCount; ++i){
        retVal += GetElement(i);
    }

	return retVal;
}


bool CVarVector::Normalize(double length)
{
    double isLength = GetLength();

    double proportion = isLength / length;

	if (::fabs(proportion) > I_BIG_EPSILON){
		int elementsCount = GetElementsCount();

        for (int i = 0; i < elementsCount; ++i){
            m_elements[i] = m_elements[i] / proportion;
        }

		return true;
    }
	else{
		return false;
	}
}


void CVarVector::GetMinimal(const CVarVector& vector, CVarVector& result) const
{
	int elementsCount = istd::Min(GetElementsCount(), vector.GetElementsCount());

	result.SetElementsCount(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		result.SetElement(i, istd::Min(GetElement(i), vector.GetElement(i)));
	}
}


void CVarVector::GetMaximal(const CVarVector& vector, CVarVector& result) const
{
	int elementsCount = istd::Min(GetElementsCount(), vector.GetElementsCount());

	result.SetElementsCount(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		result.SetElement(i, istd::Max(GetElement(i), vector.GetElement(i)));
	}
}


bool CVarVector::GetNormalized(CVarVector& result, double length) const
{
    double isLength = GetLength();

    double proportion = isLength / length;

	if (::fabs(proportion) > I_BIG_EPSILON){
		int elementsCount = GetElementsCount();

		result.m_elements.resize(elementsCount);

        for (int i = 0; i < elementsCount; ++i){
            result.m_elements[i] = m_elements[i] / proportion;
        }

		return true;
    }
	else{
		return false;
	}
}


bool CVarVector::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	int elementsCount = GetElementsCount();

	static iser::CArchiveTag elementsTag("Elements", "List of vector element");
	static iser::CArchiveTag elementTag("Element", "Single vector element");

	retVal = retVal && archive.BeginMultiTag(elementsTag, elementTag, elementsCount);

	if (!retVal){
		return false;
	}

	if (!archive.IsStoring()){
		m_elements.resize(elementsCount);
	}

    for (int i = 0; i < elementsCount; ++i){
		retVal = retVal && archive.BeginTag(elementTag);
		retVal = retVal && archive.Process(m_elements[i]);
		retVal = retVal && archive.EndTag(elementTag);
	}

	retVal = retVal && archive.EndTag(elementsTag);

	return retVal;
}


} // namespace imath


