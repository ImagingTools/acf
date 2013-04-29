#ifndef ilog_CLogComp_included
#define ilog_CLogComp_included


// ACF includes
#include "icomp/CComponentBase.h"
#include "ilog/IMessageContainer.h"
#include "ilog/TMessageDelegatorComp.h"
#include "ilog/CMessageContainer.h"
#include "ilog/CMessage.h"


namespace ilog
{


/**
	Data model of logging component.

	\ingroup Logging
*/
class CLogComp:
	public ilog::TMessageDelegatorComp<icomp::CComponentBase>,
	public ilog::CMessageContainer
{
public:
	typedef ilog::TMessageDelegatorComp<icomp::CComponentBase> BaseClass;
	typedef ilog::CMessageContainer BaseClass2;

	I_BEGIN_COMPONENT(CLogComp);
		I_REGISTER_INTERFACE(ilog::IMessageConsumer);
		I_REGISTER_INTERFACE(ilog::IMessageContainer);
		I_ASSIGN(m_maxMessageCountAttrPtr, "MaxMessageCount", "Maximal number of messages", false, 1000);
	I_END_COMPONENT;

	// reimplemented (ilog::IMessageConsumer)
	virtual bool IsMessageSupported(
				int messageCategory = -1,
				int messageId = -1,
				const istd::IInformationProvider* messagePtr = NULL) const;
	virtual void AddMessage(const IMessageConsumer::MessagePtr& messagePtr);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(int, m_maxMessageCountAttrPtr);
};


} // namespace ilog


#endif // !ilog_CLogComp_included

