#ifndef ilog_CStreamLogCompBase_included
#define ilog_CStreamLogCompBase_included


// ACF includes
#include "ilog/TMessageDelegatorComp.h"


namespace ilog
{


/**
	Basic implementation for a log written to an output stream.

	\ingroup Logging
*/
class CStreamLogCompBase:
			public ilog::TMessageDelegatorComp<icomp::CComponentBase>
{
public:
	typedef ilog::TMessageDelegatorComp<icomp::CComponentBase> BaseClass;

	I_BEGIN_BASE_COMPONENT(CStreamLogCompBase);
		I_ASSIGN(m_minPriorityAttrPtr, "MinCategory", "Minimal category of message to print it out:\n *1-Information\n *2-Warning\n *3-Error\n *4-Critical", true, 0);
		I_ASSIGN(m_isDotEnabledAttrPtr, "ShowDots", "If it's true, dot will be shown for each ignored message", true, false);
		I_ASSIGN(m_useTimeStampAttrPtr, "UseTimeStamp", "Use time stamp for the messsages", false, true);
	I_END_COMPONENT;

	CStreamLogCompBase();

	// reimplemented (ilog::IMessageConsumer)
	virtual bool IsMessageSupported(
				int messageCategory = -1,
				int messageId = -1,
				const istd::IInformationProvider* messagePtr = NULL) const;
	virtual void AddMessage(const MessagePtr& messagePtr);

protected:
	/**
		Write formatted message to the output stream.
	*/
	virtual void WriteMessageToStream(const istd::IInformationProvider& message);
	
	/**
		Generate formatted message text for the output.
	*/
	virtual QString GenerateMessageText(const istd::IInformationProvider& message) const;

	// abstract methods

	/**
		Write a text line to the output stream.
	*/
	virtual void WriteText(const QString& text) = 0;

	/**
		Make new line.
	*/
	virtual void NewLine() = 0;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentDestroyed();

private:
	I_ATTR(int, m_minPriorityAttrPtr);
	I_ATTR(bool, m_isDotEnabledAttrPtr);
	I_ATTR(bool, m_useTimeStampAttrPtr);

	bool m_isLastDotShown;
};


} // namespace ilog


#endif // !ilog_CStreamLogCompBase_included

