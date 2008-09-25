#include "iqtgui/CLogGuiComp.h"


#include <QHeaderView>
#include <QDateTime>


namespace iqtgui
{


// public methods

CLogGuiComp::CLogGuiComp()
{
	m_categoryNameMap[ibase::IMessage::MC_INFO] = tr("Info");
	m_categoryNameMap[ibase::IMessage::MC_WARNING] = tr("Warning");
	m_categoryNameMap[ibase::IMessage::MC_ERROR] = tr("Error");
	m_categoryNameMap[ibase::IMessage::MC_CRITICAL] = tr("Critical");

	connect(this, SIGNAL(EmitAddMessage(CMessageInfo*)), this, SLOT(OnAddMessage(CMessageInfo*)), Qt::QueuedConnection);
	connect(this, SIGNAL(EmitRemoveMessage(int)), this, SLOT(OnRemoveMessage(int)), Qt::QueuedConnection);
	connect(this, SIGNAL(EmitReset()), this, SLOT(OnReset()), Qt::QueuedConnection);
}


// protected methods
	
// reimplemented (CGuiComponentBase)

void CLogGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	LogView->header()->setStretchLastSection(true);

	ExportButton->setVisible(m_fileLoaderCompPtr.IsValid());

	if (m_maxMessageCountAttrPtr.IsValid()){
		SetMaxMessageCount(m_maxMessageCountAttrPtr->GetValue());
	}
}


// reimplemented (istd::IChangeable)

void CLogGuiComp::OnBeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	if (changeFlags & ibase::IMessageContainer::Reset){
		emit EmitReset();
	}

	if (changeFlags & ibase::IMessageContainer::MessageRemoved){
		ibase::IMessage* messagePtr = dynamic_cast<ibase::IMessage*>(changeParamsPtr);
		if (messagePtr != NULL){
			emit EmitRemoveMessage(int(messagePtr));
		}
	}

	BaseClass2::OnBeginChanges(changeFlags, changeParamsPtr);	
}


void CLogGuiComp::OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	if (changeFlags & ibase::IMessageContainer::MessageAdded){
		ibase::IMessage* messagePtr = dynamic_cast<ibase::IMessage*>(changeParamsPtr);
		if (messagePtr != NULL){
			CMessageInfo* messageItemPtr = new CMessageInfo(
						int(messagePtr),
						messagePtr->GetCategory(),
						messagePtr->GetId(),
						messagePtr->GetText(),
						messagePtr->GetSource(),
						messagePtr->GetFlags());

			emit EmitAddMessage(messageItemPtr);
		}
	}

	BaseClass2::OnEndChanges(changeFlags, changeParamsPtr);
}


// protected slots

void CLogGuiComp::OnAddMessage(CMessageInfo* messagePtr)
{	
	I_ASSERT(messagePtr != NULL);

	QDateTime dateTime;
	dateTime = QDateTime::fromTime_t(messagePtr->GetTimeStamp().ToCTime());

	CMessageTreeItem* treeItemPtr = new CMessageTreeItem;
	treeItemPtr->m_messagePtr.SetPtr(messagePtr);

	QString date = dateTime.toString();
	treeItemPtr->setText(TimeColumn, date);
	treeItemPtr->setText(MessageColumn, iqt::GetQString(messagePtr->GetText()));
	treeItemPtr->setText(SourceColumn, iqt::GetQString(messagePtr->GetSource()));

	treeItemPtr->setToolTip(TimeColumn, iqt::GetQString(messagePtr->GetText()));
	treeItemPtr->setToolTip(MessageColumn, iqt::GetQString(messagePtr->GetText()));
	treeItemPtr->setToolTip(SourceColumn, iqt::GetQString(messagePtr->GetText()));

	QColor messageColor = GetMessageColor(*treeItemPtr);

	treeItemPtr->setBackgroundColor(TimeColumn, messageColor);
	treeItemPtr->setBackgroundColor(SourceColumn, messageColor);
	treeItemPtr->setBackgroundColor(MessageColumn, messageColor);

	// add message item to the list
	LogView->addTopLevelItem(treeItemPtr);

	if (NeedToBeHidden(*treeItemPtr)){
		treeItemPtr->setHidden(true);
	}
}


void CLogGuiComp::OnRemoveMessage(int messageId)
{
	for (int itemIndex = 0; itemIndex < LogView->topLevelItemCount(); itemIndex++){
		CMessageTreeItem* treeMessageItemPtr = dynamic_cast<CMessageTreeItem*>(LogView->topLevelItem(itemIndex));
		I_ASSERT(treeMessageItemPtr != NULL);
		I_ASSERT(treeMessageItemPtr->m_messagePtr.IsValid());

		if (treeMessageItemPtr->m_messagePtr.GetPtr()->m_messageId == messageId){
			LogView->takeTopLevelItem(itemIndex);

			delete treeMessageItemPtr;

			return;
		}
	}
}


void CLogGuiComp::OnReset()
{
	while(LogView->topLevelItemCount()){
		CMessageTreeItem* treeMessageItemPtr = dynamic_cast<CMessageTreeItem*>(LogView->topLevelItem(0));
		I_ASSERT(treeMessageItemPtr != NULL);

		LogView->takeTopLevelItem(0);

		delete treeMessageItemPtr;
	}
}


void CLogGuiComp::on_ClearButton_clicked()
{
	ClearMessages();
}


void CLogGuiComp::on_ExportButton_clicked()
{
	if (m_fileLoaderCompPtr.IsValid()){
		m_fileLoaderCompPtr->SaveToFile(*this, istd::CString());
	}
}


void CLogGuiComp::on_CategorySlider_valueChanged(int category)
{
	for (int itemIndex = 0; itemIndex < LogView->topLevelItemCount(); itemIndex++){
		CMessageTreeItem* messageItemPtr = dynamic_cast<CMessageTreeItem*>(LogView->topLevelItem(itemIndex));
		I_ASSERT(messageItemPtr != NULL);

		if (NeedToBeHidden(*messageItemPtr)){
			messageItemPtr->setHidden(true);
		}
		else{
			messageItemPtr->setHidden(false);
		}
	}

	CategoryLabel->setText(m_categoryNameMap[category]);
}


// private methods

QColor CLogGuiComp::GetMessageColor(const CMessageTreeItem& message) const
{
	QColor messageColor(0,0,0,0);

	I_ASSERT(message.m_messagePtr.IsValid());

	int category = message.m_messagePtr->GetCategory();

	switch(category){
		case ibase::IMessage::MC_WARNING:
			messageColor = QColor(235, 235, 0);
			break;

		case ibase::IMessage::MC_ERROR:
			messageColor = QColor(255, 0, 255, 128);
			break;

		case ibase::IMessage::MC_CRITICAL:
			messageColor = QColor(255, 0, 0);
			break;
	}

	return messageColor;
}


bool CLogGuiComp::NeedToBeHidden(const CMessageTreeItem& message) const
{
	I_ASSERT(message.m_messagePtr.IsValid());

	int currentCategory = CategorySlider->value();

	int itemCategory = message.m_messagePtr->GetCategory();
	if (itemCategory < currentCategory){
		return true;
	}

	return false;
}


} // namespace iqtgui


