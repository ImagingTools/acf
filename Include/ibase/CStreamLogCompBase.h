#ifndef ibase_CStreamLogCompBase_included
#define ibase_CStreamLogCompBase_included


#include "icomp/CComponentBase.h"

#include "ibase/IMessageConsumer.h"


namespace ibase
{


/**
	Basic implementation for a log written to an output stream.
*/
class CStreamLogCompBase:
			public icomp::CComponentBase,
			virtual public ibase::IMessageConsumer
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_BASE_COMPONENT(CStreamLogCompBase);
		I_REGISTER_INTERFACE(IMessageConsumer);
		I_ASSIGN(m_minPriorityAttrPtr, "MinCategory", "Minimal category of message to print it out", true, 0);
		I_ASSIGN(m_isDotEnabledAttrPtr, "ShowDots", "If it's true, dot will be shown for each ignored message", true, false);
	I_END_COMPONENT;

	CStreamLogCompBase();

	// reimplemented (ibase::IMessageConsumer)
	virtual void AddMessage(ibase::IMessage* messagePtr);

	// reimplemented (icomp::IComponent)
	virtual void OnComponentDestroyed();

protected:
	/**
		Make output for skipped messages.
	*/
	virtual void ProcessIngoredMessage(const ibase::IMessage& message);

	/**
		Write formatted the message to the output stream.
	*/
	virtual void WriteMessageToStream(const ibase::IMessage& message);

	// abstract methods

	/**
		Write a text line to the output stream.
	*/
	virtual void WriteLine(const istd::CString& text) = 0;

	/**
		Make new line.
	*/
	virtual void NewLine() = 0;

private:
	I_ATTR(int, m_minPriorityAttrPtr);
	I_ATTR(bool, m_isDotEnabledAttrPtr);

	bool m_isLastDotShown;
};


} // namespace ibase


#endif // !ibase_CStreamLogCompBase_included

