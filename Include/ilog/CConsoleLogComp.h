#ifndef ilog_CConsoleLogComp_included
#define ilog_CConsoleLogComp_included


// ACF includes
#include "ilog/CStreamLogCompBase.h"


namespace ilog
{


/**
	Message container displaying messages as log list.

	\ingroup Logging
*/
class CConsoleLogComp: public CStreamLogCompBase
{
public:
	typedef CStreamLogCompBase BaseClass;

	I_BEGIN_COMPONENT(CConsoleLogComp);
	I_END_COMPONENT;

protected:
	// reimplemented (CStreamLogCompBase)
	virtual void WriteText(const QString& text);
	virtual void NewLine();
};


} // namespace ilog


#endif // !ilog_CConsoleLogComp_included

