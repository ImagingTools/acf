#ifndef ibase_CComposedLoaderComp_included
#define ibase_CComposedLoaderComp_included


#include "iser/IFileLoader.h"

#include "icomp/CComponentBase.h"


namespace ibase
{


class CComposedLoaderComp:
			public icomp::CComponentBase,
			virtual public iser::IFileLoader
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CComposedLoaderComp);
		I_REGISTER_INTERFACE(iser::IFileLoader);
		I_ASSIGN_MULTI_0(m_slaveLoadersCompPtr, "SlaveLoaders", "List of slave loaders", true);
		I_ASSIGN(m_commonDescriptionAttrPtr, "CommonDescription", "Optional common description of this file type", false, "Composed");
	I_END_COMPONENT;

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				int flags = 0,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual bool GetFileExtensions(istd::CStringList& result, bool doAppend = false) const;
	virtual istd::CString GetTypeDescription(const istd::CString* extensionPtr = NULL) const;

private:
	I_MULTIREF(iser::IFileLoader, m_slaveLoadersCompPtr);
	I_ATTR(istd::CString, m_commonDescriptionAttrPtr);
};


} // namespace ibase


#endif // !ibase_CComposedLoaderComp_included


