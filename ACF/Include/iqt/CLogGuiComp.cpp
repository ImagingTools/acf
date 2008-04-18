#include "iqt/CLogGuiComp.h"


#include <QHeaderView>
#include <QDateTime>


namespace iqt
{


// public methods

CLogGuiComp::CLogGuiComp()
	:m_logObserver(*this)
{
	m_categoryNameMap[0] = tr("Info");
	m_categoryNameMap[1] = tr("Warning");
	m_categoryNameMap[2] = tr("Error");
	m_categoryNameMap[3] = tr("Critical");
}


// protected methods
	
// reimplemented (CGuiComponentBase)

void CLogGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	connect(this, SIGNAL(EmitAddMessage(ibase::IMessage*)), this, SLOT(OnAddMessage(ibase::IMessage*)), Qt::QueuedConnection);
	connect(this, SIGNAL(EmitRemoveMessage(ibase::IMessage*)), this, SLOT(OnRemoveMessage(ibase::IMessage*)), Qt::QueuedConnection);
	connect(ClearButton, SIGNAL(clicked()), this, SLOT(OnClear()));
	connect(ExportButton, SIGNAL(clicked()), this, SLOT(OnExport()));
	connect(CategorySlider, SIGNAL(valueChanged(int)), this, SLOT(OnCategoryChanged(int)));

	LogView->header()->setResizeMode(QHeaderView::Stretch);

	ExportButton->setVisible(m_fileSerializerCompPtr.IsValid());

	if (m_maxMessageCountAttrPtr.IsValid()){
		SetMaxMessageCount(m_maxMessageCountAttrPtr->GetValue());
	}

	AttachObserver(&m_logObserver);
}


// protected slots

void CLogGuiComp::OnAddMessage(ibase::IMessage* messagePtr)
{	
	I_ASSERT(messagePtr != NULL);

	CMessageItem* messageItemPtr = new CMessageItem();
	messageItemPtr->messagePtr = messagePtr;

	QDateTime dateTime;
	dateTime = QDateTime::fromTime_t(messagePtr->GetTimeStamp().ToCTime());

	QString date = dateTime.toString();
	messageItemPtr->setText(TimeColumn, date);
	messageItemPtr->setText(TextColumn, iqt::GetQString(messagePtr->GetText()));
	messageItemPtr->setText(SourceColumn, iqt::GetQString(messagePtr->GetSource()));

	QColor messageColor = GetMessageColor(*messagePtr);

	messageItemPtr->setBackgroundColor(TimeColumn, messageColor);
	messageItemPtr->setBackgroundColor(SourceColumn, messageColor);
	messageItemPtr->setBackgroundColor(TextColumn, messageColor);

	LogView->addTopLevelItem(messageItemPtr);
	if (NeedToBeHidden(*messagePtr)){
		messageItemPtr->setHidden(true);
	}
}


void CLogGuiComp::OnRemoveMessage(ibase::IMessage* messagePtr)
{
	I_ASSERT(messagePtr != NULL);

	for (int itemIndex = 0; itemIndex < LogView->topLevelItemCount(); itemIndex++){
		CMessageItem* messageItemPtr = dynamic_cast<CMessageItem*>(LogView->topLevelItem(itemIndex));
		I_ASSERT(messageItemPtr != NULL);

		if (messageItemPtr->messagePtr == messagePtr){
			LogView->takeTopLevelItem(itemIndex);

			return;
		}
	}
}


void CLogGuiComp::OnClear()
{
	ClearMessages();
}


void CLogGuiComp::OnExport()
{
	if (m_fileSerializerCompPtr.IsValid()){
		m_fileSerializerCompPtr->SaveToFile(*this, istd::CString());
	}
}


void CLogGuiComp::OnCategoryChanged(int category)
{
	for (int itemIndex = 0; itemIndex < LogView->topLevelItemCount(); itemIndex++){
		CMessageItem* messageItemPtr = dynamic_cast<CMessageItem*>(LogView->topLevelItem(itemIndex));
		I_ASSERT(messageItemPtr != NULL);

		if (NeedToBeHidden(*messageItemPtr->messagePtr)){
			messageItemPtr->setHidden(true);
		}
		else{
			messageItemPtr->setHidden(false);
		}
	}

	CategoryLabel->setText(m_categoryNameMap[category]);
}


// private methods

QColor CLogGuiComp::GetMessageColor(const ibase::IMessage& message) const
{
	QColor messageColor(0,0,0,0);

	int category = SubstractMask(message.GetCategory());

	switch(category){
		case ibase::IMessage::Warning:
			messageColor = QColor(235, 235, 0);
			break;

		case ibase::IMessage::Error:
			messageColor = QColor(255, 0, 255, 128);
			break;

		case ibase::IMessage::Critical:
			messageColor = QColor(255, 0, 0);
			break;
	}

	return messageColor;
}

	
bool CLogGuiComp::NeedToBeHidden(const ibase::IMessage& message) const
{
	int currentCategory = CategorySlider->value();

	int itemCategory = SubstractMask(message.GetCategory());
	if (itemCategory < currentCategory){
		return true;
	}

	return false;
}



// public methods of embedded class LogObserver
	
CLogGuiComp::LogObserver::LogObserver(CLogGuiComp& parent)
	:m_parent(parent)
{
}


// protected methods of embedded class LogObserver

// reimplemented (imod::CSingleModelObserverBase)

void CLogGuiComp::LogObserver::BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	if (updateFlags & ibase::IMessageContainer::Reset){
		m_parent.LogView->clear();
	}

	if (updateFlags & ibase::IMessageContainer::MessageRemoved){
		ibase::IMessage* messagePtr = dynamic_cast<ibase::IMessage*>(updateParamsPtr);
		if (messagePtr != NULL){
			emit m_parent.EmitRemoveMessage(messagePtr);
		}
	}

	BaseClass::BeforeUpdate(modelPtr, updateFlags, updateParamsPtr);	
}


void CLogGuiComp::LogObserver::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	if (updateFlags & ibase::IMessageContainer::MessageAdded){
		ibase::IMessage* messagePtr = dynamic_cast<ibase::IMessage*>(updateParamsPtr);
		if (messagePtr != NULL){
			emit m_parent.EmitAddMessage(messagePtr);
		}
	}

	BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
}


} // namespace iqt


