#include <iqtgui/CMultiPageWidget.h>


// Qt includes
#include <QtCore/QVariant>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QToolBox>
#include <QtGui/QTabWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QSplitter>


namespace iqtgui
{


// public methods

CMultiPageWidget::CMultiPageWidget(int designMode, bool useHorizontalLayout)
	:m_guiContainerPtr(NULL),
	m_designMode(designMode),
	m_useHorizontalLayout(useHorizontalLayout)
{
}


int CMultiPageWidget::InsertPage(
			QWidget* pageWidgetPtr,
			const QString& pageTitle,
			int pageIndex)
{
	if (m_guiContainerPtr != NULL){
		qWarning("Container GUI was not created, page could not be inserted.");

		return -1;
	}

	if (!m_containerWidgetDelegateMap.contains(m_designMode)){
		qWarning("No container delegate registered for the current UI mode. The page could not be inserted");

		return -1;
	}

	MultiPageWidgetDelegatePtr delegatePtr = m_containerWidgetDelegateMap[m_designMode];
	Q_ASSERT(delegatePtr.IsValid());

	return delegatePtr->InsertPage(*m_guiContainerPtr, pageWidgetPtr, pageTitle, pageIndex);

	bool addSpacer = false;

	QWidget* panelPtr = NULL;

	if (m_designMode == DT_TAB_WIDGET){
		panelPtr = new QWidget(m_guiContainerPtr);
		new QVBoxLayout(panelPtr);
		QTabWidget* tabWidgetPtr = static_cast<QTabWidget*>(m_guiContainerPtr);
		pageIndex = tabWidgetPtr->insertTab(pageIndex, panelPtr, pageTitle);

		addSpacer = true;
	}
	else if (m_designMode == DT_TOOL_BOX){
		panelPtr = new QWidget(m_guiContainerPtr);
		QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
		panelLayoutPtr->setContentsMargins(6, 0, 6, 0);
		QToolBox* toolBoxPtr = static_cast<QToolBox*>(m_guiContainerPtr);
		pageIndex = toolBoxPtr->insertItem(pageIndex, panelPtr, pageTitle);

		addSpacer = true;
	}
	else if (m_designMode == DT_SPLITTER){
		QSplitter* splitterPtr = static_cast<QSplitter*>(m_guiContainerPtr);
		panelPtr = new QWidget(m_guiContainerPtr);
		QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
		panelLayoutPtr->setMargin(0);
		splitterPtr->addWidget(panelPtr);
	}
	else{
		if (!pageTitle.isEmpty()){
			panelPtr = new QGroupBox(pageTitle, m_guiContainerPtr);
			new QVBoxLayout(panelPtr);
		}
		else{
			panelPtr = new QWidget(this);
			QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
			panelLayoutPtr->setContentsMargins(0, 0, 0, 0);
		}

		QLayout* parentLayoutPtr = m_guiContainerPtr->layout();
		if (parentLayoutPtr != NULL){
			parentLayoutPtr->addWidget(panelPtr);
		}
	}

	QLayout* panelLayoutPtr = panelPtr->layout();
	if (panelLayoutPtr != NULL){
		panelLayoutPtr->addWidget(pageWidgetPtr);
	}
	else{
		pageWidgetPtr->setParent(panelPtr);
	}

	if (addSpacer){
		QLayout* panelLayoutPtr = panelPtr->layout();
		if (panelLayoutPtr != NULL){
			QSpacerItem* spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
			panelLayoutPtr->addItem(spacerPtr);
		}
	}

	return pageIndex;
}


void CMultiPageWidget::RemovePage(int pageIndex)
{
	if (m_guiContainerPtr != NULL){
		qWarning("Container GUI was not created, page could not be removed.");

		return;
	}

	if (!m_containerWidgetDelegateMap.contains(m_designMode)){
		qWarning("No container delegate registered for the current UI mode. The page could not be removed");

		return;
	}

	MultiPageWidgetDelegatePtr delegatePtr = m_containerWidgetDelegateMap[m_designMode];
	Q_ASSERT(delegatePtr.IsValid());

	delegatePtr->RemovePage(*m_guiContainerPtr, pageIndex);
}


void CMultiPageWidget::SetPageEnabled(int pageIndex, bool isEnabled)
{
	if (m_guiContainerPtr != NULL){
		qWarning("Container GUI was not created, page could not be enabled.");

		return;
	}

	if (!m_containerWidgetDelegateMap.contains(m_designMode)){
		qWarning("No container delegate registered for the current UI mode. The page could not be enabled");

		return;
	}

	MultiPageWidgetDelegatePtr delegatePtr = m_containerWidgetDelegateMap[m_designMode];
	Q_ASSERT(delegatePtr.IsValid());

	delegatePtr->SetPageEnabled(*m_guiContainerPtr, pageIndex, isEnabled);
}


void CMultiPageWidget::SetPageVisible(int pageIndex, bool isVisible)
{
	if (m_guiContainerPtr != NULL){
		qWarning("Container GUI was not created, page could not be set visible\\hidden.");

		return;
	}

	if (!m_containerWidgetDelegateMap.contains(m_designMode)){
		qWarning("No container delegate registered for the current UI mode. The page could not be set visible\\hidden");

		return;
	}

	MultiPageWidgetDelegatePtr delegatePtr = m_containerWidgetDelegateMap[m_designMode];
	Q_ASSERT(delegatePtr.IsValid());

	delegatePtr->SetPageVisible(*m_guiContainerPtr, pageIndex, isVisible);
}


int CMultiPageWidget::GetPagesCount() const
{
	if (m_guiContainerPtr != NULL){
		qWarning("Container GUI was not created");

		return 0;
	}

	if (!m_containerWidgetDelegateMap.contains(m_designMode)){
		qWarning("No container delegate registered for the current UI mode");

		return 0;
	}

	MultiPageWidgetDelegatePtr delegatePtr = m_containerWidgetDelegateMap[m_designMode];
	Q_ASSERT(delegatePtr.IsValid());

	return delegatePtr->GetPagesCount(*m_guiContainerPtr);
}


QWidget* CMultiPageWidget::GetPageWidgetPtr(int pageIndex) const
{
	if (m_guiContainerPtr != NULL){
		qWarning("Container GUI was not created");

		return 0;
	}

	if (!m_containerWidgetDelegateMap.contains(m_designMode)){
		qWarning("No container delegate registered for the current UI mode");

		return 0;
	}

	MultiPageWidgetDelegatePtr delegatePtr = m_containerWidgetDelegateMap[m_designMode];
	Q_ASSERT(delegatePtr.IsValid());

	return delegatePtr->GetPageWidgetPtr(*m_guiContainerPtr, pageIndex);
}


// protected methods

int CMultiPageWidget::GetCurrentPageIndex() const
{
	if (m_guiContainerPtr != NULL){
		qWarning("Container GUI was not created");

		return 0;
	}

	if (!m_containerWidgetDelegateMap.contains(m_designMode)){
		qWarning("No container delegate registered for the current UI mode");

		return 0;
	}

	MultiPageWidgetDelegatePtr delegatePtr = m_containerWidgetDelegateMap[m_designMode];
	Q_ASSERT(delegatePtr.IsValid());

	return delegatePtr->GetCurrentPage(*m_guiContainerPtr);
}


QWidget* CMultiPageWidget::GetGuiContainerPtr() const
{
	return m_guiContainerPtr;
}


bool CMultiPageWidget::CreateContainerGui()
{
	if (!m_containerWidgetDelegateMap.contains(m_designMode)){
		qWarning("No container delegate registered for the current UI mode. The container UI could not be created");

		return false;
	}

	MultiPageWidgetDelegatePtr delegatePtr = m_containerWidgetDelegateMap[m_designMode];
	Q_ASSERT(delegatePtr.IsValid());


	// initialize the GUI container
	QLayout* layoutPtr = layout();
	if (layoutPtr == NULL){
		if (m_useHorizontalLayout){
			layoutPtr = new QHBoxLayout(this);
		}
		else{
			layoutPtr = new QVBoxLayout(this);
		}
	}

	layoutPtr->setMargin(0);

	QToolBox* toolBoxPtr;
	QSplitter* splitterPtr;

	switch (m_designMode){
		case DT_TAB_WIDGET:
			setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
			m_guiContainerPtr = delegatePtr->CreateContainerWidget(this);
			layoutPtr->addWidget(m_guiContainerPtr);
			break;

		case DT_TOOL_BOX:
			setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
			m_guiContainerPtr = toolBoxPtr = new QToolBox(this);
			toolBoxPtr->setBackgroundRole(QPalette::Window);
			layoutPtr->addWidget(toolBoxPtr);
			break;

		case DT_SPLITTER:
			m_guiContainerPtr = splitterPtr = new QSplitter(this);
			if (m_useHorizontalLayout){
				splitterPtr->setOrientation(Qt::Horizontal);
			}
			else{
				splitterPtr->setOrientation(Qt::Vertical);
			}
			layoutPtr->addWidget(splitterPtr);
			break;

		default:
			m_guiContainerPtr = this;
	}

	if (!m_pageIconSize.isNull() && m_pageIconSize.isValid() && !m_pageIconSize.isEmpty()){
		delegatePtr->SetPageIconSize(*m_guiContainerPtr, m_pageIconSize);
	}

	return (m_guiContainerPtr != NULL);
}


/*

bool CMultiPageWidget::SetPageEnabled(QWidget& containerWidget, int pageIndex, bool isPageEnabled) const
{
	QToolBox* toolBoxPtr = dynamic_cast<QToolBox*>(&containerWidget);
	if (toolBoxPtr != NULL){
		toolBoxPtr->setItemEnabled(pageIndex, isPageEnabled);

		return true;
	}

	QSplitter* splitterPtr = dynamic_cast<QSplitter*>(&containerWidget);
	if (splitterPtr != NULL){
		QWidget* pageWidgetPtr = splitterPtr->widget(pageIndex);
		Q_ASSERT(pageWidgetPtr != NULL);

		pageWidgetPtr->setEnabled(isPageEnabled);

		return true;
	}

	return false;
}

*/

} // namespace iqtgui


