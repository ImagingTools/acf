#ifndef ibase_TLoggableWrap_included
#define ibase_TLoggableWrap_included


#include "ibase/IMessageConsumer.h"
#include "ibase/CMessage.h"


namespace ibase
{		


template <class Base>
class TLoggableWrap: public Base
{
public:
	TLoggableWrap();

	/**
		Install log.
	*/
	void SetLogPtr(ibase::IMessageConsumer* logPtr);
	
	/**
		Get log.
	*/
	ibase::IMessageConsumer* GetLogPtr() const;

protected:
	/**
		Send any message to log.
		\sa ibase::IMessage		for message meaning documentation.
		\param	category		message category defined in ibase::IMessage.
		\param	id				binary id identifying this message type for automatical processing.
		\param	message			message text will be send.
		\param	messageSource	source of the message
	*/
	virtual bool SendMessage(ibase::IMessage::MessageCategory category, int id, const istd::CString& message, const istd::CString& messageSource) const;

	/**
		Send info message to log.
		\sa ibase::IMessage for message meaning documentation.
		\param	id				binary id identifying this message type for automatical processing.
		\param	message			message text will be send.
		\param	messageSource	source of the message
	*/
	bool SendInfoMessage(int id, const istd::CString& message, const istd::CString& messageSource = istd::CString()) const;

	/**
		Send warning message to log.
		\sa ibase::IMessage for message meaning documentation.
		\param	id				binary id identifying this message type for automatical processing.
		\param	message			message text will be send.
		\param	messageSource	source of the message
	*/
	bool SendWarningMessage(int id, const istd::CString& message, const istd::CString& messageSource = istd::CString()) const;

	/**
		Send error message to log.
		\sa ibase::IMessage for message meaning documentation.
		\param	id				binary id identifying this message type for automatical processing.
		\param	message			message text will be send.
		\param	messageSource	source of the message
	*/
	bool SendErrorMessage(int id, const istd::CString& message, const istd::CString& messageSource = istd::CString()) const;

	/**
		Send critical message to log.
		\sa ibase::IMessage for message meaning documentation.
		\param	id			binary id identifying this message type for automatical processing.
		\param	message		message text will be send.
	*/
	bool SendCriticalMessage(int id, const istd::CString& message, const istd::CString& messageSource = istd::CString()) const;

private:
	ibase::IMessageConsumer* m_logPtr;
};


// public methods

template <class Base>
TLoggableWrap<Base>::TLoggableWrap()
	:m_logPtr(NULL)
{
}


template <class Base>
inline void TLoggableWrap<Base>::SetLogPtr(ibase::IMessageConsumer* logPtr)
{
	m_logPtr = logPtr;
}


template <class Base>
inline ibase::IMessageConsumer* TLoggableWrap<Base>::GetLogPtr() const
{
	return m_logPtr;
}


// protected methods

template <class Base>
bool TLoggableWrap<Base>::SendMessage(ibase::IMessage::MessageCategory category, int id, const istd::CString& message, const istd::CString& messageSource) const
{
	if (m_logPtr != NULL){
		m_logPtr->AddMessage(new ibase::CMessage(category, id, message, messageSource));
	
		return true;
	}

	return false;
}


template <class Base>
bool TLoggableWrap<Base>::SendInfoMessage(int id, const istd::CString& message, const istd::CString& messageSource) const
{
	return SendMessage(ibase::IMessage::MC_INFO, id, message, messageSource);
}


template <class Base>
bool TLoggableWrap<Base>::SendWarningMessage(int id, const istd::CString& message, const istd::CString& messageSource) const
{
	return SendMessage(ibase::IMessage::MC_WARNING, id, message, messageSource);
}


template <class Base>
bool TLoggableWrap<Base>::SendErrorMessage(int id, const istd::CString& message, const istd::CString& messageSource) const
{
	return SendMessage(ibase::IMessage::MC_ERROR, id, message, messageSource);
}


template <class Base>
bool TLoggableWrap<Base>::SendCriticalMessage(int id, const istd::CString& message, const istd::CString& messageSource) const
{
	return SendMessage(ibase::IMessage::MC_CRITICAL, id, message, messageSource);
}


} // namespace ibase


#endif // !ibase_TLoggableWrap_included


