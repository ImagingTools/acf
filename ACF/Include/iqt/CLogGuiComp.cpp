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
	RegisterInterface<QtGuiInterface>(this);
}


void CLogGuiComp::initializeGui()
{
	BaseClass::initializeGui();

	connect(this, SIGNAL(EmitUpdate()), this, SLOT(UpdateLog()), Qt::QueuedConnection);
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

	emit EmitUpdate();
}


// protected methods

// reimplemented (imod::TMultiModelObserverBase)

void CLogGuiComp::OnUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{

}


// protected slots

void CLogGuiComp::UpdateLog()
{	
	m_lock.lock();
	int rows = LogView->topLevelItemCount();
	int objects = GetModelCount();
	
	for (int index = rows; index < objects; index++){
		ibase::IMessage* messagePtr = GetObjectPtr(index);
		I_ASSERT(messagePtr != NULL);

		QTreeWidgetItem* logItemPtr = new QTreeWidgetItem();

		QDateTime dateTime;
		dateTime.fromTime_t(messagePtr->GetTimeStamp().ToCTime());

		logItemPtr->setText(TimeColumn, dateTime.toString());
		logItemPtr->setText(TextColumn, iqt::GetQString(messagePtr->GetText()));
		logItemPtr->setText(SourceColumn, iqt::GetQString(messagePtr->GetSource()));

		LogView->addTopLevelItem(logItemPtr);
	}

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
