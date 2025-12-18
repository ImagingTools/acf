#include <ilog/CTracingConfigurationComp.h>


// ACF includes
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <istd/CChangeNotifier.h>


namespace ilog
{

// public methods

CTracingConfigurationComp::CTracingConfigurationComp(): m_tracingLevel(-1)
{

}


// reimplemented (ilog::ITracingConfiguration)
int CTracingConfigurationComp::GetTracingLevel() const
{
	return m_tracingLevel;
}


void CTracingConfigurationComp::SetTracingLevel(int tracingLevel)
{
	m_tracingLevel = tracingLevel;
}


// reimplemented (iser::ISerializable)
bool CTracingConfigurationComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag tacingLevelTag("TacingLevel", "Level of tracing messages", iser::CArchiveTag::TT_LEAF);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);
	Q_UNUSED(notifier);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(tacingLevelTag);
	retVal = retVal && archive.Process(m_tracingLevel);
	retVal = retVal && archive.EndTag(tacingLevelTag);

	return retVal;
}

// protected methods
// reimplemented (icomp::CComponentBase)
void CTracingConfigurationComp::OnComponentCreated()
{
	if (m_defaultTracingLevel.IsValid()){
		m_tracingLevel = *m_defaultTracingLevel;
	}
}



} // namespace ilog

