#include <imath/CVarVector.h>


// Qt includes
#include <QtCore/QtGlobal>

// ACF includes
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace imath
{


void CVarVector::SetElementsFrom(const CVarVector& vector, double expansionValue)
{
	int elementsCount = GetElementsCount();
	int minElements = qMin(elementsCount, vector.GetElementsCount());

	for (int i = 0; i < minElements; ++i){
		SetElement(i, vector[i]);
	}

	for (int i = minElements; i < elementsCount; ++i){
		SetElement(i, expansionValue);
	}
}


bool CVarVector::EnsureElementsCount(int count, double value)
{
	int oldSize = int(m_elements.size());
	if (count > oldSize){
		m_elements.resize(count);

		if (value != 0){
			for (int i = oldSize; i < count; ++i){
				m_elements[i] = value;
			}
		}
	}

	return true;
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

	if (qAbs(proportion) > I_BIG_EPSILON){
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
	int elementsCount = qMin(GetElementsCount(), vector.GetElementsCount());

	result.SetElementsCount(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		result.SetElement(i, qMin(GetElement(i), vector.GetElement(i)));
	}
}


void CVarVector::GetMaximal(const CVarVector& vector, CVarVector& result) const
{
	int elementsCount = qMin(GetElementsCount(), vector.GetElementsCount());

	result.SetElementsCount(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		result.SetElement(i, qMax(GetElement(i), vector.GetElement(i)));
	}
}


bool CVarVector::GetNormalized(CVarVector& result, double length) const
{
	double isLength = GetLength();

	double proportion = isLength / length;

	if (qAbs(proportion) > I_BIG_EPSILON){
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
	static iser::CArchiveTag elementsTag("Elements", "List of vector element", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag elementTag("Element", "Single vector element", iser::CArchiveTag::TT_LEAF, &elementsTag);

	bool isStoring = archive.IsStoring();

	bool retVal = true;

	int elementsCount = GetElementsCount();

	retVal = retVal && archive.BeginMultiTag(elementsTag, elementTag, elementsCount);

	if (!retVal){
		return false;
	}

	if (!isStoring){
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


