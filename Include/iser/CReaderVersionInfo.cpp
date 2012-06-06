#include "iser/CReaderVersionInfo.h"


// ACF includes
#include <istd/TChangeNotifier.h>


namespace iser
{


void CReaderVersionInfo::Reset()
{
	istd::CChangeNotifier notifier(this);

	m_versionIdList.clear();
}


bool CReaderVersionInfo::InsertVersionId(int versionId, quint32 versionNumber, const QString& description)
{
	istd::CChangeNotifier notifier(this);

	VersionIdElement& element = m_versionIdList[versionId];
	element.versionNumber = versionNumber;
	element.description = description;

	return true;
}


bool CReaderVersionInfo::RemoveVersionId(int versionId)
{
	istd::CChangeNotifier notifier(this);

	return m_versionIdList.remove(versionId) > 0;
}


// reimplemented (iser::IVersionInfo)

CReaderVersionInfo::VersionIds CReaderVersionInfo::GetVersionIds() const
{
	return m_versionIdList.keys().toSet();
}


bool CReaderVersionInfo::GetVersionNumber(int versionId, quint32& result) const
{
	VersionElements::ConstIterator findIter = m_versionIdList.constFind(versionId);
	if (findIter != m_versionIdList.constEnd()){
		result = findIter.value().versionNumber;

		return true;
	}

	result = 0xffffffff;

	return false;
}


QString CReaderVersionInfo::GetVersionIdDescription(int versionId) const
{
	VersionElements::ConstIterator findIter = m_versionIdList.constFind(versionId);
	if (findIter != m_versionIdList.constEnd()){
		return findIter.value().description;
	}

	return QString();
}


QString CReaderVersionInfo::GetEncodedVersionName(int /*versionId*/, quint32 /*versionNumber*/) const
{
	return "";
}


// reimplemented (istd::IChangeable)

bool CReaderVersionInfo::CopyFrom(const istd::IChangeable& object)
{
	const iser::IVersionInfo* versionInfoPtr = dynamic_cast<const iser::IVersionInfo*>(&object);
	if (versionInfoPtr == NULL){
		return false;
	}

	iser::IVersionInfo::VersionIds ids = versionInfoPtr->GetVersionIds();
	for (		iser::IVersionInfo::VersionIds::const_iterator iter = ids.begin();
				iter != ids.begin();
				++iter){
		int versionId = *iter;

		quint32 versionNumber;
		if (!versionInfoPtr->GetVersionNumber(versionId, versionNumber)){
			continue;
		}

		VersionIdElement& element = m_versionIdList[versionId];
		element.versionNumber = versionNumber;
		element.description = versionInfoPtr->GetVersionIdDescription(versionId);
	}
	return true;
}


} // namespace iser


