#include "iqtgui/CLogGuiComp.h"


// Qt includes
#include <QHeaderView>
#include <QDateTime>
#include <QToolBar>
#include <QPainter>


// ACF includes
#include "iqtgui/CItemDelegate.h"


namespace iqtgui
{


// public methods

CLogGuiComp::CLogGuiComp()
	:m_currentMessageMode(MM_ALL)
{
	qRegisterMetaType<QVariant>("QVariant");

	connect(this, SIGNAL(EmitAddMessage(QTreeWidgetItem*)), this, SLOT(OnAddMessage(QTreeWidgetItem*)), Qt::QueuedConnection);
	connect(this, SIGNAL(EmitRemoveMessage(QVariant)), this, SLOT(OnRemoveMessage(QVariant)), Qt::QueuedConnection);
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
		dateTime = QDateTime::fromTime_t(uint(message.GetTimeStamp().ToCTime()));

		QString date = dateTime.toString();

		treeItemPtr->setText(CT_TIME, date);
		treeItemPtr->setText(CT_MESSAGE, iqt::GetQString(message.GetText()));
		treeItemPtr->setText(CT_SOURCE, iqt::GetQString(message.GetSource()));

		treeItemPtr->setToolTip(CT_TIME, iqt::GetQString(message.GetText()));
		treeItemPtr->setToolTip(CT_MESSAGE, iqt::GetQString(message.GetText()));
		treeItemPtr->setToolTip(CT_SOURCE, iqt::GetQString(message.GetText()));
		treeItemPtr->setData(0, DR_MESSAGE_ID, QVariant::fromValue((void*)&message));
		treeItemPtr->setData(0, DR_CATEGORY, message.GetCategory());

		QIcon messageIcon;
		switch (message.GetCategory()){
		case istd::ILogger::MC_WARNING:
			messageIcon = m_warningIcon;
			break;

		case istd::ILogger::MC_ERROR:
		case istd::ILogger::MC_CRITICAL:
			messageIcon = m_errorIcon;
			break;

		case istd::ILogger::MC_INFO:
			messageIcon = m_infoIcon;
			break;
		}

		treeItemPtr->setIcon(CT_ICON, messageIcon);
	}

	return treeItemPtr;
}


void CLogGuiComp::UpdateItemState(QTreeWidgetItem& item) const
{
	int itemCategory = item.data(0, DR_CATEGORY).toInt();

	item.setHidden(itemCategory < m_currentMessageMode);
}


// reimplemented (CGuiComponentBase)

void CLogGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	LogView->header()->setResizeMode(QHeaderView::ResizeToContents);
	LogView->header()->setStretchLastSection(true);

	iqtgui::CItemDelegate* itemDelegate = new iqtgui::CItemDelegate(20, this);
	LogView->setItemDelegate(itemDelegate);

	LogView->header()->hide();

	LogView->header()->setResizeMode(CT_ICON, QHeaderView::Fixed);
	LogView->header()->resizeSection(CT_ICON, itemDelegate->GetItemHeight());

	QToolBar* toolBar = new QToolBar(ToolBarFrame);
	if (ToolBarFrame->layout()){
		ToolBarFrame->layout()->addWidget(toolBar);
	}

	QActionGroup* actionGroup = new QActionGroup(this);
	actionGroup->setExclusive(true);

	QAction* infoAction = new QAction(QIcon(QString::fromUtf8(":/Icons/Info.svg")), tr("Info"), ToolBarFrame);
	infoAction->setCheckable(true);
	infoAction->setData(MM_INFO);
	connect(infoAction, SIGNAL(toggled(bool)), this, SLOT(OnMessageModeChanged()), Qt::QueuedConnection);
	actionGroup->addAction(infoAction);
	if (*m_defaultModeAttrPtr == 0){
		infoAction->setChecked(true);
	}

	QAction* warningAction = new QAction(QIcon(":/Icons/Warning.svg"), tr("Warning"), ToolBarFrame);
	warningAction->setCheckable(true);
	warningAction->setData(MM_WARNING);
	connect(warningAction, SIGNAL(toggled(bool)), this, SLOT(OnMessageModeChanged()), Qt::QueuedConnection);
	actionGroup->addAction(warningAction);
	if (*m_defaultModeAttrPtr == 1){
		warningAction->setChecked(true);
	}

	QAction* errorAction = new QAction(QIcon(QString::fromUtf8(":/Icons/Error.svg")), tr("Error"), ToolBarFrame);
	errorAction->setCheckable(true);
	errorAction->setData(MM_ERROR);
	connect(errorAction, SIGNAL(toggled(bool)), this, SLOT(OnMessageModeChanged()), Qt::QueuedConnection);
	actionGroup->addAction(errorAction);
	if (*m_defaultModeAttrPtr == 2){
		errorAction->setChecked(true);
	}

	QAction* clearAction = new QAction(QIcon(QString::fromUtf8(":/Icons/Clear")), tr("Clear"), ToolBarFrame);
	connect(clearAction, SIGNAL(triggered()), this, SLOT(OnClearAction()), Qt::QueuedConnection);

	toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	toolBar->setIconSize(QSize(16, 16));
	toolBar->addAction(infoAction);
	toolBar->addAction(warningAction);
	toolBar->addAction(errorAction);
	toolBar->addAction(clearAction);
	toolBar->insertSeparator(clearAction);

	if (m_fileLoaderCompPtr.IsValid()){
		QAction* exportAction = new QAction(QIcon(QString::fromUtf8(":/Icons/DocumentExport.svg")), tr("Export..."), ToolBarFrame);
		connect(exportAction, SIGNAL(triggered()), this, SLOT(OnExportAction()), Qt::QueuedConnection);		
		toolBar->addAction(exportAction);
		toolBar->insertSeparator(exportAction);
	}

	m_infoIcon = QIcon(":/Icons/Info.svg").pixmap(QSize(12, 12),QIcon::Normal, QIcon::On);
	m_warningIcon = QIcon(":/Icons/Warning.svg").pixmap(QSize(12, 12),QIcon::Normal, QIcon::On);
	m_errorIcon = QIcon(":/Icons/Error.svg").pixmap(QSize(12, 12),QIcon::Normal, QIcon::On);

	Messages messages = GetMessages();
	for (		Messages::const_iterator iter = messages.begin();
				iter != messages.end();
				++iter){
		const MessagePtr& messagePtr = *iter;
		if (messagePtr.IsValid()){
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
		Q_EMIT EmitReset();
	}

	if (changeFlags & ibase::IMessageContainer::MessageRemoved){
		ibase::IMessage* messagePtr = dynamic_cast<ibase::IMessage*>(changeParamsPtr);
		if (messagePtr != NULL){
			Q_EMIT EmitRemoveMessage(QVariant::fromValue((void*)messagePtr));
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
				Q_EMIT EmitAddMessage(itemPtr);
			}
		}
	}

	BaseClass2::OnEndChanges(changeFlags, changeParamsPtr);
}


// protected slots

void CLogGuiComp::OnAddMessage(QTreeWidgetItem* itemPtr)
{	
	I_ASSERT(itemPtr != NULL);

	// add message item to the list
	LogView->addTopLevelItem(itemPtr);

	UpdateItemState(*itemPtr);
}


void CLogGuiComp::OnRemoveMessage(QVariant messageId)
{
	for (int itemIndex = 0; itemIndex < LogView->topLevelItemCount(); itemIndex++){
		QTreeWidgetItem* itemPtr = LogView->topLevelItem(itemIndex);
		I_ASSERT(itemPtr != NULL);

		if (itemPtr->data(0, DR_MESSAGE_ID) == messageId){
			delete LogView->takeTopLevelItem(itemIndex);

			break;
		}
	}
}


void CLogGuiComp::OnReset()
{
	LogView->clear();
}


void CLogGuiComp::OnMessageModeChanged()
{
	QAction* actionPtr = dynamic_cast<QAction*> (sender());
	if (actionPtr != NULL){
		m_currentMessageMode = actionPtr->data().toInt();
	
	}

	for (int itemIndex = 0; itemIndex < LogView->topLevelItemCount(); itemIndex++){
		QTreeWidgetItem* itemPtr = LogView->topLevelItem(itemIndex);
		I_ASSERT(itemPtr != NULL);

		UpdateItemState(*itemPtr);
	}
}


void CLogGuiComp::OnClearAction()
{
	ClearMessages();
}


void CLogGuiComp::OnExportAction()
{
	if (m_fileLoaderCompPtr.IsValid()){
		m_fileLoaderCompPtr->SaveToFile(*this);
	}
}



} // namespace iqtgui


