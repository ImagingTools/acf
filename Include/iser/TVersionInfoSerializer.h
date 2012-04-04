#ifndef iser_CVersionInfoBase_included
#define iser_CVersionInfoBase_included


// ACF includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iser
{


/**
	Generic implementation of the version info serializer.

	\ingroup Persistence
*/

template <class VersionInfo>
class TVersionInfoSerializer
{
public:
	static bool ReadVersion(VersionInfo* versionInfoPtr, iser::IArchive& archive);
	static bool WriteVersion(const VersionInfo* versionInfoPtr, iser::IArchive& archive);

protected:
	static CArchiveTag s_headerTag;
	static CArchiveTag s_versionInfosTag;
	static CArchiveTag s_versionInfoTag;
	static CArchiveTag s_versionIdTag;
	static CArchiveTag s_versionNumberTag;
	static CArchiveTag s_versionDescriptionTag;
};


// reimplemented (iser::ISerializable)

template <class VersionInfo>
bool TVersionInfoSerializer<VersionInfo>::ReadVersion(VersionInfo* versionInfoPtr, iser::IArchive& archive)
{
	I_ASSERT(!archive.IsStoring());
	if (archive.IsStoring()){
		return false;
	}

	bool retVal = archive.BeginTag(s_headerTag);

	int versionIdsCount = 0;
	retVal = retVal && archive.BeginMultiTag(s_versionInfosTag, s_versionInfoTag, versionIdsCount);

	if (versionIdsCount > 0 && versionInfoPtr == NULL){
		return false;
	}

	if (retVal){
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

			versionInfoPtr->InsertVersionId(id, version, description);

			retVal = retVal && archive.EndTag(s_versionInfoTag);
		}
	}

	retVal = retVal && archive.EndTag(s_versionInfosTag);

	retVal = retVal && archive.EndTag(s_headerTag);

	return retVal;
}


template <class VersionInfo>
bool TVersionInfoSerializer<VersionInfo>::WriteVersion(const VersionInfo* versionInfoPtr, iser::IArchive& archive)
{
	I_ASSERT(archive.IsStoring());
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

	for (		IVersionInfo::VersionIds::iterator iter = ids.begin();
				retVal && (iter != ids.end());
				++iter){
		retVal = retVal && archive.BeginTag(s_versionInfoTag);

		I_ASSERT(versionInfoPtr != NULL);

		int id = *iter;
		I_IF_DEBUG(
			quint32 dummyVersion;
			I_ASSERT(versionInfoPtr->GetVersionNumber(id, dummyVersion)) // // all known IDs must have its version.
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

template<class VersionInfo>
CArchiveTag TVersionInfoSerializer<VersionInfo>::s_headerTag("AcfHeader", "Header of ACF archive");

template<class VersionInfo>
CArchiveTag TVersionInfoSerializer<VersionInfo>::s_versionInfosTag("VersionInfos", "List of version infos");

template<class VersionInfo>
CArchiveTag TVersionInfoSerializer<VersionInfo>::s_versionInfoTag("Version", "Version info");

template<class VersionInfo>
CArchiveTag TVersionInfoSerializer<VersionInfo>::s_versionIdTag("Id", "Version ID");

template<class VersionInfo>
CArchiveTag TVersionInfoSerializer<VersionInfo>::s_versionNumberTag("Number", "Version number");

template<class VersionInfo>
CArchiveTag TVersionInfoSerializer<VersionInfo>::s_versionDescriptionTag("Description", "Version description");


} // namespace iser


#endif // !iser_CVersionInfoBase_included


