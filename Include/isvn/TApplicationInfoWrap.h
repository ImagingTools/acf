#ifndef isvn_TApplicationInfoWrap_included
#define isvn_TApplicationInfoWrap_included


#include "isvn/Generated/AcfVersion.h"


namespace isvn
{


template <class Base>
class TApplicationInfoWrap: public Base
{
public:
	typedef Base BaseClass;

	// pseudo-reimplemented (ibase::IApplicationInfo)
	virtual istd::CString GetEncodedVersionName(int versionId, I_DWORD versionNumber) const;

	// pseudo-reimplemented (iser::IVersionInfo)
	virtual bool GetVersionNumber(int versionId, I_DWORD& result) const;
	virtual istd::CString GetVersionIdDescription(int versionId) const;
	virtual iser::IVersionInfo::VersionIds GetVersionIds() const;
};


// public methods

// pseudo-reimplemented (ibase::IApplicationInfo)

template <class Base>
istd::CString TApplicationInfoWrap<Base>::GetEncodedVersionName(int versionId, I_DWORD versionNumber) const
{
	istd::CString retVal = BaseClass::GetEncodedVersionName(versionId, versionNumber);

	if ((RS_DIRTY_FLAG != 0) && (versionId == iser::IVersionInfo::FrameworkVersionId) && (versionNumber == RS_USE_VERSION)){
		return retVal + "'";
	}

	return retVal;
}


// pseudo-reimplemented (iser::IVersionInfo)

template <class Base>
bool TApplicationInfoWrap<Base>::GetVersionNumber(int versionId, I_DWORD& result) const
{
	if (versionId == iser::IVersionInfo::FrameworkVersionId){
		result = RS_USE_VERSION;

		return true;
	}
	else{
		return BaseClass::GetVersionNumber(versionId, result);
	}
}


template <class Base>
istd::CString TApplicationInfoWrap<Base>::GetVersionIdDescription(int versionId) const
{
	if (versionId == iser::IVersionInfo::FrameworkVersionId){
		return "ACF";
	}
	else{
		return BaseClass::GetVersionIdDescription(versionId);
	}
}


template <class Base>
iser::IVersionInfo::VersionIds TApplicationInfoWrap<Base>::GetVersionIds() const
{
	VersionIds retVal = BaseClass::GetVersionIds();

	retVal.insert(iser::IVersionInfo::FrameworkVersionId);

	return retVal;
}


} // namespace isvn


#endif // !isvn_TApplicationInfoWrap_included


