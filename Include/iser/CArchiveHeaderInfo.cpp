#include <iser/CArchiveHeaderInfo.h>


// ACF includes
#include <istd/istd.h>
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace iser
{


// local variables
static const istd::IChangeable::ChangeSet versionChangeIds(CArchiveHeaderInfo::CF_VERSIONS_UPDATED);
static const istd::IChangeable::ChangeSet allDataChangeIds(CArchiveHeaderInfo::CF_VERSIONS_UPDATED, istd::IChangeable::CF_ALL_DATA);

static const CArchiveTag s_headerTag("AcfHeader", "Header of ACF archive", CArchiveTag::TT_GROUP);
static const CArchiveTag s_versionInfosTag("VersionInfos", "List of version infos", CArchiveTag::TT_MULTIPLE, &s_headerTag);
static const CArchiveTag s_versionInfoTag("Version", "Version info", CArchiveTag::TT_GROUP, &s_versionInfosTag);
static const CArchiveTag s_versionIdTag("Id", "Version ID", CArchiveTag::TT_LEAF, &s_versionInfoTag);
static const CArchiveTag s_versionNumberTag("Number", "Version number", CArchiveTag::TT_LEAF, &s_versionInfoTag);
static const CArchiveTag s_versionDescriptionTag("Description", "Version description", CArchiveTag::TT_LEAF, &s_versionInfoTag);


void CArchiveHeaderInfo::Reset()
{
	istd::CChangeNotifier notifier(this, &versionChangeIds);
	Q_UNUSED(notifier);

	m_versionIdList.clear();
}


bool CArchiveHeaderInfo::InsertVersionId(int versionId, quint32 versionNumber, const QString& description)
{
	istd::CChangeNotifier notifier(this, &versionChangeIds);
	Q_UNUSED(notifier);

	VersionIdElement& element = m_versionIdList[versionId];
	element.versionNumber = versionNumber;
	element.description = description;

	return true;
}


bool CArchiveHeaderInfo::RemoveVersionId(int versionId)
{
	istd::CChangeNotifier notifier(this, &versionChangeIds);
	Q_UNUSED(notifier);

	return m_versionIdList.remove(versionId) > 0;
}


bool CArchiveHeaderInfo::SerializeArchiveHeader(IArchive& archive)
{
	if (archive.IsStoring()){
		return WriteArchiveHeader(archive, this);
	}

	bool retVal = archive.BeginTag(s_headerTag);

	int versionIdsCount = 0;
	retVal = retVal && archive.BeginMultiTag(s_versionInfosTag, s_versionInfoTag, versionIdsCount);

	if (!retVal){
		return false;
	}
	for (int i = 0; i < versionIdsCount; ++i){
		retVal = retVal && archive.BeginTag(s_versionInfoTag);

		int id = 0;
		retVal = retVal && archive.BeginTag(s_versionIdTag);
		retVal = retVal && archive.Process(id);
		retVal = retVal && archive.EndTag(s_versionIdTag);

		quint32 version = 0;
		retVal = retVal && archive.BeginTag(s_versionNumberTag);
		retVal = retVal && archive.Process(version);
		retVal = retVal && archive.EndTag(s_versionNumberTag);
	
		QString description;
		retVal = retVal && archive.BeginTag(s_versionDescriptionTag);
		retVal = retVal && archive.Process(description);
		retVal = retVal && archive.EndTag(s_versionDescriptionTag);

		if (!retVal){
			return false;
		}

		InsertVersionId(id, version, description);

		retVal = retVal && archive.EndTag(s_versionInfoTag);
	}

	retVal = retVal && archive.EndTag(s_versionInfosTag);

	retVal = retVal && archive.EndTag(s_headerTag);

	return retVal;
}


bool CArchiveHeaderInfo::WriteArchiveHeader(IArchive& archive, const IVersionInfo* versionInfoPtr)
{
	Q_ASSERT(archive.IsStoring());
	if (!archive.IsStoring()){
		return false;
	}

	bool retVal = archive.BeginTag(s_headerTag);

	IVersionInfo::VersionIds ids;

	if (versionInfoPtr != NULL){
		ids = versionInfoPtr->GetVersionIds();
	}

	int versionIdsCount = int(ids.size());

	retVal = retVal && archive.BeginMultiTag(s_versionInfosTag, s_versionInfoTag, versionIdsCount);

	QList<int> sortedIds(ids.begin(), ids.end());
	std::sort(sortedIds.begin(), sortedIds.end());

	for (		QList<int>::iterator iter = sortedIds.begin();
				retVal && (iter != sortedIds.end());
				++iter){
		retVal = retVal && archive.BeginTag(s_versionInfoTag);

		Q_ASSERT(versionInfoPtr != NULL);

		int id = *iter;
		I_IF_DEBUG(
			quint32 dummyVersion;
			Q_ASSERT(versionInfoPtr->GetVersionNumber(id, dummyVersion)) // all known IDs must have its version.
		);

		retVal = retVal && archive.BeginTag(s_versionIdTag);
		retVal = retVal && archive.Process(id);
		retVal = retVal && archive.EndTag(s_versionIdTag);

		quint32 versionNumber;
		versionInfoPtr->GetVersionNumber(id, versionNumber);
		retVal = retVal && archive.BeginTag(s_versionNumberTag);
		retVal = retVal && archive.Process(versionNumber);
		retVal = retVal && archive.EndTag(s_versionNumberTag);

		QString description = versionInfoPtr->GetVersionIdDescription(id);
		retVal = retVal && archive.BeginTag(s_versionDescriptionTag);
		retVal = retVal && archive.Process(description);
		retVal = retVal && archive.EndTag(s_versionDescriptionTag);
	
		retVal = retVal && archive.EndTag(s_versionInfoTag);
	}

	retVal = retVal && archive.EndTag(s_versionInfosTag);

	retVal = retVal && archive.EndTag(s_headerTag);

	return retVal;
}


// reimplemented (iser::IVersionInfo)

CArchiveHeaderInfo::VersionIds CArchiveHeaderInfo::GetVersionIds() const
{
#if QT_VERSION >= 0x060000
	QList<int> keys  = m_versionIdList.keys();

	return QSet<int>(keys.begin(), keys.end());
#else
	return m_versionIdList.keys().toSet();
#endif
}


bool CArchiveHeaderInfo::GetVersionNumber(int versionId, quint32& result) const
{
	VersionElements::ConstIterator findIter = m_versionIdList.constFind(versionId);
	if (findIter != m_versionIdList.constEnd()){
		result = findIter.value().versionNumber;

		return true;
	}

	result = 0xffffffff;

	return false;
}


QString CArchiveHeaderInfo::GetVersionIdDescription(int versionId) const
{
	VersionElements::ConstIterator findIter = m_versionIdList.constFind(versionId);
	if (findIter != m_versionIdList.constEnd()){
		return findIter.value().description;
	}

	return QString();
}


QString CArchiveHeaderInfo::GetEncodedVersionName(int /*versionId*/, quint32 /*versionNumber*/) const
{
	return "";
}


// reimplemented (istd::IChangeable)

bool CArchiveHeaderInfo::CopyFrom(const istd::IChangeable& object, CompatibilityMode /*mode*/)
{
	const IVersionInfo* versionInfoPtr = dynamic_cast<const IVersionInfo*>(&object);
	if (versionInfoPtr == NULL){
		return false;
	}

	istd::CChangeNotifier notifier(this, &allDataChangeIds);
	Q_UNUSED(notifier);

	IVersionInfo::VersionIds ids = versionInfoPtr->GetVersionIds();
	for (		IVersionInfo::VersionIds::const_iterator iter = ids.begin();
				iter != ids.end();
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


