#ifndef ibase_CExternalFileConverterComp_included
#define ibase_CExternalFileConverterComp_included


#include "istd/IChangeable.h"

#include "icomp/CComponentBase.h"

#include "ibase/IApplication.h"
#include "ibase/IApplicationInfo.h"
#include "ibase/IFileConvertCopy.h"


namespace ibase
{


/**
	File converter, which uses an external programm to perform the convert action. 
*/
class CExternalFileConverterComp:
			public icomp::CComponentBase,
			virtual public IFileConvertCopy
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CExternalFileConverterComp);
		I_REGISTER_INTERFACE(IFileConvertCopy);
		I_ASSIGN(m_applicationCompPtr, "Application", "The program, that will be used for convert action", true, "Application");
		I_ASSIGN(m_applicationArgumentsAttrPtr, "ApplicationArguments", "Application conversion arguments.\nUse %1 to specify the input and %2 for output file name", false, "%1 %2");
	I_END_COMPONENT;

	// reimplemented (ibase::IFileConvertCopy)
	virtual bool CopyFile(
				const istd::CString& inputFilePath,
				const istd::CString& outputFilePath,
				const iprm::IParamsSet* paramsPtr = NULL) const;

private:
	I_REF(ibase::IApplication, m_applicationCompPtr);
	I_ATTR(istd::CString, m_applicationArgumentsAttrPtr);
};


} // namespace ibase


#endif // !ibase_CExternalFileConverterComp_included


