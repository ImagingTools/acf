#include "iqt/CLogGuiComp.h"


#include <QHeaderView>
#include <QDateTime>


namespace iqt
{


// public methods

// reimplemented (ibase::IMessageContainer)

void CLogGuiComp::AddMessage(ibase::IMessage* messagePtr)
{
	I_ASSERT(messagePtr != NULL);

	m_lock.lock();

	BaseClass2::AddMessage(messagePtr);
	
	m_lock.unlock();

	emit EmitAddMessage(messagePtr);
}


// protected methods
	
// reimplemented (CGuiComponentBase)

void CLogGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	connect(this, SIGNAL(EmitAddMessage(ibase::IMessage*)), this, SLOT(OnAddMessage(ibase::IMessage*)), Qt::QueuedConnection);
	connect(ClearButton, SIGNAL(clicked()), this, SLOT(OnClear()));
	connect(ExportButton, SIGNAL(clicked()), this, SLOT(OnExport()));

	LogView->header()->setResizeMode(QHeaderView::Stretch);
	LogView->header()->hide();

	ExportButton->setEnabled(m_fileSerializerCompPtr.IsValid());
}


// protected slots

void CLogGuiComp::OnAddMessage(ibase::IMessage* messagePtr)
{	
	m_lock.lock();

	QTreeWidgetItem* messageItemPtr = new QTreeWidgetItem();

	QDateTime dateTime;
	dateTime = QDateTime::fromTime_t(messagePtr->GetTimeStamp().ToCTime());

	QString date = dateTime.toString();
	messageItemPtr->setText(TimeColumn, date);
	messageItemPtr->setText(TextColumn, iqt::GetQString(messagePtr->GetText()));
	messageItemPtr->setText(SourceColumn, iqt::GetQString(messagePtr->GetSource()));

	QColor categoryColor(0,0,0,0);

	int category = SubstractMask(messagePtr->GetCategory());

	switch(category){
		case ibase::IMessage::Warning:
			categoryColor = QColor(200, 200, 0, 128);
			break;

		case ibase::IMessage::Error:
			categoryColor = QColor(255, 0, 0, 128);
			break;

		case ibase::IMessage::Critical:
			categoryColor = QColor(255, 0, 0, 255);
			break;
	}

	messageItemPtr->setBackgroundColor(TimeColumn, categoryColor);
	messageItemPtr->setBackgroundColor(SourceColumn, categoryColor);
	messageItemPtr->setBackgroundColor(TextColumn, categoryColor);

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
	if (m_fileSerializerCompPtr.IsValid()){
		m_fileSerializerCompPtr->SaveToFile(*this, istd::CString());
	}
}


} // namespace iqt



