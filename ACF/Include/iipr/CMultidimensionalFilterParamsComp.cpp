#include "iipr/CMultidimensionalFilterParamsComp.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "imath/CDoubleManip.h"


namespace iipr
{


CMultidimensionalFilterParamsComp::CMultidimensionalFilterParamsComp()
{
}


// reimplemented (iipr::IMultidimensionalFilterParams)

const IMultidimensionalFilterConstraints* CMultidimensionalFilterParamsComp::GetConstraints() const
{
	if (m_constraintsCompPtr.IsValid()){
		return m_constraintsCompPtr.GetPtr();
	}

	return this;
}


imath::CVarVector CMultidimensionalFilterParamsComp::GetFilterLengths() const
{
	return m_filterLengts;
}


bool CMultidimensionalFilterParamsComp::SetFilterLengths(const imath::CVarVector& lengths)
{
	if (m_filterLengts != lengths){
		const IMultidimensionalFilterConstraints* constraintsPtr = GetConstraints();
		I_ASSERT(constraintsPtr != NULL);
		if (lengths.GetElementsCount() != constraintsPtr->GetFilterDimensionsCount()){
			return false;
		}

		istd::CChangeNotifier notifier(this);

		m_filterLengts = lengths;
	}

	return true;
}


// reimplemented (iipr::IMultidimensionalFilterConstraints)

int CMultidimensionalFilterParamsComp::GetFilterDimensionsCount() const
{
	return *m_dimensionsCountAttrPtr;
}


istd::CRange CMultidimensionalFilterParamsComp::GetFilterLengthRange(int /*dimension*/) const
{
	return istd::CRange(*m_minFilterLengthAttrPtr, *m_maxFilterLengthAttrPtr);
}


const imath::IDoubleManip& CMultidimensionalFilterParamsComp::GetFilterLengthManip(int /*dimension*/) const
{
	static imath::CDoubleManip manip;

	return manip;
}


// reimplemented (iser::ISerializable)

bool CMultidimensionalFilterParamsComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag filterLengthsTag("FilterLengts", "List of filter lengths for each dimension");

	bool retVal = archive.BeginTag(filterLengthsTag);
	retVal = retVal && m_filterLengts.Serialize(archive);
	retVal = retVal && archive.EndTag(filterLengthsTag);

	return retVal;
}


// reimplemented (icomp::IComponent)

void CMultidimensionalFilterParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	const IMultidimensionalFilterConstraints* constraintsPtr = GetConstraints();
	I_ASSERT(constraintsPtr != NULL);

	int count = constraintsPtr->GetFilterDimensionsCount();
	m_filterLengts.SetElementsCount(count, 1);

	if (m_filterLengthsAttrPtr.IsValid()){
		int commonCount = istd::Min(count, m_filterLengthsAttrPtr.GetCount());
		for (int i = 0; i < commonCount; ++i){
			m_filterLengts[i] = m_filterLengthsAttrPtr[i];
		}
	}
}


} // namespace iipr

