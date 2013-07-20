#include <iqtgui/CMultiPageWidget.h>


// Qt includes
#include <QtCore/QVariant>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QToolBox>
#include <QtGui/QTabWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QSplitter>

// ACF includes
#include <iqtgui/CTabWidgetDelegate.h>
#include <iqtgui/CToolBoxDelegate.h>
#include <iqtgui/CSplitterDelegate.h>
#include <iqtgui/CSimpleGuiContainerDelegate.h>


namespace iqtgui
{


// public methods

CMultiPageWidget::CMultiPageWidget(int designMode, bool useHorizontalLayout)
	:m_guiContainerPtr(NULL),
	m_designMode(designMode),
	m_useHorizontalLayout(useHorizontalLayout)
{
	// Register default delegates:
	RegisterMultiPageWidgetDelegate<iqtgui::CSimpleGuiContainerDelegate>(DT_SIMPLE);
	RegisterMultiPageWidgetDelegate<iqtgui::CTabWidgetDelegate>(DT_TAB_WIDGET);
	RegisterMultiPageWidgetDelegate<iqtgui::CToolBoxDelegate>(DT_TOOL_BOX);
	RegisterMultiPageWidgetDelegate<iqtgui::CSplitterDelegate>(DT_SPLITTER);

	CreateContainerGui();
}


int CMultiPageWidget::InsertPage(
			QWidget* pageWidgetPtr,
			const QString& pageTitle,
			int pageIndex)
{
	if (m_guiContainerPtr == NULL){
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
}


void CMultiPageWidget::RemovePage(int pageIndex)
{
	if (m_guiContainerPtr == NULL){
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
	if (m_guiContainerPtr == NULL){
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
	if (m_guiContainerPtr == NULL){
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
	if (m_guiContainerPtr == NULL){
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
	if (m_guiContainerPtr == NULL){
		qWarning("Container GUI was not created");

		return NULL;
	}

	if (!m_containerWidgetDelegateMap.contains(m_designMode)){
		qWarning("No container delegate registered for the current UI mode");

		return NULL;
	}

	MultiPageWidgetDelegatePtr delegatePtr = m_containerWidgetDelegateMap[m_designMode];
	Q_ASSERT(delegatePtr.IsValid());

	return delegatePtr->GetPageWidgetPtr(*m_guiContainerPtr, pageIndex);
}


// protected methods

int CMultiPageWidget::GetCurrentPageIndex() const
{
	if (m_guiContainerPtr == NULL){
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
		layoutPtr = new QVBoxLayout(this);
	}

	layoutPtr->setMargin(0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

	m_guiContainerPtr = delegatePtr->CreateContainerWidget(this, m_useHorizontalLayout ? Qt::Horizontal : Qt::Vertical);
	layoutPtr->addWidget(m_guiContainerPtr);

	if (!m_pageIconSize.isNull() && m_pageIconSize.isValid() && !m_pageIconSize.isEmpty()){
		delegatePtr->SetPageIconSize(*m_guiContainerPtr, m_pageIconSize);
	}

	return (m_guiContainerPtr != NULL);
}


} // namespace iqtgui


