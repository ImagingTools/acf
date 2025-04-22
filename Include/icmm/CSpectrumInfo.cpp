#include <icmm/CSpectrumInfo.h>


// ACF includes
#include <iser/CArchiveTag.h>
#include <iser/CPrimitiveTypesSerializer.h>
#include <iser/IArchive.h>
#include <istd/CChangeNotifier.h>
#include <istd/TDelPtr.h>


namespace icmm
{


CSpectrumInfo::CSpectrumInfo()
	: m_step(0)
{
}


bool CSpectrumInfo::SetRange(const istd::CIntRange& range)
{
	if (m_step != 0 && range.GetLength() % m_step != 0){
		return false;
	}

	if (m_range != range){
		istd::CChangeNotifier notifier(this);

		m_range = range;
	}

	return true;
}


bool CSpectrumInfo::SetStep(int step)
{
	if (m_range.GetLength() % step != 0){
		return false;
	}

	if (m_step != step){
		istd::CChangeNotifier notifier(this);

		m_step = step;
	}

	return true;
}


bool CSpectrumInfo::operator==(const CSpectrumInfo& other) const
{
	bool retVal = m_range == other.m_range;
	retVal = retVal && m_step == other.m_step;

	return retVal;
}


// reimplemented (icmm::ISpectrumInfo)

istd::CIntRange CSpectrumInfo::GetSpectralRange() const
{
	return m_range;
}


int CSpectrumInfo::GetStep() const
{
	return m_step;
}

int CSpectrumInfo::GetSamplesCount() const
{
	if (m_step == 0){ 
		return 0; 
	}
	
	return m_range.GetLength() / m_step + 1;
}


// reimplemented (iser::ISerializable)

bool CSpectrumInfo::Serialize(iser::IArchive& archive)
{
	istd::CChangeNotifier notifier(!archive.IsStoring() ? this : nullptr);

	bool retVal = iser::CPrimitiveTypesSerializer::SerializeIntRange(archive, m_range);

	iser::CArchiveTag stepTag("StepValue", "Step value", iser::CArchiveTag::TT_LEAF);
	retVal = retVal && archive.BeginTag(stepTag);
	retVal = retVal && archive.Process(m_step);
	retVal = retVal && archive.EndTag(stepTag);

	return retVal;
}


// reimplemented (istd::IChangeable)

int CSpectrumInfo::GetSupportedOperations() const
{
	return SO_CLONE | SO_COMPARE | SO_COPY | SO_RESET;
}


bool CSpectrumInfo::CopyFrom(const IChangeable& object, CompatibilityMode /*mode*/)
{
	const CSpectrumInfo* objectPtr = dynamic_cast<const CSpectrumInfo*>(&object);
	if (objectPtr != nullptr){
		istd::CChangeNotifier notifier(this);

		*this = *objectPtr;

		return true;
	}

	return false;
}


bool CSpectrumInfo::IsEqual(const IChangeable& object) const
{
	const CSpectrumInfo* objectPtr = dynamic_cast<const CSpectrumInfo*>(&object);
	if (objectPtr != nullptr){
		return *this == *objectPtr;
	}

	return false;
}


istd::IChangeable* CSpectrumInfo::CloneMe(CompatibilityMode /*mode*/) const
{
	istd::TDelPtr<CSpectrumInfo> clonePtr(new CSpectrumInfo());

	if (clonePtr->CopyFrom(*this)){
		return clonePtr.PopPtr();
	}

	return nullptr;
}


bool CSpectrumInfo::ResetData(CompatibilityMode /*mode*/)
{
	istd::CChangeNotifier notifier(this);

	m_range.Reset();
	m_step = 0;

	return true;
}


} // namespace icmm
