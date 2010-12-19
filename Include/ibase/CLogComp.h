#ifndef ibase_CLogComp_included
#define ibase_CLogComp_included


// ACF includes
#include "icomp/CComponentBase.h"

#include "ibase/IMessageContainer.h"
#include "ibase/TMessageContainerWrap.h"

#include "ibase/CMessage.h"


namespace ibase
{


/**
	Data model of logging component.
*/
class CLogComp:
	public icomp::CComponentBase,
	public ibase::CMessageContainer
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef ibase::CMessageContainer BaseClass2;

	I_BEGIN_COMPONENT(CLogComp);
		I_REGISTER_INTERFACE(ibase::IMessageConsumer);
		I_REGISTER_INTERFACE(ibase::IMessageContainer);
		I_ASSIGN(m_maxMessageCountAttrPtr, "MaxMessageCount", "Maximal number of messages", false, 1000);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(int, m_maxMessageCountAttrPtr);
};


} // namespace ibase


#endif // !ibase_CLogComp_included

