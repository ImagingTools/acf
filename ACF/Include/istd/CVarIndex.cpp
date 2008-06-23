#include "istd/CVarIndex.h"


namespace istd
{


// public methods

CVarIndex::CVarIndex(int size, int value)
:	m_elements(size)
{
	SetAllTo(value);
}


CVarIndex::CVarIndex()
{
}


CVarIndex::CVarIndex(const CVarIndex& index)
:	m_elements(index.m_elements)
{
}


bool CVarIndex::IsZero() const
{
	for (int i = 0; i < int(m_elements.size()); ++i){
		if (m_elements[i] > 0){
			return false;
		}
	}

	return true;
}


void CVarIndex::Reset()
{
	m_elements.clear();
}


void CVarIndex::Clear()
{
	SetAllTo(0);
}


void CVarIndex::SetAllTo(int value)
{
	for (int i = 0; i < int(m_elements.size()); ++i){
		m_elements[i] = value;
	}
}


bool CVarIndex::IsInside(const CVarIndex& boundaries) const
{
	int dimensionsCount = GetMinDimensionsCount();
	int boundariesCount = boundaries.GetDimensionsCount();
	if ((boundariesCount <= 0) || (boundariesCount < dimensionsCount)){
		return false;
	}

	for (int commonIndex = 0; commonIndex < dimensionsCount; ++commonIndex){
		I_ASSERT(m_elements[commonIndex] >= 0);

		if (m_elements[commonIndex] >= boundaries.m_elements[commonIndex]){
			return false;
		}
	}

	for (int restIndex = dimensionsCount; restIndex < boundariesCount; ++restIndex){
		if (boundaries.m_elements[restIndex] <= 0){
			return false;
		}
	}

	return true;
}


bool CVarIndex::Increase(const CVarIndex& boundaries)
{
	I_ASSERT(IsInside(boundaries));

	int dimensionsCount = istd::Min(GetDimensionsCount(), boundaries.GetDimensionsCount());

	for (int i = 0; i < dimensionsCount; ++i){
		if (m_elements[i] < boundaries.m_elements[i] - 1){
			m_elements[i]++;
			for (int j = 0; j < i; ++j){
				m_elements[j] = 0;
			}

			return true;
		}
	}

	return false;
}


bool CVarIndex::Decrease(const CVarIndex& boundaries)
{
	I_ASSERT(IsInside(boundaries));

	int dimensionsCount = istd::Min(GetDimensionsCount(), boundaries.GetDimensionsCount());

	for (int i = 0; i < dimensionsCount; ++i){
		if (m_elements[i] > 0){
			m_elements[i]--;

			for (int j = 0; j < i; ++j){
				m_elements[j] = boundaries.m_elements[j] - 1;
			}

			return true;
		}
	}

	return false;
}


bool CVarIndex::operator==(const CVarIndex& index) const
{
	int minDimensionsCount = GetMinDimensionsCount();
	if (minDimensionsCount != index.GetMinDimensionsCount()){
		return false;
	}

	for (int i = 0; i < minDimensionsCount; ++i){
		if (m_elements[i] != index.m_elements[i]){
			return false;
		}
	}

	return true;
}


bool CVarIndex::operator!=(const CVarIndex& index) const
{
	return !operator==(index);
}


} // namespace istd


