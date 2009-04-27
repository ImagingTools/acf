#ifndef ibase_CConsoleLogComp_included
#define ibase_CConsoleLogComp_included


#include "ibase/CStreamLogCompBase.h"


namespace ibase
{


/**
	Message container displaying messages as log list.
*/
class CConsoleLogComp: public CStreamLogCompBase
{
public:
	typedef CStreamLogCompBase BaseClass;

	I_BEGIN_COMPONENT(CConsoleLogComp);
	I_END_COMPONENT;

protected:
	// reimplemented (CStreamLogCompBase)
	virtual void WriteLine(const istd::CString& text);
	virtual void NewLine();

};


} // namespace ibase


#endif // !ibase_CConsoleLogComp_included

