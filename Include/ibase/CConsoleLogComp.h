#ifndef ibase_CConsoleLogComp_included
#define ibase_CConsoleLogComp_included


#include "icomp/CComponentBase.h"

#include "ibase/IMessageConsumer.h"


namespace ibase
{


/**
	Message container displaying messages as log list.
*/
class CConsoleLogComp:
			public icomp::CComponentBase,
			virtual public ibase::IMessageConsumer
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CConsoleLogComp);
		I_REGISTER_INTERFACE(IMessageConsumer);
		I_ASSIGN(m_minPriorityAttrPtr, "MinCategory", "Minimal category of message to print it out", true, 0);
		I_ASSIGN(m_isDotEnabledAttrPtr, "ShowDots", "If it's true, dot will be shown for each ignored message", true, false);
	I_END_COMPONENT;

	CConsoleLogComp();

	// reimplemented (ibase::IMessageConsumer)
	virtual void AddMessage(ibase::IMessage* messagePtr);

	// reimplemented (icomp::IComponent)
	virtual void OnComponentDestroyed();

private:
	I_ATTR(int, m_minPriorityAttrPtr);
	I_ATTR(bool, m_isDotEnabledAttrPtr);

	bool m_isLastDotShown;
};


} // namespace ibase


#endif // !ibase_CConsoleLogComp_included

