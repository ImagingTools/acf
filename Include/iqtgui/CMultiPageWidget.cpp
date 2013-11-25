#include <iqtgui/CMultiPageWidget.h>


// Qt includes
#include <QtCore/QtGlobal>
#include <QtCore/QVariant>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QSplitter>
#else
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QToolBox>
#include <QtGui/QTabWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QSplitter>
#endif

// ACF includes
#include <iqtgui/CTabWidgetDelegate.h>
#include <iqtgui/CToolBoxDelegate.h>
#include <iqtgui/CSplitterDelegate.h>
#include <iqtgui/CSimpleGuiContainerDelegate.h>
#include <iqtgui/CStackWidgetDelegate.h>


namespace iqtgui
{


// public methods

CMultiPageWidget::CMultiPageWidget(QWidget* parentWidgetPtr, int designMode, int containerGuiFlags, Qt::Orientation orientation)
	:BaseClass(parentWidgetPtr),
	m_designMode(designMode),
	m_containerGuiFlags(containerGuiFlags),
	m_orientation(orientation)
{
	// Register default delegates:
	RegisterMultiPageWidgetDelegate<iqtgui::CSimpleGuiContainerDelegate>(DT_SIMPLE);
	RegisterMultiPageWidgetDelegate<iqtgui::CTabWidgetDelegate>(DT_TAB_WIDGET);
	RegisterMultiPageWidgetDelegate<iqtgui::CToolBoxDelegate>(DT_TOOL_BOX);
	RegisterMultiPageWidgetDelegate<iqtgui::CSplitterDelegate>(DT_SPLITTER);
	RegisterMultiPageWidgetDelegate<iqtgui::CStackWidgetDelegate>(DT_STACK);

	CreateContainerGui();
}


QWidget* CMultiPageWidget::GetContainerWidgetPtr() const
{
	return m_guiContainerPtr.GetPtr();
}


void CMultiPageWidget::SetDesignMode(int designMode)
{
	if (GetPagesCount() > 0){
		// TODO: reparent all pages and add to the new container.
	}
	else{
		m_guiContainerPtr.Reset();
		m_designMode = designMode;

		CreateContainerGui();
	}
}


void CMultiPageWidget::ResetPages()
{
	while (GetPagesCount() > 0){
		RemovePage(0);
	}
}


void CMultiPageWidget::SetPageHeaderPosition(PageHeaderPosition pageHeaderPosition)
{
	MultiPageWidgetDelegatePtr delegatePtr = GetCurrentDelegate();
	if (delegatePtr.IsValid()){
		delegatePtr->SetPageHeaderPosition(*m_guiContainerPtr, pageHeaderPosition);
	}
}


int CMultiPageWidget::InsertPage(
			QWidget* pageWidgetPtr,
			const QString& pageTitle,
			int pageIndex)
{
	MultiPageWidgetDelegatePtr delegatePtr = GetCurrentDelegate();
	if (delegatePtr.IsValid()){
		return delegatePtr->InsertPage(*m_guiContainerPtr, pageWidgetPtr, pageTitle, pageIndex);
	}

	return -1;
}


void CMultiPageWidget::RemovePage(int pageIndex)
{
	MultiPageWidgetDelegatePtr delegatePtr = GetCurrentDelegate();
	if (delegatePtr.IsValid()){
		delegatePtr->RemovePage(*m_guiContainerPtr, pageIndex);
	}
}


int CMultiPageWidget::GetPagesCount() const
{
	MultiPageWidgetDelegatePtr delegatePtr = GetCurrentDelegate();
	if (delegatePtr.IsValid()){
		return delegatePtr->GetPagesCount(*m_guiContainerPtr);
	}

	return 0;
}


QWidget* CMultiPageWidget::GetPageWidgetPtr(int pageIndex) const
{
	MultiPageWidgetDelegatePtr delegatePtr = GetCurrentDelegate();
	if (delegatePtr.IsValid()){
		return delegatePtr->GetPageWidgetPtr(*m_guiContainerPtr, pageIndex);
	}

	return NULL;
}


int CMultiPageWidget::GetCurrentPage() const
{
	MultiPageWidgetDelegatePtr delegatePtr = GetCurrentDelegate();
	if (delegatePtr.IsValid()){
		return delegatePtr->GetCurrentPage(*m_guiContainerPtr);
	}

	return -1;
}


bool CMultiPageWidget::SetCurrentPage(int pageIndex)
{
	MultiPageWidgetDelegatePtr delegatePtr = GetCurrentDelegate();
	if (delegatePtr.IsValid()){
		return delegatePtr->SetCurrentPage(*m_guiContainerPtr, pageIndex);
	}

	return false;
}


QString CMultiPageWidget::GetPageTitle(int pageIndex) const
{
	MultiPageWidgetDelegatePtr delegatePtr = GetCurrentDelegate();
	if (delegatePtr.IsValid()){
		return delegatePtr->GetPageTitle(*m_guiContainerPtr, pageIndex);
	}

	return QString();
}


void CMultiPageWidget::SetPageTitle(int pageIndex, const QString& pageTitle)
{
	MultiPageWidgetDelegatePtr delegatePtr = GetCurrentDelegate();
	if (delegatePtr.IsValid()){
		delegatePtr->SetPageTitle(*m_guiContainerPtr, pageIndex, pageTitle);
	}
}


QIcon CMultiPageWidget::GetPageIcon(int pageIndex) const
{
	MultiPageWidgetDelegatePtr delegatePtr = GetCurrentDelegate();
	if (delegatePtr.IsValid()){
		return delegatePtr->GetPageIcon(*m_guiContainerPtr, pageIndex);
	}

	return QIcon();
}


void CMultiPageWidget::SetPageIcon(int pageIndex, const QIcon& pageIcon)
{
	MultiPageWidgetDelegatePtr delegatePtr = GetCurrentDelegate();
	if (delegatePtr.IsValid()){
		delegatePtr->SetPageIcon(*m_guiContainerPtr, pageIndex, pageIcon);
	}
}


QString CMultiPageWidget::GetPageToolTip(int pageIndex) const
{
	MultiPageWidgetDelegatePtr delegatePtr = GetCurrentDelegate();
	if (delegatePtr.IsValid()){
		return delegatePtr->GetPageToolTip(*m_guiContainerPtr, pageIndex);
	}

	return QString();
}


void CMultiPageWidget::SetPageToolTip(int pageIndex, const QString& pageToolTip)
{
	MultiPageWidgetDelegatePtr delegatePtr = GetCurrentDelegate();
	if (delegatePtr.IsValid()){
		delegatePtr->SetPageToolTip(*m_guiContainerPtr, pageIndex, pageToolTip);
	}
}


bool CMultiPageWidget::IsPageEnabled(int pageIndex) const
{
	MultiPageWidgetDelegatePtr delegatePtr = GetCurrentDelegate();
	if (delegatePtr.IsValid()){
		return delegatePtr->IsPageEnabled(*m_guiContainerPtr, pageIndex);
	}

	return false;
}


bool CMultiPageWidget::SetPageEnabled(int pageIndex, bool isEnabled)
{
	MultiPageWidgetDelegatePtr delegatePtr = GetCurrentDelegate();
	if (delegatePtr.IsValid()){
		return delegatePtr->SetPageEnabled(*m_guiContainerPtr, pageIndex, isEnabled);
	}

	return false;
}


bool CMultiPageWidget::IsPageVisible(int pageIndex) const
{
	MultiPageWidgetDelegatePtr delegatePtr = GetCurrentDelegate();
	if (delegatePtr.IsValid()){
		return delegatePtr->IsPageVisible(*m_guiContainerPtr, pageIndex);
	}

	return false;
}


bool CMultiPageWidget::SetPageVisible(int pageIndex, bool isVisible)
{
	MultiPageWidgetDelegatePtr delegatePtr = GetCurrentDelegate();
	if (delegatePtr.IsValid()){
		return delegatePtr->SetPageVisible(*m_guiContainerPtr, pageIndex, isVisible);
	}

	return false;
}


QSize CMultiPageWidget::GetPageIconSize() const
{
	MultiPageWidgetDelegatePtr delegatePtr = GetCurrentDelegate();
	if (delegatePtr.IsValid()){
		return delegatePtr->GetPageIconSize(*m_guiContainerPtr);
	}

	return QSize();
}


bool CMultiPageWidget::SetPageIconSize(const QSize& pageIconSize)
{
	MultiPageWidgetDelegatePtr delegatePtr = GetCurrentDelegate();
	if (delegatePtr.IsValid()){
		return delegatePtr->SetPageIconSize(*m_guiContainerPtr, pageIconSize);
	}

	return false;
}


// protected slots

void CMultiPageWidget::OnPageIndexChanged(int pageIndex)
{
	Q_EMIT EmitPageIndexChanged(pageIndex);
}


// private methods

bool CMultiPageWidget::CreateContainerGui()
{
	if (!m_containerWidgetDelegateMap.contains(m_designMode)){
		qWarning("No container delegate registered for the current UI mode");

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

	m_guiContainerPtr.SetPtr(delegatePtr->CreateContainerWidget(this, m_containerGuiFlags, m_orientation));
	m_guiContainerPtr->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	layoutPtr->addWidget(m_guiContainerPtr.GetPtr());

	delegatePtr->ConnectPageIndexListener(*m_guiContainerPtr.GetPtr(), this, SLOT(OnPageIndexChanged(int)));

	if (!m_pageIconSize.isNull() && m_pageIconSize.isValid() && !m_pageIconSize.isEmpty()){
		delegatePtr->SetPageIconSize(*m_guiContainerPtr.GetPtr(), m_pageIconSize);
	}

	if (m_containerGuiFlags & IMultiPageWidgetDelegate::CGF_COMPACT){
		switch (m_orientation){
			case Qt::Vertical:
				m_guiContainerPtr->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
				break;

			case Qt::Horizontal:
				m_guiContainerPtr->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
				break;

			default:
				break;
		}
		
		QBoxLayout* boxLayoutPtr = dynamic_cast<QBoxLayout*>(layoutPtr);
		if (boxLayoutPtr != NULL){
			boxLayoutPtr->insertStretch(-1);
		}
	}

	return (m_guiContainerPtr != NULL);
}


CMultiPageWidget::MultiPageWidgetDelegatePtr CMultiPageWidget::GetCurrentDelegate() const
{
	if (!m_guiContainerPtr.IsValid()){
		qWarning("Container GUI was not created");

		return MultiPageWidgetDelegatePtr();
	}

	if (!m_containerWidgetDelegateMap.contains(m_designMode)){
		qWarning("No container delegate registered for the current UI mode");

		return MultiPageWidgetDelegatePtr();
	}

	MultiPageWidgetDelegatePtr delegatePtr = m_containerWidgetDelegateMap[m_designMode];
	Q_ASSERT(delegatePtr.IsValid());

	return delegatePtr;
}


} // namespace iqtgui


