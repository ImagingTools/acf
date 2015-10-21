#include "ilog/CStreamLogCompBase.h"


// Qt includes
#include <QtCore/QDateTime>

// ACF includes
#include "istd/IInformationProvider.h"


namespace ilog
{


// public methods

CStreamLogCompBase::CStreamLogCompBase()
:	m_isLastDotShown(false),
	m_lastDotCategory(istd::IInformationProvider::IC_NONE),
	m_worseCategory(istd::IInformationProvider::IC_NONE)
{
	qRegisterMetaType<MessagePtr>("MessagePtr");

	connect(
				this,
				SIGNAL(EmitAddMessage(const MessagePtr&)),
				this,
				SLOT(OnAddMessage(const MessagePtr&)),
				Qt::QueuedConnection);
}


istd::IInformationProvider::InformationCategory CStreamLogCompBase::GetWorseCategory() const
{
	return m_worseCategory;
}


// reimplemented (ilog::IMessageConsumer)

bool CStreamLogCompBase::IsMessageSupported(
			int /*messageCategory*/,
			int /*messageId*/,
			const istd::IInformationProvider* /*messagePtr*/) const
{
	return true;
}


void CStreamLogCompBase::AddMessage(const MessagePtr& messagePtr)
{
	if (messagePtr.IsValid() && IsMessageSupported(messagePtr->GetInformationCategory())){
		Q_EMIT EmitAddMessage(messagePtr);
	}
}


// protected methods

void CStreamLogCompBase::WriteMessageToStream(const istd::IInformationProvider& message)
{
	QString messageText = GenerateMessageText(message);
	
	WriteText(messageText + "\n", message.GetInformationCategory());
}


QString CStreamLogCompBase::GenerateMessageText(const istd::IInformationProvider& message) const
{
	QString messageText;

	QString categoryCode;
	if (*m_useCategoryAttrPtr){
		switch (message.GetInformationCategory()){
		case istd::IInformationProvider::IC_WARNING:
			messageText += "warning: ";
			categoryCode = "W";
			break;

		case istd::IInformationProvider::IC_ERROR:
			messageText += "error: ";
			categoryCode = "E";
			break;

		case istd::IInformationProvider::IC_CRITICAL:
			messageText += "fatal error: ";
			categoryCode = "E!";
			break;

		default:
			categoryCode = "I";
			break;
		}
	}

	if (*m_useCodeAttrPtr){
		QString codeText = QString::number(message.GetInformationId());

		messageText += categoryCode + codeText + ": ";
	}

	if (*m_useTimeStampAttrPtr){
		QString timeStampText = message.GetInformationTimeStamp().toString();

		messageText += "[" + timeStampText + "] ";
	}

	messageText += message.GetInformationDescription();

	return messageText;
}


// reimplemented (icomp::CComponentBase)

void CStreamLogCompBase::OnComponentDestroyed()
{
	BaseClass::OnComponentDestroyed();

	if (m_isLastDotShown){
		WriteText("\n", m_lastDotCategory);

		m_isLastDotShown = false;
	}

	m_worseCategory = istd::IInformationProvider::IC_NONE;
}


// private slots

void CStreamLogCompBase::OnAddMessage(const MessagePtr& messagePtr)
{
	if (messagePtr.IsValid()){
		istd::IInformationProvider::InformationCategory category = messagePtr->GetInformationCategory();
		if (category >= *m_minPriorityAttrPtr){
			if (m_isLastDotShown){
				WriteText("\n", m_lastDotCategory);

				m_isLastDotShown = false;
			}

			WriteMessageToStream(*messagePtr);
		}
		else if (*m_isDotEnabledAttrPtr){
			WriteText(".", category);

			m_isLastDotShown = true;
			m_lastDotCategory = category;
		}

		if (category > m_worseCategory){
			m_worseCategory = category;
		}

		BaseClass::AddMessage(messagePtr);
	}
}


} // namespace ilog


