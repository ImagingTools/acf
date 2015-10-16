#include "iloggui/CLogGuiComp.h"


// Qt includes
#include <QtCore/QDateTime>
#include <QtGui/QPainter>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QScrollBar>
#else
#include <QtGui/QHeaderView>
#include <QtGui/QToolBar>
#include <QtGui/QScrollBar>
#endif

// ACF includes
#include "iwidgets/CItemDelegate.h"
#include "iwidgets/CWidgetUpdateBlocker.h"


namespace iloggui
{


// public methods

CLogGuiComp::CLogGuiComp()
:	m_infoActionPtr(NULL),
	m_warningActionPtr(NULL),
	m_errorActionPtr(NULL),
	m_clearActionPtr(NULL),
	m_exportActionPtr(NULL),
	m_diagnosticModeActionPtr(NULL),
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

		treeItemPtr->setText(CT_TIME, messageTimeString);
		treeItemPtr->setText(CT_MESSAGE, message.GetInformationDescription());
		treeItemPtr->setText(CT_SOURCE, message.GetInformationSource());

		treeItemPtr->setToolTip(CT_TIME, messageTimeString);
		treeItemPtr->setToolTip(CT_MESSAGE, message.GetInformationDescription());
		treeItemPtr->setToolTip(CT_SOURCE, message.GetInformationSource());
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
	static QIcon infoIcon(":/Icons/Info");
	static QIcon warningIcon(":/Icons/Warning");
	static QIcon errorIcon(":/Icons/Error");

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


// reimplemented (ilog::IMessageConsumer)

bool CLogGuiComp::IsMessageSupported(
			int messageCategory,
			int /*messageId*/,
			const istd::IInformationProvider* /*messagePtr*/) const
{
	switch (messageCategory){
	case istd::IInformationProvider::IC_NONE:
		return *m_allowDiagnosticMessagesAttrPtr && (m_diagnosticModeActionPtr != NULL) && m_diagnosticModeActionPtr->isChecked();

	default:
		return true;
	}
}


void CLogGuiComp::AddMessage(const MessagePtr& messagePtr)
{
	if (messagePtr.IsValid() && IsMessageSupported(messagePtr->GetInformationCategory())){
		Q_EMIT EmitAddMessage(messagePtr);
	}
}


// reimplemented (CGuiComponentBase)

void CLogGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

#if QT_VERSION < 0x050000
	LogView->header()->setResizeMode(QHeaderView::ResizeToContents);
#else
	LogView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
#endif

	LogView->header()->setStretchLastSection(true);

	iwidgets::CItemDelegate* itemDelegate = new iwidgets::CItemDelegate(20, this);
	LogView->setItemDelegate(itemDelegate);
	LogView->header()->hide();

	if (*m_showLogDescriptionAttrPtr){
		LogView->header()->show();
	}

#if QT_VERSION < 0x050000
	LogView->header()->setResizeMode(CT_ICON, QHeaderView::Fixed);
#else
	LogView->header()->setSectionResizeMode(CT_ICON, QHeaderView::Fixed);
#endif
#if QT_VERSION >= 0x050200
	FilterText->setClearButtonEnabled(true);
#endif

	LogView->header()->resizeSection(CT_ICON, itemDelegate->GetItemHeight());

	QToolBar* toolBar = new QToolBar(ToolBarFrame);
	if (ToolBarFrame->layout()){
		ToolBarFrame->layout()->addWidget(toolBar);
	}

	QActionGroup* actionGroup = new QActionGroup(this);
	actionGroup->setExclusive(true);

	static QIcon infoIcon(":/Icons/Info");
	m_infoActionPtr = new QAction(infoIcon, tr("Info"), ToolBarFrame);
	m_infoActionPtr->setCheckable(true);
	m_infoActionPtr->setData(MM_INFO);
	connect(m_infoActionPtr, SIGNAL(toggled(bool)), this, SLOT(OnMessageModeChanged()), Qt::QueuedConnection);
	actionGroup->addAction(m_infoActionPtr);
	if (*m_defaultModeAttrPtr == 0){
		m_infoActionPtr->setChecked(true);
	}

	static QIcon warningIcon(":/Icons/Warning");
	m_warningActionPtr = new QAction(warningIcon, tr("Warning"), ToolBarFrame);
	m_warningActionPtr->setCheckable(true);
	m_warningActionPtr->setData(MM_WARNING);
	connect(m_warningActionPtr, SIGNAL(toggled(bool)), this, SLOT(OnMessageModeChanged()), Qt::QueuedConnection);
	actionGroup->addAction(m_warningActionPtr);
	if (*m_defaultModeAttrPtr == 1){
		m_warningActionPtr->setChecked(true);
	}

	static QIcon errorIcon(":/Icons/Error");
	m_errorActionPtr = new QAction(errorIcon, tr("Error"), ToolBarFrame);
	m_errorActionPtr->setCheckable(true);
	m_errorActionPtr->setData(MM_ERROR);
	connect(m_errorActionPtr, SIGNAL(toggled(bool)), this, SLOT(OnMessageModeChanged()), Qt::QueuedConnection);
	actionGroup->addAction(m_errorActionPtr);
	if (*m_defaultModeAttrPtr == 2){
		m_errorActionPtr->setChecked(true);
	}

	static QIcon clearIcon(":/Icons/Clear");
	m_clearActionPtr = new QAction(clearIcon, tr("Clear"), ToolBarFrame);
	connect(m_clearActionPtr, SIGNAL(triggered()), this, SLOT(OnClearAction()), Qt::QueuedConnection);

	toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	toolBar->setIconSize(QSize(16, 16));
	toolBar->addAction(m_infoActionPtr);
	toolBar->addAction(m_warningActionPtr);
	toolBar->addAction(m_errorActionPtr);
	toolBar->addAction(m_clearActionPtr);
	toolBar->insertSeparator(m_clearActionPtr);

	if (m_fileLoaderCompPtr.IsValid()){
		static QIcon exportIcon(":/Icons/DocumentExport");

		m_exportActionPtr = new QAction(exportIcon, tr("Export..."), ToolBarFrame);
		connect(m_exportActionPtr, SIGNAL(triggered()), this, SLOT(OnExportAction()), Qt::QueuedConnection);
		toolBar->addAction(m_exportActionPtr);
		toolBar->insertSeparator(m_exportActionPtr);
	}

	if (*m_allowDiagnosticMessagesAttrPtr){
		static QIcon diagnosticModeIcon(":/Icons/Diagnostics");

		m_diagnosticModeActionPtr = new QAction(diagnosticModeIcon, tr("Diagnostic Mode"), ToolBarFrame);
		m_diagnosticModeActionPtr->setCheckable(true);
		toolBar->addAction(m_diagnosticModeActionPtr);
		toolBar->insertSeparator(m_diagnosticModeActionPtr);
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
	iwidgets::CWidgetUpdateBlocker widgetUpdateBlocker(LogView);

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
	QAction* actionPtr = dynamic_cast<QAction*>(sender());
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
	iwidgets::CWidgetUpdateBlocker widgetUpdateBlocker(LogView);

	if (m_maxMessagesCountAttrPtr.IsValid() && (*m_maxMessagesCountAttrPtr > 0)){
		int count = LogView->topLevelItemCount();

		int itemsToRemove = count - *m_maxMessagesCountAttrPtr;
		if (itemsToRemove > 0){
			LogView->model()->removeRows(*m_maxMessagesCountAttrPtr - 1, itemsToRemove);
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


} // namespace iloggui


