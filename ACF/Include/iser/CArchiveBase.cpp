#include "iser/CArchiveBase.h"


namespace iser
{


// reimplemented (iser::IArchive)

bool CArchiveBase::IsTagSkippingSupported() const
{
	return false;
}


bool CArchiveBase::BeginMultiTag(const CArchiveTag& tag, const CArchiveTag& /*subTag*/, int& count)
{
	bool retVal = BeginTag(tag);

	retVal = retVal && Process(count);

	return retVal;
}


// static attributes

iser::CArchiveTag CArchiveBase::s_acfRootTag("Acf", "Root tag of ACF");
iser::CArchiveTag CArchiveBase::s_headerTag("AcfHeader", "Header of ACF archive");
iser::CArchiveTag CArchiveBase::s_versionInfosTag("VersionInfos", "List of version infos");
iser::CArchiveTag CArchiveBase::s_versionInfoTag("Version", "Version info");
iser::CArchiveTag CArchiveBase::s_versionIdTag("Id", "Version ID");
iser::CArchiveTag CArchiveBase::s_versionNumberTag("Number", "Version number");
iser::CArchiveTag CArchiveBase::s_versionDescriptionTag("Description", "Version description");


} // namespace iser


