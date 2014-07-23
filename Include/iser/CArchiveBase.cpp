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

	
const CArchiveTag& CArchiveBase::GetAcfRootTag()
{
	static CArchiveTag retVal("Acf", "Root tag of ACF");
	
	return retVal;
}


// private methods

CArchiveBase::CArchiveBase(const CArchiveBase&)
:	istd::IPolymorphic(),
	iser::IArchive(),
	istd::ILogger()
{
}


// static attributes

CArchiveTag CArchiveBase::s_acfRootTag("Acf", "Root tag of ACF", iser::CArchiveTag::TT_GROUP);


} // namespace iser


