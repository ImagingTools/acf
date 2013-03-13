#include "istd/CEventBasedNotifier.h"


// Qt includes
#include <QtCore/QCoreApplication>
#include <QtCore/QThread>


namespace istd
{


// public methods

CEventBasedNotifier::CEventBasedNotifier(istd::IChangeable* slavePtr,  int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	m_notificationTarget = new NotificationTarget(slavePtr, changeFlags, changeParamsPtr);

	if (QCoreApplication::instance() != NULL){
		m_notificationTarget->moveToThread(QCoreApplication::instance()->thread());
	}

	m_notificationTarget->connect(m_notificationTarget, SIGNAL(EmitBeginChanges()), m_notificationTarget, SLOT(DoBeginChanges()));
	m_notificationTarget->connect(m_notificationTarget, SIGNAL(EmitEndChanges()), m_notificationTarget, SLOT(DoEndChanges()));

	Q_EMIT m_notificationTarget->EmitBeginChanges();
}


CEventBasedNotifier::~CEventBasedNotifier()
{
	m_notificationTarget->Reset();

	m_notificationTarget->deleteLater();
}


// public methods of the internal class NotificationTarget

NotificationTarget::NotificationTarget(istd::IChangeable* slavePtr, int changeFlags, istd::IPolymorphic* changeParamsPtr)
:	m_slavePtr(slavePtr),
	m_changeFlags(changeFlags),
 	m_isBeginPending(true)
{
	QThread* applicationThreadPtr = NULL;
	if (QCoreApplication::instance() != NULL){
		applicationThreadPtr = QCoreApplication::instance()->thread();
		Q_ASSERT(applicationThreadPtr != NULL);
	}

	QThread* objectThread = thread();

	// If event notifier lives in the application (main) thread, no parameter cloning is neccassary:
	if (objectThread == applicationThreadPtr){
		m_changeParamsPtr.SetPtr(changeParamsPtr, false);
	}
	else{
		if (changeParamsPtr != NULL){
			istd::IChangeable* changeablePtr = dynamic_cast<istd::IChangeable* >(changeParamsPtr);
			if (changeablePtr != NULL){
				istd::TOptDelPtr<istd::IChangeable> clonedParamsPtr(changeablePtr->CloneMe());
				if (clonedParamsPtr.IsValid()){
					m_changeParamsPtr.SetPtr(clonedParamsPtr.PopPtr(), true);
				}
				else{
					qDebug("Update parameter could not be cloned. Change notifying will be processed without parameter instance");
				}
			}
			else{
				qDebug("Notification parameter doesn't support clone operation. Change notifying will be processed without parameter instance");			
			}
		}
	}
}


void NotificationTarget::Reset()
{
	Q_EMIT EmitEndChanges();
}


// protected slots

void NotificationTarget::DoBeginChanges()
{
	Q_ASSERT(m_isBeginPending == true);

	m_isBeginPending = false;

	if (m_slavePtr != NULL){
		m_slavePtr->BeginChanges(m_changeFlags, m_changeParamsPtr.GetPtr());
	}
}


void NotificationTarget::DoEndChanges()
{
	Q_ASSERT(!m_isBeginPending);

	if ((m_slavePtr != NULL) && !m_isBeginPending){
		m_slavePtr->EndChanges(m_changeFlags, m_changeParamsPtr.GetPtr());
	}
}


} // namespace istd


