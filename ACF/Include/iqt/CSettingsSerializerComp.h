#ifndef iqt_CSettingsSerializerComp_included
#define iqt_CSettingsSerializerComp_included


#include "iser/IFileLoader.h"

#include "ibase/IApplicationInfo.h"

#include "icomp/CComponentBase.h"

#include "iqt/iqt.h"


namespace iqt
{


class CSettingsSerializerComp: public icomp::CComponentBase, virtual public iser::IFileLoader
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSettingsSerializerComp)
		I_REGISTER_INTERFACE(iser::IFileLoader)
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Application info", true, "ApplicationInfo")
	I_END_COMPONENT

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				bool forLoading = true,
				bool forSaving = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual const istd::CString& GetLastLoadFileName() const;
	virtual const istd::CString& GetLastSaveFileName() const;

private:
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);
};
	  

} // namespace iqt


#endif // !iqt_CSettingsSerializerComp_included

