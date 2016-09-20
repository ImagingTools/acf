#ifndef iloggui_CMessageBoxComp_included
#define iloggui_CMessageBoxComp_included


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QVector>
#include <QtGui/QIcon>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMessageBox>
#else
#include <QtGui/QMessageBox>
#endif

// ACF includes
#include <icomp/CComponentBase.h>
#include <ilog/IMessageConsumer.h>
#include <ilog/CMessage.h>


namespace iloggui
{


/**
	Message container displaying messages as log list.
*/
class CMessageBoxComp:
			public QObject,
			public icomp::CComponentBase,
			virtual public ilog::IMessageConsumer
{
	Q_OBJECT

public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CMessageBoxComp);
		I_REGISTER_INTERFACE(ilog::IMessageConsumer);
	I_END_COMPONENT;

	CMessageBoxComp();

	// reimplemented (ilog::IMessageConsumer)
	virtual bool IsMessageSupported(
				int messageCategory = -1,
				int messageId = -1,
				const istd::IInformationProvider* messagePtr = NULL) const;
	virtual void AddMessage(const MessagePtr& messagePtr);

protected:
	/**
		Get icons corresponding to specified information category.
	*/
	QIcon GetCategoryIcon(int category) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	void CreateMessageText(QString& messageText, QString& detailedText, QIcon& statusIcon) const;

protected Q_SLOTS:
	virtual void OnAddMessage();

Q_SIGNALS:
	void EmitAddMessage();

private:
	QMessageBox m_messageBox;
	QMutex m_messageQueueMutex;

	QVector<ilog::CMessage> m_messageQueue;
};


} // namespace iloggui


#endif // !iloggui_CMessageBoxComp_included


