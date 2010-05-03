#ifndef ibase_CCopyAppComp_included
#define ibase_CCopyAppComp_included


#include "istd/IChangeable.h"

#include "iser/IFileLoader.h"

#include "icomp/CComponentBase.h"

#include "ibase/IApplication.h"
#include "ibase/IFileConvertCopy.h"


namespace ibase
{


/**
	General file transformation application.
	This implementation uses some slave ibase::IFileConvertCopy implementation to provide copy operation.
	It implements ibase::IApplication and can be used as standalone application.
*/
class CCopyAppComp:
			public icomp::CComponentBase,
			virtual public IApplication
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CCopyAppComp);
		I_REGISTER_INTERFACE(IApplication);

		I_ASSIGN(m_fileCopyCompPtr, "FileCopy", "Provide copy of single file", true, "FileCopy");
		I_ASSIGN(m_needExplicitInputAttrPtr, "NeedExplicitInput", "Indicate if explicit input file with -input is needed, otherwise first argument will be taken", true, false);
	I_END_COMPONENT;

	// reimplemented (ibase::IApplication)
	virtual bool InitializeApplication(int argc, char** argv);
	virtual int Execute(int argc, char** argv);
	virtual istd::CString GetHelpText() const;

private:
	I_REF(IFileConvertCopy, m_fileCopyCompPtr);
	I_ATTR(bool, m_needExplicitInputAttrPtr);
};


} // namespace ibase


#endif // !ibase_CCopyAppComp_included


