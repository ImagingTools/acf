#ifndef ibase_TLoggerCompWrap_included
#define ibase_TLoggerCompWrap_included


#include "icomp/CComponentBase.h"
#include "icomp/CComponentContext.h"

#include "istd/ILogger.h"

#include "ibase/TLoggerWrap.h"


namespace ibase
{		


/**
	Wrapper provider of log-functionality for component based implementations.

	\ingroup Helpers
*/
template <class Base>
class TLoggerCompWrap: public ibase::TLoggerWrap<Base>
{
public:
	typedef ibase::TLoggerWrap<Base> BaseClass;

	I_BEGIN_BASE_COMPONENT(TLoggerCompWrap);
		I_ASSIGN(m_logCompPtr, "Log", "Consumer log messages", false, "Log");
		I_ASSIGN(m_verboseEnabledAttrPtr, "EnableVerbose", "If enabled, verbose messages can be produced", true, false);
	I_END_COMPONENT;

protected:
	/**
		Check if verbose messages are eneabled.
	*/
	bool IsVerboseEnabled() const;

	/**
		Send verbose message. If \c m_verboseEnabledAttrPtr is not enabled, the function does nothiing.
	*/
	void SendVerboseMessage(const istd::CString& message, const istd::CString& messageSource = istd::CString()) const;

	// reimplemented (istd::ILogger)
	virtual void DecorateMessage(
				istd::ILogger::MessageCategory category,
				int id,
				int flags,
				istd::CString& message,
				istd::CString& messageSource) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(ibase::IMessageConsumer, m_logCompPtr);
	I_ATTR(bool, m_verboseEnabledAttrPtr);
};


// protected methods
	
template <class Base>
bool TLoggerCompWrap<Base>::IsVerboseEnabled() const
{
	return *m_verboseEnabledAttrPtr;
}


template <class Base>
void TLoggerCompWrap<Base>::SendVerboseMessage(const istd::CString& message, const istd::CString& messageSource) const
{
	if (*m_verboseEnabledAttrPtr){
		BaseClass::SendInfoMessage(0, message, messageSource);
	}
}


// reimplemented (istd::ILogger)

template <class Base>
void TLoggerCompWrap<Base>::DecorateMessage(
			istd::ILogger::MessageCategory category,
			int id,
			int flags,
			istd::CString& message,
			istd::CString& messageSource) const
{
	BaseClass::DecorateMessage(category, id, flags, message, messageSource);

	const icomp::CComponentContext* contextPtr = dynamic_cast<const icomp::CComponentContext*>(BaseClass::GetComponentContext());
	if (contextPtr != NULL){
		if (messageSource.empty()){
			messageSource = contextPtr->GetContextId();
		}
		else{
			messageSource = istd::CString(contextPtr->GetContextId()) + " (" + messageSource + ")";
		}
	}
}


// reimplemented (icomp::CComponentBase)

template <class Base>
void TLoggerCompWrap<Base>::OnComponentCreated()
{
	if (m_logCompPtr.IsValid()){
		this->SetLogPtr(m_logCompPtr.GetPtr());
	}

	BaseClass::OnComponentCreated();
}


/**
	Type defininig base class for simple components outputing log messages.

	\ingroup Main
*/
typedef TLoggerCompWrap<icomp::CComponentBase> CLoggerComponentBase;


} // namespace ibase


#endif // !ibase_TLoggerCompWrap_included


