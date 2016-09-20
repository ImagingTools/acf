#ifndef ilog_CLogComp_included
#define ilog_CLogComp_included


// ACF includes
#include <ilog/CLogCompBase.h>
#include <ilog/CMessageContainer.h>


namespace ilog
{


/**
	Data model of logging component.

	\ingroup Logging
*/
class CLogComp:
			public ilog::CLogCompBase,
			public ilog::CMessageContainer
{
public:
	typedef CLogCompBase BaseClass;
	typedef ilog::CMessageContainer BaseClass2;

	I_BEGIN_COMPONENT(CLogComp);
		I_REGISTER_INTERFACE(ilog::IMessageContainer);
		I_ASSIGN(m_maxMessageCountAttrPtr, "MaxMessageCount", "Maximal number of messages", false, 1000);
	I_END_COMPONENT;

	// reimplemented (ilog::IMessageConsumer)
	virtual bool IsMessageSupported(
				int messageCategory = -1,
				int messageId = -1,
				const istd::IInformationProvider* messagePtr = NULL) const;
	virtual void AddMessage(const MessagePtr& messagePtr);

protected:
	// reimplemented (ilog::CLogCompBase)
	virtual void WriteMessageToLog(const MessagePtr& messagePtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(int, m_maxMessageCountAttrPtr);
};


} // namespace ilog


#endif // !ilog_CLogComp_included

