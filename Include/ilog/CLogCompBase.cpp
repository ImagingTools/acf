#include <ilog/CLogCompBase.h>


// Qt includes
#include <QtCore/QDateTime>
#include <QtCore/QThread>
#include <QtCore/QMetaType>


namespace ilog
{


// public methods

CLogCompBase::CLogCompBase()
{
	qRegisterMetaType<MessagePtr>("MessagePtr");

	connect(
				this,
				SIGNAL(EmitAddMessage(const MessagePtr&)),
				this,
				SLOT(OnAddMessage(const MessagePtr&)),
				Qt::QueuedConnection);
}


// reimplemented (ilog::IMessageConsumer)

void CLogCompBase::AddMessage(const MessagePtr& messagePtr)
{
	if (messagePtr.IsValid() && IsMessageSupported(messagePtr->GetInformationCategory())){
		QCoreApplication* applicationPtr = QCoreApplication::instance();
		bool isMainThread = (applicationPtr == NULL) || (QThread::currentThread() == applicationPtr->thread());
		if (!isMainThread){
			Q_EMIT EmitAddMessage(messagePtr);
		}
		else{
			OnAddMessage(messagePtr);
		}
	}
}


// private slots

void CLogCompBase::OnAddMessage(const MessagePtr& messagePtr)
{
	BaseClass::AddMessage(messagePtr);

	WriteMessageToLog(messagePtr);
}



} // namespace ilog


