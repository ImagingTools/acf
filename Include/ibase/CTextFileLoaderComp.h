#ifndef istdc_CTextFileLoaderComp_included
#define istdc_CTextFileLoaderComp_included


#include "iser/IFileLoader.h"

#include "icomp/IRegistry.h"
#include "icomp/CComponentBase.h"


namespace ibase
{


class CTextFileLoaderComp: public icomp::CComponentBase, public iser::IFileLoader
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CTextFileLoaderComp)
		I_REGISTER_INTERFACE(iser::IFileLoader)
		I_ASSIGN_MULTI_1(m_fileExtensionsAttrPtr, "FileExtensions", "List of possible file extensions", false, "txt");
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
	I_MULTIATTR(istd::CString, m_fileExtensionsAttrPtr);
};


} // namespace ibase


#endif // !istdc_CTextFileLoaderComp_included


