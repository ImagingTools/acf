#ifndef ibase_TLoggerCompWrap_included
#define ibase_TLoggerCompWrap_included


#include "icomp/CComponentBase.h"
#include "icomp/CComponentContext.h"

#include "ibase/TLoggerWrap.h"


namespace ibase
{		


/**
	Wrapper provider of log-functionality for component based implementations.
*/
template <class Base>
class TLoggerCompWrap: public ibase::TLoggerWrap<Base>
{
public:
	typedef ibase::TLoggerWrap<Base> BaseClass;

	I_BEGIN_BASE_COMPONENT(TLoggerCompWrap);
		I_ASSIGN(m_logCompPtr, "Log", "Consumer log messages", false, "Log");
	I_END_COMPONENT
	
	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

protected:
	// reimplemented (istd::ILogger)
	virtual void DecorateMessage(
				MessageCategory category,
				int id,
				int flags,
				istd::CString& message,
				istd::CString& messageSource) const;

private:
	I_REF(ibase::IMessageConsumer, m_logCompPtr);
};


// public methods

// reimplemented (icomp::IComponent)

template <class Base>
void TLoggerCompWrap<Base>::OnComponentCreated()
{
	if (m_logCompPtr.IsValid()){
		this->SetLogPtr(m_logCompPtr.GetPtr());
	}

	BaseClass::OnComponentCreated();
}


// protected methods

// reimplemented (istd::ILogger)

template <class Base>
void TLoggerCompWrap<Base>::DecorateMessage(
			MessageCategory category,
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


typedef TLoggerCompWrap<icomp::CComponentBase> CLoggerComponentBase;


} // namespace ibase


#endif // !ibase_TLoggerCompWrap_included


