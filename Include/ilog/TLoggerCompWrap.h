#ifndef ilog_TLoggerCompWrap_included
#define ilog_TLoggerCompWrap_included


// ACF includes
#include <icomp/CComponentBase.h>
#include <icomp/CComponentContext.h>
#include <ilog/CLoggerBase.h>
#include <ilog/ITracingConfiguration.h>


namespace ilog
{


/**
	Wrapper provider of log-functionality for component based implementations.

	\ingroup Logging
*/
template <class Base>
class TLoggerCompWrap:
			public Base,
			virtual public ilog::CLoggerBase
{
public:
	typedef Base BaseClass;
	typedef ilog::CLoggerBase BaseClass2;

	I_BEGIN_BASE_COMPONENT(TLoggerCompWrap);
		I_ASSIGN(m_logCompPtr, "Log", "Consumer log messages", false, "Log");
		I_ASSIGN(m_verboseEnabledAttrPtr, "EnableVerbose", "If enabled, verbose messages can be produced", true, false);
		I_ASSIGN(m_tracingConfigurationCompPtr, "TracingConfiguration", "If enabled, verbose messages can be produced with tracing level", false, "TracingConfiguration");
		I_ASSIGN(m_showComponentIdAttrPtr, "ShowComponentId", "If enabled the component ID will be shown as a part of the message source", true, true);
	I_END_COMPONENT;

protected:
	/**
		Check if verbose messages are enabled.
	*/
	bool IsVerboseEnabled(int tracingLevel = 0) const;

	/**
		Send verbose message. If \c m_verboseEnabledAttrPtr is not enabled, the function does nothing.
	*/
	void SendVerboseMessage(const QString& message, const QString& messageSource = QString(), int tracingLevel = 0) const;

	// reimplemented (istd::ILogger)
	virtual void DecorateMessage(
				istd::IInformationProvider::InformationCategory category,
				int id,
				int flags,
				QString& message,
				QString& messageSource) const override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

private:
	I_REF(ilog::IMessageConsumer, m_logCompPtr);
	I_ATTR(bool, m_verboseEnabledAttrPtr);
	I_REF(ilog::ITracingConfiguration, m_tracingConfigurationCompPtr);
	I_ATTR(bool, m_showComponentIdAttrPtr);
};


// protected methods
	
template <class Base>
bool TLoggerCompWrap<Base>::IsVerboseEnabled(int tracingLevel) const
{
	static const istd::IInformationProvider::InformationCategory categoryNone = istd::IInformationProvider::IC_NONE;

	bool retVal = *m_verboseEnabledAttrPtr;;

	if (m_tracingConfigurationCompPtr.IsValid()){
		retVal = m_tracingConfigurationCompPtr->GetTracingLevel() >= tracingLevel;
	}

	retVal = retVal && BaseClass2::IsLogConsumed(&categoryNone);

	return retVal;
}


template <class Base>
void TLoggerCompWrap<Base>::SendVerboseMessage(const QString& message, const QString& messageSource, int tracingLevel) const
{
	if (IsVerboseEnabled(tracingLevel)){
		BaseClass2::SendLogMessage(istd::IInformationProvider::IC_NONE, 0, message, messageSource);
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
	BaseClass2::DecorateMessage(category, id, flags, message, messageSource);

	if (*m_showComponentIdAttrPtr){
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


template <class Base>
void TLoggerCompWrap<Base>::OnComponentDestroyed()
{
	this->SetLogPtr(NULL);

	BaseClass::OnComponentDestroyed();
}


/**
	Type defining base class for simple components outputting log messages.

	\ingroup Main
*/
typedef TLoggerCompWrap<icomp::CComponentBase> CLoggerComponentBase;


} // namespace ilog


#endif // !ilog_TLoggerCompWrap_included


