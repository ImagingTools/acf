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

	connect(this, SIGNAL(EmitAddMessage(QTreeWidgetItem*)), this, SLOT(OnAddMessage(QTreeWidgetItem*)), Qt::QueuedConnection);
	connect(this, SIGNAL(EmitRemoveMessage(int)), this, SLOT(OnRemoveMessage(int)), Qt::QueuedConnection);
	connect(this, SIGNAL(EmitReset()), this, SLOT(OnReset()), Qt::QueuedConnection);
}


// reimplemented (icomp::IComponent)

void CLogGuiComp::OnComponentCreated()
{
	if (m_maxMessageCountAttrPtr.IsValid()){
		SetMaxMessageCount(m_maxMessageCountAttrPtr->GetValue());
	}
}


// protected methods
	
QTreeWidgetItem* CLogGuiComp::CreateGuiItem(const ibase::IMessage& message)
{
	QTreeWidgetItem* treeItemPtr = new QTreeWidgetItem;
	if (treeItemPtr != NULL){
		QDateTime dateTime;
		dateTime = QDateTime::fromTime_t(message.GetTimeStamp().ToCTime());

		QString date = dateTime.toString();
		treeItemPtr->setText(TimeColumn, date);
		treeItemPtr->setText(MessageColumn, iqt::GetQString(message.GetText()));
		treeItemPtr->setText(SourceColumn, iqt::GetQString(message.GetSource()));

		treeItemPtr->setToolTip(TimeColumn, iqt::GetQString(message.GetText()));
		treeItemPtr->setToolTip(MessageColumn, iqt::GetQString(message.GetText()));
		treeItemPtr->setToolTip(SourceColumn, iqt::GetQString(message.GetText()));
		treeItemPtr->setData(0, MessageId, int(&message));
		treeItemPtr->setData(0, MessageCategory, message.GetCategory());

		QColor messageColor;
		switch (message.GetCategory()){
		case ibase::IMessage::MC_WARNING:
			messageColor = QColor(235, 235, 0);
			break;

		case ibase::IMessage::MC_ERROR:
			messageColor = QColor(255, 0, 255, 128);
			break;

		case ibase::IMessage::MC_CRITICAL:
			messageColor = QColor(255, 0, 0);
			break;

		default:
			messageColor = QColor(0,0,0,0);
			break;
		}

		treeItemPtr->setBackgroundColor(TimeColumn, messageColor);
		treeItemPtr->setBackgroundColor(SourceColumn, messageColor);
		treeItemPtr->setBackgroundColor(MessageColumn, messageColor);
	}

	return treeItemPtr;
}


void CLogGuiComp::UpdateItemState(QTreeWidgetItem& item) const
{
	int currentCategory = CategorySlider->value();

	item.setHidden(item.data(0, MessageCategory).toInt() < currentCategory);
}


// reimplemented (CGuiComponentBase)

void CLogGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	LogView->header()->setStretchLastSection(true);

	ExportButton->setVisible(m_fileLoaderCompPtr.IsValid());

	Messages messages = GetMessages();
	for (		Messages::const_iterator iter = messages.begin();
				iter != messages.end();
				++iter){
		const ibase::IMessage* messagePtr = *iter;
		if (messagePtr != NULL){
			QTreeWidgetItem* itemPtr = CreateGuiItem(*messagePtr);
			if (itemPtr != NULL){
				OnAddMessage(itemPtr);
			}
		}
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
	if (((changeFlags & ibase::IMessageContainer::MessageAdded) != 0) && IsGuiCreated()){
		ibase::IMessage* messagePtr = dynamic_cast<ibase::IMessage*>(changeParamsPtr);
		if (messagePtr != NULL){
			QTreeWidgetItem* itemPtr = CreateGuiItem(*messagePtr);
			if (itemPtr != NULL){
				emit EmitAddMessage(itemPtr);
			}
		}
	}

	BaseClass2::OnEndChanges(changeFlags, changeParamsPtr);
}


// protected slots

void CLogGuiComp::OnAddMessage(QTreeWidgetItem* itemPtr)
{	
	I_ASSERT(itemPtr != NULL);

	UpdateItemState(*itemPtr);

	// add message item to the list
	LogView->addTopLevelItem(itemPtr);
}


void CLogGuiComp::OnRemoveMessage(int id)
{
	for (int itemIndex = 0; itemIndex < LogView->topLevelItemCount(); itemIndex++){
		QTreeWidgetItem* itemPtr = LogView->topLevelItem(itemIndex);
		I_ASSERT(itemPtr != NULL);

		if (itemPtr->data(0, MessageId).toInt() == id){
			delete LogView->takeTopLevelItem(itemIndex);

			break;
		}
	}
}


void CLogGuiComp::OnReset()
{
	LogView->clear();
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
		QTreeWidgetItem* itemPtr = LogView->topLevelItem(itemIndex);
		I_ASSERT(itemPtr != NULL);

		UpdateItemState(*itemPtr);
	}

	CategoryLabel->setText(m_categoryNameMap[category]);
}


} // namespace iqtgui


