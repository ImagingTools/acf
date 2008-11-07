#ifndef iqt_CSettingsSerializerComp_included
#define iqt_CSettingsSerializerComp_included


#include "iser/IFileLoader.h"

#include "icomp/CComponentBase.h"

#include "ibase/IApplicationInfo.h"
#include "ibase/TLoggerCompWrap.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Store and load parameter as global settings.
*/
class CSettingsSerializerComp:
			public ibase::TLoggerCompWrap<icomp::CComponentBase>,
			virtual public iser::IFileLoader
{
public:
	typedef ibase::TLoggerCompWrap<icomp::CComponentBase> BaseClass;

	I_BEGIN_COMPONENT(CSettingsSerializerComp)
		I_REGISTER_INTERFACE(iser::IFileLoader)
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Application info", true, "ApplicationInfo")
	I_END_COMPONENT

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				bool forLoading = true,
				bool forSaving = true,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual bool GetFileExtensions(istd::CStringList& result, bool doAppend = false) const;

private:
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);
};
	  

} // namespace iqt


#endif // !iqt_CSettingsSerializerComp_included

