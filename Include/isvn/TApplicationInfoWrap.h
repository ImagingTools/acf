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
	virtual istd::CString EncodeVersionName(I_DWORD version, int versionId = UserVersionId) const;

	// pseudo-reimplemented (iser::IVersionInfo)
	virtual I_DWORD GetVersion(int versionId = UserVersionId) const;
	virtual istd::CString GetVersionIdDescription(int versionId) const;
	virtual iser::IVersionInfo::VersionIds GetVersionIds() const;
};


// public methods

// pseudo-reimplemented (ibase::IApplicationInfo)

template <class Base>
istd::CString TApplicationInfoWrap<Base>::EncodeVersionName(I_DWORD version, int versionId) const
{
	istd::CString retVal = BaseClass::EncodeVersionName(version, versionId);

	if ((RS_DIRTY_FLAG != 0) && (versionId == FrameworkVersionId) && (version == RS_USE_VERSION)){
		return retVal + "´";
	}

	return retVal;
}


// pseudo-reimplemented (iser::IVersionInfo)

template <class Base>
I_DWORD TApplicationInfoWrap<Base>::GetVersion(int versionId) const
{
	if (versionId == FrameworkVersionId){
		return RS_USE_VERSION;
	}
	else{
		return BaseClass::GetVersion(versionId);
	}
}


template <class Base>
istd::CString TApplicationInfoWrap<Base>::GetVersionIdDescription(int versionId) const
{
	if (versionId == FrameworkVersionId){
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

	retVal.insert(FrameworkVersionId);

	return retVal;
}


} // namespace isvn


#endif // !isvn_TApplicationInfoWrap_included


