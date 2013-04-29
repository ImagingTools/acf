#ifndef ilog_TLoggerCompWrap_included
#define ilog_TLoggerCompWrap_included


// ACF includes
#include "icomp/CComponentBase.h"
#include "icomp/CComponentContext.h"
#include "istd/ILogger.h"
#include "ilog/TLoggerWrap.h"


namespace ilog
{

/**
	Wrapper provider of log-functionality for component based implementations.

	\ingroup Logging
*/
template <class Base>
class TLoggerCompWrap: public ilog::TLoggerWrap<Base>
{
public:
	typedef ilog::TLoggerWrap<Base> BaseClass;

	I_BEGIN_BASE_COMPONENT(TLoggerCompWrap);
		I_ASSIGN(m_logCompPtr, "Log", "Consumer log messages", false, "Log");
		I_ASSIGN(m_verboseEnabledAttrPtr, "EnableVerbose", "If enabled, verbose messages can be produced", true, false);
	I_END_COMPONENT;

protected:
	/**
		Check if verbose messages are enabled.
	*/
	bool IsVerboseEnabled() const;

	/**
		Send verbose message. If \c m_verboseEnabledAttrPtr is not enabled, the function does nothing.
	*/
	void SendVerboseMessage(const QString& message, const QString& messageSource = QString()) const;

	// reimplemented (istd::ILogger)
	virtual void DecorateMessage(
				istd::IInformationProvider::InformationCategory category,
				int id,
				int flags,
				QString& message,
				QString& messageSource) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(ilog::IMessageConsumer, m_logCompPtr);
	I_ATTR(bool, m_verboseEnabledAttrPtr);
};


// protected methods
	
template <class Base>
bool TLoggerCompWrap<Base>::IsVerboseEnabled() const
{
	return *m_verboseEnabledAttrPtr;
}


template <class Base>
void TLoggerCompWrap<Base>::SendVerboseMessage(const QString& message, const QString& messageSource) const
{
	if (*m_verboseEnabledAttrPtr){
		BaseClass::SendInfoMessage(0, message, messageSource);
	}
}


// reimplemented (istd::ILogger)

template <class Base>
void TLoggerCompWrap<Base>::DecorateMessage(
			istd::IInformationProvider::InformationCategory category,
			int id,
			int flags,
			QString& message,
			QString& messageSource) const
{
	BaseClass::DecorateMessage(category, id, flags, message, messageSource);

	const icomp::CComponentContext* contextPtr = dynamic_cast<const icomp::CComponentContext*>(BaseClass::GetComponentContext());
	if (contextPtr != NULL){
		if (messageSource.isEmpty()){
			messageSource = contextPtr->GetContextId();
		}
		else{
			messageSource = QString(contextPtr->GetContextId()) + " (" + messageSource + ")";
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
	Type defining base class for simple components outputting log messages.

	\ingroup Main
*/
typedef TLoggerCompWrap<icomp::CComponentBase> CLoggerComponentBase;


} // namespace ilog


#endif // !ilog_TLoggerCompWrap_included


