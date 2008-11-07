#ifndef ibase_TMessageProducerWrap_included
#define ibase_TMessageProducerWrap_included


#include "icomp/CComponentBase.h"
#include "icomp/CComponentContext.h"

#include "ibase/TLoggableWrap.h"


namespace ibase
{		

template <class Base>
class TMessageProducerWrap: public ibase::TLoggableWrap<Base>
{
public:
	typedef ibase::TLoggableWrap<Base> BaseClass;

	I_BEGIN_BASE_COMPONENT(TMessageProducerWrap);
		I_ASSIGN(m_logCompPtr, "Log", "Consumer log messages", false, "Log");
	I_END_COMPONENT;
	
	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

protected:
	// reimplemented (TLoggableWrap)
	virtual bool SendMessage(IMessage::MessageCategory category, int id, const istd::CString& message, const istd::CString& messageSource) const;

private:
	I_REF(ibase::IMessageConsumer, m_logCompPtr);
};


// public methods

// reimplemented (icomp::IComponent)

template <class Base>
void TMessageProducerWrap<Base>::OnComponentCreated()
{
	if (m_logCompPtr.IsValid()){
		SetLogPtr(m_logCompPtr.GetPtr());
	}

	BaseClass::OnComponentCreated();
}


// protected methods

template <class Base>
bool TMessageProducerWrap<Base>::SendMessage(IMessage::MessageCategory category, int id, const istd::CString& message, const istd::CString& /*messageSource*/) const
{
	istd::CString messageSource = "<unknown>";

	if (m_logCompPtr.IsValid()){
		const icomp::CComponentContext* contextPtr = dynamic_cast<const icomp::CComponentContext*>(GetComponentContext());
		if (contextPtr != NULL){
			messageSource = contextPtr->GetContextId();
		}
	}

	return BaseClass::SendMessage(category, id, message, messageSource);
}


} // namespace ibase


#endif // !ibase_TMessageProducerWrap_included


