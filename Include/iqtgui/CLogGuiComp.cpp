#include "iqtgui/CLogGuiComp.h"


// Qt includes
#include <QtCore/QDateTime>
#include <QtGui/QHeaderView>
#include <QtGui/QToolBar>
#include <QtGui/QPainter>

// ACF includes
#include "iqtgui/CItemDelegate.h"


namespace iqtgui
{


// public methods

CLogGuiComp::CLogGuiComp()
:	m_infoAction(NULL),
	m_warningAction(NULL),
	m_errorAction(NULL),
	m_clearAction(NULL),
	m_exportAction(NULL),
	m_currentMessageMode(MM_ALL)
{
	qRegisterMetaType<QVariant>("QVariant");

	connect(this, SIGNAL(EmitAddMessage(QTreeWidgetItem*)), this, SLOT(OnAddMessage(QTreeWidgetItem*)), Qt::QueuedConnection);
	connect(this, SIGNAL(EmitRemoveMessage(QVariant)), this, SLOT(OnRemoveMessage(QVariant)), Qt::QueuedConnection);
	connect(this, SIGNAL(EmitReset()), this, SLOT(OnReset()), Qt::QueuedConnection);
}


// reimplemented (istd::IVisualStatusProvider)

QIcon CLogGuiComp::GetStatusIcon() const
{
	static QIcon infoIcon(":/Icons/Info.svg");
	static QIcon warningIcon(":/Icons/Warning.svg");
	static QIcon errorIcon(":/Icons/Error.svg");
	static QIcon logIcon(":/Icons/Log");

	int worstCategory = -1;
	
	ibase::IMessageContainer* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		worstCategory = objectPtr->GetWorstCategory();
	}

	switch (worstCategory){
	case istd::IInformationProvider::IC_INFO:
		return infoIcon;

	case istd::IInformationProvider::IC_WARNING:
		return warningIcon;

	case istd::IInformationProvider::IC_ERROR:
	case istd::IInformationProvider::IC_CRITICAL:
		return errorIcon;

	default:
		return logIcon;
	}
}


QString CLogGuiComp::GetStatusText() const
{
	int worstCategory = -1;
	
	ibase::IMessageContainer* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		worstCategory = objectPtr->GetWorstCategory();
	}

	switch (worstCategory){
	case istd::IInformationProvider::IC_INFO:
		return tr("New informations available");

	case istd::IInformationProvider::IC_WARNING:
		return tr("There are warnings");

	case istd::IInformationProvider::IC_ERROR:
		return tr("There are errors");

	case istd::IInformationProvider::IC_CRITICAL:
		return tr("There are critical errors!");

	default:
		return "";
	}
}


// protected methods
	
QTreeWidgetItem* CLogGuiComp::CreateGuiItem(const istd::IInformationProvider& message)
{
	QTreeWidgetItem* treeItemPtr = new QTreeWidgetItem;
	if (treeItemPtr != NULL){
		QDateTime timeStamp = message.GetInformationTimeStamp();

		QString date = timeStamp.toString();

		treeItemPtr->setText(CT_TIME, date);
		treeItemPtr->setText(CT_MESSAGE, message.GetInformationDescription());
		treeItemPtr->setText(CT_SOURCE, message.GetInformationSource());

		treeItemPtr->setToolTip(CT_TIME, message.GetInformationDescription());
		treeItemPtr->setToolTip(CT_MESSAGE, message.GetInformationDescription());
		treeItemPtr->setToolTip(CT_SOURCE, message.GetInformationDescription());
		treeItemPtr->setData(0, DR_MESSAGE_ID, QVariant::fromValue((void*)&message));
		treeItemPtr->setData(0, DR_CATEGORY, message.GetInformationCategory());

		QIcon messageIcon = GetIcon(message.GetInformationCategory());

		treeItemPtr->setIcon(CT_ICON, messageIcon);
	}

	return treeItemPtr;
}


void CLogGuiComp::UpdateItemState(QTreeWidgetItem& item) const
{
	int itemCategory = item.data(0, DR_CATEGORY).toInt();

	item.setHidden(itemCategory < m_currentMessageMode);
}


QIcon CLogGuiComp::GetIcon(istd::IInformationProvider::InformationCategory mode)
{
	QIcon infoIcon = QIcon(":/Icons/Info.svg").pixmap(QSize(12, 12),QIcon::Normal, QIcon::On);
	QIcon warningIcon = QIcon(":/Icons/Warning.svg").pixmap(QSize(12, 12),QIcon::Normal, QIcon::On);
	QIcon errorIcon = QIcon(":/Icons/Error.svg").pixmap(QSize(12, 12),QIcon::Normal, QIcon::On);

	QIcon messageIcon;
	switch (mode)
	{
	case istd::IInformationProvider::IC_WARNING:
		messageIcon = warningIcon;
		break;

	case istd::IInformationProvider::IC_ERROR:
	case istd::IInformationProvider::IC_CRITICAL:
		messageIcon = errorIcon;
		break;

	case istd::IInformationProvider::IC_INFO:
		messageIcon = infoIcon;
		break;
	}

	return messageIcon;
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CLogGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ibase::IMessageContainer* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		ibase::IMessageContainer::Messages messages = objectPtr->GetMessages();
		for (		ibase::IMessageContainer::Messages::const_iterator iter = messages.begin();
					iter != messages.end();
					++iter){
			const ibase::IMessageContainer::MessagePtr& messagePtr = *iter;
			if (messagePtr.IsValid()){
				QTreeWidgetItem* itemPtr = CreateGuiItem(*messagePtr);
				if (itemPtr != NULL){
					OnAddMessage(itemPtr);
				}
			}
		}
	}
}


