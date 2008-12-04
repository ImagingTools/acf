#ifndef ibase_TLoggerCompWrap_included
#define ibase_TLoggerCompWrap_included


#include "icomp/CComponentBase.h"
#include "icomp/CComponentContext.h"

#include "ibase/TLoggerWrap.h"


namespace ibase
{		

template <class Base>
class TLoggerCompWrap: public ibase::TLoggerWrap<Base>
{
public:
	typedef ibase::TLoggerWrap<Base> BaseClass;

	I_BEGIN_BASE_COMPONENT(TLoggerCompWrap);
		I_ASSIGN(m_logCompPtr, "Log", "Consumer log messages", false, "Log");
	I_END_COMPONENT;
	
	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

protected:
	// reimplemented (TLoggerWrap)
	virtual bool SendLogMessage(IMessage::MessageCategory category, int id, const istd::CString& message, const istd::CString& messageSource) const;

private:
	I_REF(ibase::IMessageConsumer, m_logCompPtr);
};


// public methods

// reimplemented (icomp::IComponent)

template <class Base>
void TLoggerCompWrap<Base>::OnComponentCreated()
{
	if (m_logCompPtr.IsValid()){
		SetLogPtr(m_logCompPtr.GetPtr());
	}

	BaseClass::OnComponentCreated();
}


// protected methods

template <class Base>
bool TLoggerCompWrap<Base>::SendLogMessage(IMessage::MessageCategory category, int id, const istd::CString& message, const istd::CString& /*messageSource*/) const
{
	istd::CString messageSource = "<unknown>";

	if (m_logCompPtr.IsValid()){
		const icomp::CComponentContext* contextPtr = dynamic_cast<const icomp::CComponentContext*>(BaseClass::GetComponentContext());
		if (contextPtr != NULL){
			messageSource = contextPtr->GetContextId();
		}
	}

	return BaseClass::SendLogMessage(category, id, message, messageSource);
}


} // namespace ibase


#endif // !ibase_TLoggerCompWrap_included


