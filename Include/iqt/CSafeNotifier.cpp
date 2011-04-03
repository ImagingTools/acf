#include "iqt/CSafeNotifier.h"


// Qt includes
#include <QApplication>


namespace iqt
{


	
NotificationTarget::NotificationTarget(istd::IChangeable* slavePtr, int changeFlags, istd::IPolymorphic* changeParamsPtr)
:	BaseClass(slavePtr),
	m_changeFlags(changeFlags),
	m_changeParamsPtr(changeParamsPtr)
{
	moveToThread(QApplication::instance()->thread());

	connect(this, SIGNAL(EmitEndChanges(int, istd::IPolymorphic*)), this, SLOT(DoEndChanges(int, istd::IPolymorphic*)));

	BaseClass::BeginChanges(changeFlags, changeParamsPtr);
}


void NotificationTarget::Reset()
{
	EndChanges(m_changeFlags, m_changeParamsPtr);
}


// reimplemented (istd::IChangeable)

void NotificationTarget::EndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	Q_EMIT EmitEndChanges(changeFlags, changeParamsPtr);
}

// protected slots:

void NotificationTarget::DoEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass::EndChanges(changeFlags, changeParamsPtr);
}


CSafeNotifier::CSafeNotifier(istd::IChangeable* slavePtr,  int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	m_notificationTarget = new NotificationTarget(slavePtr, changeFlags, changeParamsPtr);
}


CSafeNotifier::~CSafeNotifier()
{
	m_notificationTarget->Reset();

	m_notificationTarget->deleteLater();
}


} // namespace iqt


