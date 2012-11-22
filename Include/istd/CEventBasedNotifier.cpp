#include "istd/CEventBasedNotifier.h"


// Qt includes
#include <QtCore/QCoreApplication>


namespace istd
{


// public methods

CEventBasedNotifier::CEventBasedNotifier(istd::IChangeable* slavePtr,  int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	m_notificationTarget = new NotificationTarget(slavePtr, changeFlags, changeParamsPtr);
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
	m_changeParamsPtr(changeParamsPtr),
	m_isBeginPending(true)
{
	moveToThread(QCoreApplication::instance()->thread());

	connect(this, SIGNAL(EmitBeginChanges(int, istd::IPolymorphic*)), this, SLOT(DoBeginChanges(int, istd::IPolymorphic*)));
	connect(this, SIGNAL(EmitEndChanges(int, istd::IPolymorphic*)), this, SLOT(DoEndChanges(int, istd::IPolymorphic*)));

	Q_EMIT EmitBeginChanges(changeFlags, changeParamsPtr);
}


void NotificationTarget::Reset()
{
	I_ASSERT(!m_isBeginPending);

	Q_EMIT EmitEndChanges(m_changeFlags, m_changeParamsPtr);

	m_slavePtr = NULL;
}


// protected slots

void NotificationTarget::DoBeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	m_isBeginPending = false;

	if (m_slavePtr != NULL){
		m_slavePtr->BeginChanges(changeFlags, changeParamsPtr);
	}
}


void NotificationTarget::DoEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	if (m_slavePtr != NULL && !m_isBeginPending){
		m_slavePtr->EndChanges(changeFlags, changeParamsPtr);
	}
}


} // namespace istd


