#ifndef ibase_TLoggerWrap_included
#define ibase_TLoggerWrap_included


#include "ibase/IMessageConsumer.h"
#include "ibase/CMessage.h"


namespace ibase
{		


template <class Base>
class TLoggerWrap: public Base
{
public:
	TLoggerWrap();

	// pseudo-reimplemented (ibase::ILoggable)
	virtual void SetLogPtr(ibase::IMessageConsumer* logPtr);
	virtual ibase::IMessageConsumer* GetLogPtr() const;

protected:
	/**
		Send any message to log.
		\sa ibase::IMessage		for message meaning documentation.
		\param	category		message category defined in ibase::IMessage.
		\param	id				binary id identifying this message type for automatical processing.
		\param	message			message text will be send.
		\param	messageSource	source of the message
	*/
	virtual bool SendLogMessage(ibase::IMessage::MessageCategory category, int id, const istd::CString& message, const istd::CString& messageSource) const;

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
		\param	id				binary id identifying this message type for automatical processing.
		\param	message			message text will be send.
		\param	messageSource	optional human readable description of message source.
	*/
	bool SendCriticalMessage(int id, const istd::CString& message, const istd::CString& messageSource = istd::CString()) const;

private:
	ibase::IMessageConsumer* m_logPtr;
};


// public methods

template <class Base>
TLoggerWrap<Base>::TLoggerWrap()
	:m_logPtr(NULL)
{
}


// pseudo-reimplemented (ibase::ILoggable)

template <class Base>
inline void TLoggerWrap<Base>::SetLogPtr(ibase::IMessageConsumer* logPtr)
{
	m_logPtr = logPtr;
}


template <class Base>
inline ibase::IMessageConsumer* TLoggerWrap<Base>::GetLogPtr() const
{
	return m_logPtr;
}


// protected methods

template <class Base>
bool TLoggerWrap<Base>::SendLogMessage(ibase::IMessage::MessageCategory category, int id, const istd::CString& message, const istd::CString& messageSource) const
{
	if (m_logPtr != NULL){
		m_logPtr->AddMessage(new ibase::CMessage(category, id, message, messageSource));
	
		return true;
	}

	return false;
}


template <class Base>
bool TLoggerWrap<Base>::SendInfoMessage(int id, const istd::CString& message, const istd::CString& messageSource) const
{
	return SendLogMessage(ibase::IMessage::MC_INFO, id, message, messageSource);
}


template <class Base>
bool TLoggerWrap<Base>::SendWarningMessage(int id, const istd::CString& message, const istd::CString& messageSource) const
{
	return SendLogMessage(ibase::IMessage::MC_WARNING, id, message, messageSource);
}


template <class Base>
bool TLoggerWrap<Base>::SendErrorMessage(int id, const istd::CString& message, const istd::CString& messageSource) const
{
	return SendLogMessage(ibase::IMessage::MC_ERROR, id, message, messageSource);
}


template <class Base>
bool TLoggerWrap<Base>::SendCriticalMessage(int id, const istd::CString& message, const istd::CString& messageSource) const
{
	return SendLogMessage(ibase::IMessage::MC_CRITICAL, id, message, messageSource);
}


} // namespace ibase


#endif // !ibase_TLoggerWrap_included


