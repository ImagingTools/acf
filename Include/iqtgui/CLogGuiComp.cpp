#include "iqtgui/CLogGuiComp.h"


// Qt includes
#include <QtCore/QDateTime>
#include <QtGui/QHeaderView>
#include <QtGui/QToolBar>
#include <QtGui/QPainter>
#include <QtGui/QScrollBar>

// ACF includes
#include "iqtgui/CItemDelegate.h"
#include "iqtgui/CWidgetUpdateBlocker.h"


namespace iqtgui
{


// public methods

CLogGuiComp::CLogGuiComp()
:	m_infoAction(NULL),
	m_warningAction(NULL),
	m_errorAction(NULL),
	m_clearAction(NULL),
	m_exportAction(NULL),
	m_currentMessageMode(MM_ALL),
	m_statusCategory(istd::IInformationProvider::IC_NONE)
{
	qRegisterMetaType<MessagePtr>("MessagePtr");

	connect(
				this,
				SIGNAL(EmitAddMessage(const MessagePtr&)),
				this,
				SLOT(OnAddMessage(const MessagePtr&)),
				Qt::QueuedConnection);
}


// protected methods
	
QTreeWidgetItem* CLogGuiComp::CreateGuiItem(const istd::IInformationProvider& message)
{
	QTreeWidgetItem* treeItemPtr = new QTreeWidgetItem;
	if (treeItemPtr != NULL){
		QDateTime timeStamp = message.GetInformationTimeStamp();

		QString messageTimeString;
		
		if (!(*m_logTimeFormatAttrPtr).isEmpty()){
			messageTimeString = timeStamp.toString(*m_logTimeFormatAttrPtr);
		}
		else{
			messageTimeString = timeStamp.toString();
		}

		qint64 messageTimeStamp = timeStamp.toMSecsSinceEpoch();

		treeItemPtr->setText(CT_TIME, messageTimeString);
		treeItemPtr->setText(CT_MESSAGE, message.GetInformationDescription());
		treeItemPtr->setText(CT_SOURCE, message.GetInformationSource());

		treeItemPtr->setToolTip(CT_TIME, messageTimeString);
		treeItemPtr->setToolTip(CT_MESSAGE, message.GetInformationDescription());
		treeItemPtr->setToolTip(CT_SOURCE, message.GetInformationSource());
		treeItemPtr->setData(0, DR_MESSAGE_ID, messageTimeStamp);
		treeItemPtr->setData(0, DR_CATEGORY, message.GetInformationCategory());

		istd::IInformationProvider::InformationCategory category = message.GetInformationCategory();
		QIcon messageIcon = GetCategoryIcon(category).pixmap(QSize(12, 12), QIcon::Normal, QIcon::On);

		treeItemPtr->setIcon(CT_ICON, messageIcon);
	}

	return treeItemPtr;
}


QIcon CLogGuiComp::GetCategoryIcon(int category) const
{
	static QIcon logIcon(":/Icons/Log");
	static QIcon infoIcon(":/Icons/Info.svg");
	static QIcon warningIcon(":/Icons/Warning.svg");
	static QIcon errorIcon(":/Icons/Error.svg");

	switch (category){
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


QString CLogGuiComp::GetCategoryText(int category) const
{
	switch (category){
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

// reimplemented (ibase::IMessageConsumer)

bool CLogGuiComp::IsMessageSupported(
			int /*messageCategory*/,
			int /*messageId*/,
			const istd::IInformationProvider* /*messagePtr*/) const
{
	return true;
}


void CLogGuiComp::AddMessage(const MessagePtr& messagePtr)
{
	Q_EMIT EmitAddMessage(messagePtr);
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

	if (m_showLogDescriptionAttrPtr.IsValid() && *m_showLogDescriptionAttrPtr){
		LogView->header()->show();
	}

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

	connect(&m_removeMessagesTimer, SIGNAL(timeout()), this, SLOT(OnRemoveMessagesTimer()));

	m_removeMessagesTimer.start(5000);

	if (!*m_showMessageTextFilterAttrPtr){
		FilterFrame->setVisible(false);
	}

	GenerateMessageList();
}


void CLogGuiComp::OnGuiDestroyed()
{
	m_removeMessagesTimer.stop();

	BaseClass::OnGuiDestroyed();
}


// reimplemented (icomp::CComponentBase)

void CLogGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	BaseClass2::SetMaxMessageCount(*m_maxMessagesCountAttrPtr);

	if (m_slaveMessageConsumerCompPtr.IsValid()){
		BaseClass2::SetSlaveConsumer(m_slaveMessageConsumerCompPtr.GetPtr());
	}

	if (m_slaveMessageContainerCompPtr.IsValid()){
		BaseClass2::AddChildContainer(m_slaveMessageContainerCompPtr.GetPtr());
	}
}


// private methods

void CLogGuiComp::UpdateVisualStatus()
{
	istd::CChangeNotifier visualStatusNotifier(&m_visualStatus);

	SetStatusIcon(GetCategoryIcon(m_statusCategory));
	SetStatusText(GetCategoryText(m_statusCategory));
}


void CLogGuiComp::UpdateItemVisibility(QTreeWidgetItem* itemPtr, const QString& filterText) const
{
	int itemCategory = itemPtr->data(0, DR_CATEGORY).toInt();

	bool hideItem = (itemCategory < m_currentMessageMode);

	QString messageText = itemPtr->text(CT_MESSAGE);
	if (!filterText.isEmpty() && !messageText.contains(filterText, Qt::CaseInsensitive)){
		hideItem = true;
	}

	itemPtr->setHidden(hideItem);
}


void CLogGuiComp::GenerateMessageList()
{
	Messages messageList = BaseClass2::GetMessages();

	for (Messages::ConstIterator index = messageList.constBegin(); index != messageList.constEnd(); index++){
		AddMessageToList(*index);
	}
}


void CLogGuiComp::AddMessageToList(const MessagePtr& messagePtr)
{
	iqtgui::CWidgetUpdateBlocker widgetUpdateBlocker(LogView);

	Q_ASSERT(messagePtr.IsValid());

	QTreeWidgetItem* itemPtr = CreateGuiItem(*messagePtr.GetPtr());

	LogView->insertTopLevelItem(0, itemPtr);

	UpdateItemVisibility(itemPtr, FilterText->text());

	int itemCategory = itemPtr->data(0, DR_CATEGORY).toInt();
	if (itemCategory > m_statusCategory){
		m_statusCategory = itemCategory;

		UpdateVisualStatus();
	}
}


// protected slots

void CLogGuiComp::OnAddMessage(const MessagePtr& messagePtr)
{	
	if (IsGuiCreated()){
		AddMessageToList(messagePtr);
	}

	BaseClass2::AddMessage(messagePtr);
}


void CLogGuiComp::OnMessageModeChanged()
{
	QAction* actionPtr = dynamic_cast<QAction*> (sender());
	if (actionPtr != NULL){
		m_currentMessageMode = actionPtr->data().toInt();
	}

	int worstCategory = istd::IInformationProvider::IC_NONE;

	QString filterText = FilterText->text();
	
	for (int itemIndex = 0; itemIndex < LogView->topLevelItemCount(); itemIndex++){
		QTreeWidgetItem* itemPtr = LogView->topLevelItem(itemIndex);
		Q_ASSERT(itemPtr != NULL);

		int itemCategory = itemPtr->data(0, DR_CATEGORY).toInt();

		UpdateItemVisibility(itemPtr, filterText);

		if (itemCategory > worstCategory){
			worstCategory = itemCategory;
		}
	}

	m_statusCategory = worstCategory;

	UpdateVisualStatus();
}


void CLogGuiComp::OnClearAction()
{
	BaseClass2::ClearMessages();

	LogView->clear();

	if (m_statusCategory != istd::IInformationProvider::IC_NONE){
		m_statusCategory = istd::IInformationProvider::IC_NONE;

		UpdateVisualStatus();
	}
}


void CLogGuiComp::OnExportAction()
{
	if (m_fileLoaderCompPtr.IsValid()){
		m_fileLoaderCompPtr->SaveToFile(*this);
	}
}


void CLogGuiComp::OnRemoveMessagesTimer()
{
	iqtgui::CWidgetUpdateBlocker widgetUpdateBlocker(LogView);

	if (m_maxMessagesCountAttrPtr.IsValid() && (*m_maxMessagesCountAttrPtr > 0)){
		int count = LogView->topLevelItemCount();
		while (count > *m_maxMessagesCountAttrPtr){
			QTreeWidgetItem* lastItem = LogView->takeTopLevelItem (count - 1);
			delete lastItem;
			
			count = LogView->topLevelItemCount();
		}
	}
}


void CLogGuiComp::on_FilterText_textChanged(const QString& filterText)
{
	for (int itemIndex = 0; itemIndex < LogView->topLevelItemCount(); itemIndex++){
		QTreeWidgetItem* itemPtr = LogView->topLevelItem(itemIndex);
		Q_ASSERT(itemPtr != NULL);

		UpdateItemVisibility(itemPtr, filterText);
	}
}


void CLogGuiComp::on_FilterClearButton_clicked()
{
	FilterText->clear();
}


} // namespace iqtgui


