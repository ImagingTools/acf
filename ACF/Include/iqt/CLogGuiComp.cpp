#include "iqt/CLogGuiComp.h"


#include <QHeaderView>
#include <QFileDialog>
#include <QDateTime>


#ifdef OLD_ACF_SUPPORT


#include "Persist/FileWriteArchive.h"

namespace iqt
{

	
CLogGuiComp::CLogGuiComp()
{
	RegisterInterface<IMessageManager>(this);
}


void CLogGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	connect(this, SIGNAL(EmitAddMessage(ibase::IMessage*)), this, SLOT(OnAddMessage(ibase::IMessage*)), Qt::QueuedConnection);
	connect(ClearButton, SIGNAL(clicked()), this, SLOT(OnClear()));
	connect(ExportButton, SIGNAL(clicked()), this, SLOT(OnExport()));

	LogView->header()->setResizeMode(QHeaderView::Stretch);
	LogView->header()->hide();
}


// reimplemented (ibase::IMessageManager)

void CLogGuiComp::AddMessage(ibase::IMessage* messagePtr)
{
	I_ASSERT(messagePtr != NULL);

	m_lock.lock();

	BaseClass2::AddMessage(messagePtr);
	
	m_lock.unlock();

	emit EmitAddMessage(messagePtr);
}


// protected slots

void CLogGuiComp::OnAddMessage(ibase::IMessage* messagePtr)
{	
	m_lock.lock();

	QTreeWidgetItem* messageItemPtr = new QTreeWidgetItem();

	QDateTime dateTime;
	dateTime.fromTime_t(messagePtr->GetTimeStamp().ToCTime());

	messageItemPtr->setText(TimeColumn, dateTime.toString());
	messageItemPtr->setText(TextColumn, iqt::GetQString(messagePtr->GetText()));
	messageItemPtr->setText(SourceColumn, iqt::GetQString(messagePtr->GetSource()));

	LogView->addTopLevelItem(messageItemPtr);

	m_lock.unlock();
}


void CLogGuiComp::OnClear()
{
	m_lock.lock();
	
	ClearMessages();

	LogView->clear();

	m_lock.unlock();
}


void CLogGuiComp::OnExport()
{
}


} // namespace iqt


#endif // OLD_ACF_SUPPORT
