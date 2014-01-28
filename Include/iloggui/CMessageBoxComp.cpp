#include "iloggui/CMessageBoxComp.h"


// Qt includes
#include <QtCore/QDateTime>
#include <QtCore/QMetaType>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif


namespace iloggui
{


// public methods

CMessageBoxComp::CMessageBoxComp()
{
	connect(
				this,
				SIGNAL(EmitAddMessage()),
				this,
				SLOT(OnAddMessage()),
				Qt::QueuedConnection);
}


// protected methods

QIcon CMessageBoxComp::GetCategoryIcon(int category) const
{
	static QIcon infoIcon(":/Icons/Info.svg");
	static QIcon warningIcon(":/Icons/Warning.svg");
	static QIcon errorIcon(":/Icons/Error.svg");

	switch (category){
	case istd::IInformationProvider::IC_INFO:
		return infoIcon;

	case istd::IInformationProvider::IC_WARNING:
		return warningIcon;

	case istd::IInformationProvider::IC_ERROR:
	case istd::IInformationProvider::IC_CRITICAL:
		return errorIcon;

	default:
		return QIcon();
	}
}


// reimplemented (ilog::IMessageConsumer)

bool CMessageBoxComp::IsMessageSupported(
			int /*messageCategory*/,
			int /*messageId*/,
			const istd::IInformationProvider* /*messagePtr*/) const
{
	return true;
}


void CMessageBoxComp::AddMessage(const MessagePtr& messagePtr)
{
	ilog::CMessage incommingMessage;

	m_messageQueueMutex.lock();

	if (incommingMessage.CopyFrom(*messagePtr.GetPtr())){
		m_messageQueue.push_back(incommingMessage);
	}

	m_messageQueueMutex.unlock();

	Q_EMIT EmitAddMessage();
}


// reimplemented (icomp::CComponentBase)

void CMessageBoxComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();
}


// private methods

void CMessageBoxComp::CreateMessageText(QString& messageText, QString& detailedText, QIcon& statusIcon) const
{
	if (m_messageQueue.count() == 1){
		messageText = m_messageQueue[0].GetInformationDescription();

		statusIcon = GetCategoryIcon(m_messageQueue[0].GetInformationCategory());
	}
	else{
		int worstCategoryIndex = 0;
		int worstCategory = istd::IInformationProvider::IC_NONE;
		for (int i = m_messageQueue.count() - 1; i >= 0; --i){
			const ilog::CMessage& message = m_messageQueue[i];

			detailedText += message.GetInformationDescription() + "\n";

			if (message.GetInformationCategory() > worstCategory){
				worstCategoryIndex = i;

				worstCategory = message.GetInformationCategory();
			}
		}

		messageText = m_messageQueue[worstCategoryIndex].GetInformationDescription();
		statusIcon = GetCategoryIcon(m_messageQueue[worstCategoryIndex].GetInformationCategory());
	}
}


// protected slots

void CMessageBoxComp::OnAddMessage()
{
	if (!QApplication::startingUp()){
		if (!m_messageBox.isVisible()){
			m_messageQueueMutex.lock();

			QString messageText;
			QString detailedText;
			QIcon statusIcon;
			CreateMessageText(messageText, detailedText, statusIcon);

			m_messageQueue.clear();
			m_messageQueueMutex.unlock();

			m_messageBox.setText(messageText);
			m_messageBox.setDetailedText(detailedText);
			m_messageBox.setIconPixmap(statusIcon.pixmap(32, 32));

			m_messageBox.exec();
		}
	}
}


} // namespace iloggui


