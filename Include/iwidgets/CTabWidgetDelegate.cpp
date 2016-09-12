#include <iwidgets/CTabWidgetDelegate.h>


// Qt includes
#include <QtCore/QtGlobal>
#include <QtCore/QVariant>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QVBoxLayout>
#else
#include <QtGui/QVBoxLayout>
#endif


namespace iwidgets
{


// public methods

// reimplemented (iwidgets::IMultiPageWidgetDelegate)

CTabWidgetDelegate::CTabWidgetDelegate(QTabWidget::TabPosition tabPosition, bool useDocumentMode)
	:m_tabPosition(tabPosition),
	m_useDocumentMode(useDocumentMode)
{
}


void CTabWidgetDelegate::SetTabPosition(QWidget& containerWidget, QTabWidget::TabPosition tabPosition)
{
	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		tabWidgetPtr->setTabPosition(tabPosition);
	}
}


void CTabWidgetDelegate::SetDocumentModeEnabled(QWidget& containerWidget, bool isDocumentModeEnabled)
{
	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		tabWidgetPtr->setDocumentMode(isDocumentModeEnabled);
	}
}


// reimplemented (iwidgets::IMultiPageWidgetDelegate)

QWidget* CTabWidgetDelegate::CreateContainerWidget(QWidget* parentWidgetPtr, int containerGuiFlags,int /*orientation*/)
{
	QTabWidget* tabWidgetPtr = new QTabWidget(parentWidgetPtr);

	// Setup tab widget:
	tabWidgetPtr->setTabPosition(m_tabPosition);
	tabWidgetPtr->setDocumentMode(m_useDocumentMode);

	m_containerGuiFlags = containerGuiFlags;

	return tabWidgetPtr;
}


bool CTabWidgetDelegate::ConnectPageIndexListener(
			QWidget& containerWidget,
			QObject* receiverPtr,
			const char* receiverSlotName)
{
	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		return QObject::connect(tabWidgetPtr, SIGNAL(currentChanged(int)), receiverPtr, receiverSlotName);
	}

	return false;
}


bool CTabWidgetDelegate::SetPageHeaderPosition(QWidget& containerWidget, PageHeaderPosition pageHeaderPosition)
{
	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		QTabWidget::TabPosition tabPosition = QTabWidget::North;
		switch (pageHeaderPosition){
			case PHP_LEFT:
				tabPosition = QTabWidget::West;
				break;
			case PHP_RIGHT:
				tabPosition = QTabWidget::East;
				break;
			case PHP_BOTTOM:
				tabPosition = QTabWidget::South;
				break;
			default:
				break;
		}

		tabWidgetPtr->setTabPosition(tabPosition);

		return true;
	}

	return false;
}


int CTabWidgetDelegate::InsertPage(
		QWidget& containerWidget,
		QWidget* pageWidgetPtr,
		const QString& pageTitle,
		int pageIndex)
{
	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(&containerWidget);
	if (tabWidgetPtr == NULL){
		return -1;
	}

	QWidget* panelPtr = new QWidget(tabWidgetPtr);
	QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);

	pageIndex = tabWidgetPtr->insertTab(pageIndex, panelPtr, pageTitle);

	panelLayoutPtr->addWidget(pageWidgetPtr);

	if (m_containerGuiFlags & CGF_COMPACT){
		// Add a spacer on the bottom the tab page:
		QSpacerItem* spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
		panelLayoutPtr->addItem(spacerPtr);
	}

	return pageIndex;
}


void CTabWidgetDelegate::RemovePage(QWidget& containerWidget, int pageIndex)
{
	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		tabWidgetPtr->removeTab(pageIndex);
	}
}


void CTabWidgetDelegate::ResetPages(QWidget& containerWidget)
{
	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		tabWidgetPtr->clear();
	}
}


int CTabWidgetDelegate::GetPagesCount(const QWidget& containerWidget) const
{
	const QTabWidget* tabWidgetPtr = dynamic_cast<const QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		return tabWidgetPtr->count();
	}

	return 0;
}


QWidget* CTabWidgetDelegate::GetPageWidgetPtr(const QWidget& containerWidget, int pageIndex) const
{
	const QTabWidget* tabWidgetPtr = dynamic_cast<const QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		return tabWidgetPtr->widget(pageIndex);
	}

	return NULL;
}


int CTabWidgetDelegate::GetCurrentPage(const QWidget& containerWidget) const
{
	return containerWidget.property("currentIndex").toInt();
}


bool CTabWidgetDelegate::SetCurrentPage(QWidget& containerWidget, int pageIndex)
{
	return containerWidget.setProperty("currentIndex", pageIndex);
}


QString CTabWidgetDelegate::GetPageTitle(const QWidget& containerWidget, int pageIndex) const
{
	const QTabWidget* tabWidgetPtr = dynamic_cast<const QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		return tabWidgetPtr->tabText(pageIndex);
	}

	return QString();
}


void CTabWidgetDelegate::SetPageTitle(QWidget& containerWidget, int pageIndex, const QString& pageTitle)
{
	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		tabWidgetPtr->setTabText(pageIndex, pageTitle);
	}
}


QIcon CTabWidgetDelegate::GetPageIcon(const QWidget& containerWidget, int pageIndex) const
{
	const QTabWidget* tabWidgetPtr = dynamic_cast<const QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		return tabWidgetPtr->tabIcon(pageIndex);
	}

	return QIcon();
}


void CTabWidgetDelegate::SetPageIcon(QWidget& containerWidget, int pageIndex, const QIcon& pageIcon)
{
	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		tabWidgetPtr->setTabIcon(pageIndex, pageIcon);
	}
}


QString CTabWidgetDelegate::GetPageToolTip(const QWidget& containerWidget, int pageIndex) const
{
	const QTabWidget* tabWidgetPtr = dynamic_cast<const QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		return tabWidgetPtr->tabToolTip(pageIndex);
	}

	return QString();
}


void CTabWidgetDelegate::SetPageToolTip(QWidget& containerWidget, int pageIndex, const QString& pageToolTip)
{
	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		tabWidgetPtr->setTabToolTip(pageIndex, pageToolTip);
	}
}


bool CTabWidgetDelegate::IsPageEnabled(const QWidget& containerWidget, int pageIndex) const
{
	const QTabWidget* tabWidgetPtr = dynamic_cast<const QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		return tabWidgetPtr->isTabEnabled(pageIndex);
	}

	return false;
}


bool CTabWidgetDelegate::SetPageEnabled(QWidget& containerWidget, int pageIndex, bool isPageEnabled)
{
	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		tabWidgetPtr->setTabEnabled(pageIndex, isPageEnabled);

		return true;
	}

	return false;
}


bool CTabWidgetDelegate::IsPageVisible(const QWidget& /*containerWidget*/, int /*pageIndex*/) const
{
	return true;
}


bool CTabWidgetDelegate::SetPageVisible(QWidget& /*containerWidget*/, int /*pageIndex*/, bool /*isPageVisible*/)
{
	return false;
}


QSize CTabWidgetDelegate::GetPageIconSize(const QWidget& containerWidget) const
{
	return containerWidget.property("iconSize").toSize();
}


bool CTabWidgetDelegate::SetPageIconSize(QWidget& containerWidget, const QSize& pageIconSize)
{
	return containerWidget.setProperty("iconSize", pageIconSize);
}


} // namespace iwidgets


