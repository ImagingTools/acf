#include <iqtgui/CTabWidgetDelegate.h>


// Qt includes
#include <QtCore/QVariant>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>


namespace iqtgui
{


// public methods

QWidget* CTabWidgetDelegate::CreateContainerWidget(QWidget* parentWidgetPtr)
{	
	return new QTabWidget(parentWidgetPtr);
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

	// Add a spacer on the bottom the tab page:
	QSpacerItem* spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
	panelLayoutPtr->addItem(spacerPtr);

	return pageIndex;
}


void CTabWidgetDelegate::RemovePage(QWidget& containerWidget, int pageIndex)
{
	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		tabWidgetPtr->removeTab(pageIndex);
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


int CTabWidgetDelegate::GetCurrentPage(QWidget& containerWidget) const
{
	return containerWidget.property("currentIndex").toInt();
}


bool CTabWidgetDelegate::SetCurrentPage(QWidget& containerWidget, int pageIndex)
{
	return containerWidget.setProperty("currentIndex", pageIndex);
}


QString CTabWidgetDelegate::GetPageTitle(QWidget& containerWidget, int pageIndex) const
{
	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(&containerWidget);
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


QIcon CTabWidgetDelegate::GetPageIcon(QWidget& containerWidget, int pageIndex) const
{
	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(&containerWidget);
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


QString CTabWidgetDelegate::GetPageToolTip(QWidget& containerWidget, int pageIndex) const
{
	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(&containerWidget);
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


bool CTabWidgetDelegate::IsPageEnabled(QWidget& containerWidget, int pageIndex) const
{
	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(&containerWidget);
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


bool CTabWidgetDelegate::IsPageVisible(QWidget& /*containerWidget*/, int /*pageIndex*/) const
{
	return true;
}


bool CTabWidgetDelegate::SetPageVisible(QWidget& /*containerWidget*/, int /*pageIndex*/, bool /*isPageVisible*/)
{
	return false;
}


QSize CTabWidgetDelegate::GetPageIconSize(QWidget& containerWidget) const
{
	return containerWidget.property("iconSize").toSize();
}


bool CTabWidgetDelegate::SetPageIconSize(QWidget& containerWidget, const QSize& pageIconSize)
{
	return containerWidget.setProperty("iconSize", pageIconSize);
}


} // namespace iqtgui


