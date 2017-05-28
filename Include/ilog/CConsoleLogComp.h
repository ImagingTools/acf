#ifndef ilog_CConsoleLogComp_included
#define ilog_CConsoleLogComp_included


// ACF includes
#include <ilog/CStreamLogCompBase.h>
#include <istd/IInformationProvider.h>


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
	virtual void WriteText(const QString& text, istd::IInformationProvider::InformationCategory category);

private:
	void SetConsoleColor(istd::IInformationProvider::InformationCategory category);
};


} // namespace ilog


#endif // !ilog_CConsoleLogComp_included

