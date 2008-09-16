#ifndef ibase_TMessageProducerWrap_included
#define ibase_TMessageProducerWrap_included


#include "icomp/CComponentBase.h"
#include "icomp/CComponentContext.h"

#include "ibase/IMessageConsumer.h"
#include "ibase/CMessage.h"


namespace ibase
{		

template <class Base>
class TMessageProducerWrap: public Base
{
public:
	typedef Base BaseClass;

	I_BEGIN_BASE_COMPONENT(TMessageProducerWrap);
		I_ASSIGN(m_logCompPtr, "Log", "Consumer log messages", false, "Log");
	I_END_COMPONENT;

protected:
	/**
		Send any message to log.
		\sa ibase::IMessage for message meaning documentation.
		\param	category	message category defined in ibase::IMessage.
		\param	id			binary id identifying this message type for automatical processing.
		\param	message		message text will be send.
	*/
	bool SendMessage(IMessage::MessageCategory category, int id, const istd::CString& message);

	/**
		Send info message to log.
		\sa ibase::IMessage for message meaning documentation.
		\param	id			binary id identifying this message type for automatical processing.
		\param	message		message text will be send.
	*/
	bool SendInfoMessage(int id, const istd::CString& message);

	/**
		Send warning message to log.
		\sa ibase::IMessage for message meaning documentation.
		\param	id			binary id identifying this message type for automatical processing.
		\param	message		message text will be send.
	*/
	bool SendWarningMessage(int id, const istd::CString& message);

	/**
		Send error message to log.
		\sa ibase::IMessage for message meaning documentation.
		\param	id			binary id identifying this message type for automatical processing.
		\param	message		message text will be send.
	*/
	bool SendErrorMessage(int id, const istd::CString& message);

	/**
		Send critical message to log.
		\sa ibase::IMessage for message meaning documentation.
		\param	id			binary id identifying this message type for automatical processing.
		\param	message		message text will be send.
	*/
	bool SendCriticalMessage(int id, const istd::CString& message);

private:
	I_REF(ibase::IMessageConsumer, m_logCompPtr);
};


// protected methods

template <class Base>
bool TMessageProducerWrap<Base>::SendMessage(IMessage::MessageCategory category, int id, const istd::CString& message)
{
	if (m_logCompPtr.IsValid()){
		const icomp::CComponentContext* contextPtr = dynamic_cast<const icomp::CComponentContext*>(GetComponentContext());
		if (contextPtr != NULL){
			m_logCompPtr->AddMessage(new ibase::CMessage(category, id, message, contextPtr->GetContextId()));
		}
		else{
			m_logCompPtr->AddMessage(new ibase::CMessage(category, id, message, "<unknown>"));
		}

		return true;
	}

	return false;
}


template <class Base>
bool TMessageProducerWrap<Base>::SendInfoMessage(int id, const istd::CString& message)
{
	return SendMessage(ibase::IMessage::Info, id, message);
}


template <class Base>
bool TMessageProducerWrap<Base>::SendWarningMessage(int id, const istd::CString& message)
{
	return SendMessage(ibase::IMessage::Warning, id, message);
}


template <class Base>
bool TMessageProducerWrap<Base>::SendErrorMessage(int id, const istd::CString& message)
{
	return SendMessage(ibase::IMessage::Error, id, message);
}


template <class Base>
bool TMessageProducerWrap<Base>::SendCriticalMessage(int id, const istd::CString& message)
{
	return SendMessage(ibase::IMessage::Critical, id, message);
}


} // namespace ibase


#endif // !ibase_TMessageProducerWrap_included


