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
	for (int i = 0; i < int(m_elements.size()); ++i){
		I_ASSERT(m_elements[i] >= 0);

		if (m_elements[i] >= boundaries.m_elements[i]){
			return false;
		}
	}

	return true;
}


bool CVarIndex::Increase(const CVarIndex& boundaries)
{
	I_ASSERT(IsInside(boundaries));

	for (int i = 0; i < int(m_elements.size()); ++i){
		if (m_elements[i] < boundaries.m_elements[i] - 1){
			m_elements[i]++;
			for (int j = 0; j < i; ++j){
				m_elements[j] = 0;
			}

			return true;
		}
	}

	SetAllTo(0);

	return false;
}


bool CVarIndex::Decrease(const CVarIndex& boundaries)
{
	I_ASSERT(IsInside(boundaries));

	for (int i = 0; i < int(m_elements.size()); ++i){
		if (m_elements[i] > 0){
			m_elements[i]--;

			for (int j = 0; j < i; ++j){
				m_elements[j] = boundaries.m_elements[j] - 1;
			}

			return true;
		}
	}

	for (int j = 0; j < int(m_elements.size()); ++j){
		m_elements[j] = boundaries.m_elements[j] - 1;
	}

	return false;
}


bool CVarIndex::operator==(const CVarIndex& index) const
{
	for (int i = 0; i < int(m_elements.size()); ++i){
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


