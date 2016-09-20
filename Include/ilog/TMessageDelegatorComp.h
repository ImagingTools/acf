#ifndef ilog_TMessageDelegatorComp_included
#define ilog_TMessageDelegatorComp_included


// ACF includes
#include <icomp/CComponentBase.h>
#include <ilog/IMessageConsumer.h>


namespace ilog
{		


/**
	Delegator component for a message consumer.

	\ingroup Logging
*/
template <class BaseComponent>
class TMessageDelegatorComp: public BaseComponent, virtual public ilog::IMessageConsumer
{
public:
	typedef BaseComponent BaseClass;

	I_BEGIN_COMPONENT(TMessageDelegatorComp);
		I_REGISTER_INTERFACE(ilog::IMessageConsumer);
		I_ASSIGN(m_slaveMessageConsumerCompPtr, "SlaveMessageConsumer", "Slave message consumer", false, "SlaveMessageConsumer");
	I_END_COMPONENT;

	// reimplemented (ilog::IMessageConsumer)
	virtual bool IsMessageSupported(
				int messageCategory = -1,
				int messageId = -1,
				const istd::IInformationProvider* messagePtr = NULL) const;
	virtual void AddMessage(const MessagePtr& messagePtr);

protected:
	I_REF(ilog::IMessageConsumer, m_slaveMessageConsumerCompPtr);
};


// public methods

// reimplemented (ilog::IMessageConsumer)

template <class BaseComponent>
bool TMessageDelegatorComp<BaseComponent>::IsMessageSupported(
			int messageCategory,
			int messageId,
			const istd::IInformationProvider* messagePtr) const
{
	if (m_slaveMessageConsumerCompPtr.IsValid()){
		return m_slaveMessageConsumerCompPtr->IsMessageSupported(messageCategory, messageId, messagePtr);
	}

	return true;
}


template <class BaseComponent>
void TMessageDelegatorComp<BaseComponent>::AddMessage(const MessagePtr& messagePtr)
{
	if (m_slaveMessageConsumerCompPtr.IsValid()){
		return m_slaveMessageConsumerCompPtr->AddMessage(messagePtr);
	}
}


} // namespace ilog


#endif // !ilog_TMessageDelegatorComp_included