// reimplemented (CGuiComponentBase)

void CLogGuiComp::OnGuiCreated()
{
	LogView->header()->setResizeMode(QHeaderView::ResizeToContents);
	LogView->header()->setStretchLastSection(true);

	iqtgui::CItemDelegate* itemDelegate = new iqtgui::CItemDelegate(20, this);
	LogView->setItemDelegate(itemDelegate);
	LogView->header()->hide();

	if(m_showLogDescriptionAttrPtr.IsValid() && *m_showLogDescriptionAttrPtr){
		LogView->header()->show();
	}

	LogView->setSortingEnabled(true);
	LogView->header()->setResizeMode(CT_ICON, QHeaderView::Fixed);
	LogView->header()->resizeSection(CT_ICON, itemDelegate->GetItemHeight());

	QToolBar* toolBar = new QToolBar(ToolBarFrame);
	if (ToolBarFrame->layout()){
		ToolBarFrame->layout()->addWidget(toolBar);
	}

	QActionGroup* actionGroup = new QActionGroup(this);
	actionGroup->setExclusive(true);

	static QIcon infoIcon(":/Icons/Info.svg");
	m_infoAction = new QAction(infoIcon, tr("Info"), ToolBarFrame);
	m_infoAction->setCheckable(true);
	m_infoAction->setData(MM_INFO);
	connect(m_infoAction, SIGNAL(toggled(bool)), this, SLOT(OnMessageModeChanged()), Qt::QueuedConnection);
	actionGroup->addAction(m_infoAction);
	if (*m_defaultModeAttrPtr == 0){
		m_infoAction->setChecked(true);
	}

	static QIcon warningIcon(":/Icons/Warning.svg");
	m_warningAction = new QAction(warningIcon, tr("Warning"), ToolBarFrame);
	m_warningAction->setCheckable(true);
	m_warningAction->setData(MM_WARNING);
	connect(m_warningAction, SIGNAL(toggled(bool)), this, SLOT(OnMessageModeChanged()), Qt::QueuedConnection);
	actionGroup->addAction(m_warningAction);
	if (*m_defaultModeAttrPtr == 1){
		m_warningAction->setChecked(true);
	}

	static QIcon errorIcon(":/Icons/Error.svg");
	m_errorAction = new QAction(errorIcon, tr("Error"), ToolBarFrame);
	m_errorAction->setCheckable(true);
	m_errorAction->setData(MM_ERROR);
	connect(m_errorAction, SIGNAL(toggled(bool)), this, SLOT(OnMessageModeChanged()), Qt::QueuedConnection);
	actionGroup->addAction(m_errorAction);
	if (*m_defaultModeAttrPtr == 2){
		m_errorAction->setChecked(true);
	}

	static QIcon clearIcon(":/Icons/Clear");
	m_clearAction = new QAction(clearIcon, tr("Clear"), ToolBarFrame);
	connect(m_clearAction, SIGNAL(triggered()), this, SLOT(OnClearAction()), Qt::QueuedConnection);

	toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	toolBar->setIconSize(QSize(16, 16));
	toolBar->addAction(m_infoAction);
	toolBar->addAction(m_warningAction);
	toolBar->addAction(m_errorAction);
	toolBar->addAction(m_clearAction);
	toolBar->insertSeparator(m_clearAction);

	static QIcon exportIcon(":/Icons/DocumentExport.svg");
	if (m_fileLoaderCompPtr.IsValid()){
		m_exportAction = new QAction(exportIcon, tr("Export..."), ToolBarFrame);
		connect(m_exportAction, SIGNAL(triggered()), this, SLOT(OnExportAction()), Qt::QueuedConnection);		
		toolBar->addAction(m_exportAction);
		toolBar->insertSeparator(m_exportAction);
	}

	BaseClass::OnGuiCreated();
}


// reimplemented (imod::IObserver)

void CLogGuiComp::BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	if (IsGuiCreated()){
		if (updateFlags & ibase::IMessageContainer::CF_RESET){
			Q_EMIT EmitReset();
		}

		if (updateFlags & ibase::IMessageContainer::CF_MESSAGE_REMOVED){
			istd::IInformationProvider* messagePtr = dynamic_cast<istd::IInformationProvider*>(updateParamsPtr);
			if (messagePtr != NULL){
				Q_EMIT EmitRemoveMessage(QVariant::fromValue((void*)messagePtr));
			}
		}
	}

	BaseClass::BeforeUpdate(modelPtr, updateFlags, updateParamsPtr);
}


void CLogGuiComp::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	if (((updateFlags & ibase::IMessageContainer::CF_MESSAGE_ADDED) != 0) && IsGuiCreated()){
		istd::IInformationProvider* messagePtr = dynamic_cast<istd::IInformationProvider*>(updateParamsPtr);
		if (messagePtr != NULL){
			QTreeWidgetItem* itemPtr = CreateGuiItem(*messagePtr);
			if (itemPtr != NULL){
				Q_EMIT EmitAddMessage(itemPtr);
			}
		}
	}

	BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
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
	ibase::IMessageContainer* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		objectPtr->ClearMessages();
	}
}


void CLogGuiComp::OnExportAction()
{
	ibase::IMessageContainer* objectPtr = GetObjectPtr();
	if (objectPtr != NULL && m_fileLoaderCompPtr.IsValid()){
		m_fileLoaderCompPtr->SaveToFile(*objectPtr);
	}
}


} // namespace iqtgui


