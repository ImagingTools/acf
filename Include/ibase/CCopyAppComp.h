#ifndef ibase_CCopyAppComp_included
#define ibase_CCopyAppComp_included


#include "istd/IChangeable.h"

#include "iser/IFileLoader.h"

#include "icomp/CComponentBase.h"

#include "ibase/IApplication.h"


namespace ibase
{


class CCopyAppComp:
			public icomp::CComponentBase,
			virtual public IApplication
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CCopyAppComp);
		I_REGISTER_INTERFACE(IApplication);
		I_ASSIGN(m_objectCompPtr, "Object", "Object used as representation of copied data", true, "Object");
		I_ASSIGN(m_inputLoaderCompPtr, "InputLoader", "input file loader", true, "InputLoader");
		I_ASSIGN(m_outputLoaderCompPtr, "OutputLoader", "output file loader", true, "OutputLoader");
	I_END_COMPONENT;

	// reimplemented (ibase::IApplication)
	virtual int Execute(int argc, char** argv);
	virtual istd::CString GetHelpText() const;

private:
	I_REF(istd::IChangeable, m_objectCompPtr);
	I_REF(iser::IFileLoader, m_inputLoaderCompPtr);
	I_REF(iser::IFileLoader, m_outputLoaderCompPtr);
};


} // namespace ibase


#endif // !ibase_CCopyAppComp_included


