#ifndef ilog_CStreamLogCompBase_included
#define ilog_CStreamLogCompBase_included


// ACF includes
#include <ilog/CLogCompBase.h>


namespace ilog
{


/**
	Basic implementation for a log written to an output stream.

	\ingroup Logging
*/
class CStreamLogCompBase: public CLogCompBase
{
public:
	typedef ilog::CLogCompBase BaseClass;

	I_BEGIN_BASE_COMPONENT(CStreamLogCompBase);
		I_ASSIGN(m_minPriorityAttrPtr, "MinCategory", "Minimal category of message to print it out:\n *1-Information\n *2-Warning\n *3-Error\n *4-Critical", true, 0);
		I_ASSIGN(m_isDotEnabledAttrPtr, "ShowDots", "If it's true, dot will be shown for each ignored message", true, false);
		I_ASSIGN(m_useCategoryAttrPtr, "UseCategory", "Use error category for the messsages (e.g warning or error)", true, true);
		I_ASSIGN(m_useCodeAttrPtr, "UseCode", "Use error code for the messsages", true, true);
		I_ASSIGN(m_useTimeStampAttrPtr, "UseTimeStamp", "Use time stamp for the messsages", true, false);
		I_ASSIGN(m_timeFormatAttrPtr, "TimeFormat", "Format used for timestamp output", false, "dd.MM hh:mm:ss:zzz");
	I_END_COMPONENT;

	CStreamLogCompBase();

	/**
		Get worse category (highest code) of all outputed messages.
	*/
	istd::IInformationProvider::InformationCategory GetWorseCategory() const;

	// reimplemented (ilog::IMessageConsumer)
	virtual bool IsMessageSupported(
				int messageCategory = -1,
				int messageId = -1,
				const istd::IInformationProvider* messagePtr = NULL) const;

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
	virtual void WriteText(const QString& text, istd::IInformationProvider::InformationCategory category) = 0;

	// reimplemented (ilog::CLogCompBase)
	virtual void WriteMessageToLog(const MessagePtr& messagePtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentDestroyed();

private:
	I_ATTR(int, m_minPriorityAttrPtr);
	I_ATTR(bool, m_isDotEnabledAttrPtr);
	I_ATTR(bool, m_useCategoryAttrPtr);
	I_ATTR(bool, m_useCodeAttrPtr);
	I_ATTR(bool, m_useTimeStampAttrPtr);
	I_ATTR(QByteArray, m_timeFormatAttrPtr); 

	bool m_isLastDotShown;
	istd::IInformationProvider::InformationCategory m_lastDotCategory;
	istd::IInformationProvider::InformationCategory m_worseCategory;
};


} // namespace ilog


#endif // !ilog_CStreamLogCompBase_included

