#ifndef istdc_CRegistryCodeSaverComp_included
#define istdc_CRegistryCodeSaverComp_included


#include "iser/IFileLoader.h"

#include "icomp/IRegistry.h"
#include "icomp/CComponentBase.h"


namespace ibase
{


class CRegistryCodeSaverComp: public icomp::CComponentBase, public iser::IFileLoader
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CRegistryCodeSaverComp)
		I_REGISTER_INTERFACE(iser::IFileLoader)
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
	mutable istd::CString m_lastSaveFileName;
};


} // namespace ibase


#endif // !istdc_CRegistryCodeSaverComp_included


