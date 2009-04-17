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
	I_END_COMPONENT;

	// reimplemented (ibase::IMessageConsumer)
	virtual void AddMessage(ibase::IMessage* messagePtr);
};


} // namespace ibase


#endif // !ibase_CConsoleLogComp_included

