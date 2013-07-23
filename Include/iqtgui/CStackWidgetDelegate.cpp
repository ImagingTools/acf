#include <iqtgui/CStackWidgetDelegate.h>


// Qt includes
#include <QtCore/QVariant>
#include <QtGlobal>
#include <QtGui/QIcon>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QGroupBox>
#else
#include <QtGui/QVBoxLayout>
#include <QtGui/QStackedWidget>
#include <QtGui/QGroupBox>
#endif


namespace iqtgui
{


// public methods

QWidget* CStackWidgetDelegate::CreateContainerWidget(QWidget* parentWidgetPtr, int /*orientation*/)
{	
	return new QStackedWidget(parentWidgetPtr);
}


bool CStackWidgetDelegate::SetPageHeaderPosition(QWidget& /*containerWidget*/, PageHeaderPosition /*pageHeaderPosition*/)
{
	return false;
}


int CStackWidgetDelegate::InsertPage(
		QWidget& containerWidget,
		QWidget* pageWidgetPtr,
		const QString& pageTitle,
		int pageIndex)
{
	QStackedWidget* stackedWidgetPtr = dynamic_cast<QStackedWidget*>(&containerWidget);
	if (stackedWidgetPtr == NULL){
		return -1;
	}

	QWidget* panelPtr = pageTitle.isEmpty() ? new QWidget(stackedWidgetPtr) : new QGroupBox(pageTitle, stackedWidgetPtr);
	QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
	panelLayoutPtr->addWidget(pageWidgetPtr);

	return stackedWidgetPtr->insertWidget(pageIndex, panelPtr);
}


void CStackWidgetDelegate::RemovePage(QWidget& containerWidget, int pageIndex)
{
	QStackedWidget* stackedWidgetPtr = dynamic_cast<QStackedWidget*>(&containerWidget);
	if (stackedWidgetPtr != NULL){
		QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
		if (pageWidgetPtr != NULL){
			stackedWidgetPtr->removeWidget(pageWidgetPtr);
		}
	}
}


int CStackWidgetDelegate::GetPagesCount(const QWidget& containerWidget) const
{
	const QStackedWidget* stackedWidgetPtr = dynamic_cast<const QStackedWidget*>(&containerWidget);
	if (stackedWidgetPtr != NULL){
		return stackedWidgetPtr->count();
	}

	return 0;
}


QWidget* CStackWidgetDelegate::GetPageWidgetPtr(const QWidget& containerWidget, int pageIndex) const
{
	const QStackedWidget* stackedWidgetPtr = dynamic_cast<const QStackedWidget*>(&containerWidget);
	if (stackedWidgetPtr != NULL){
		return stackedWidgetPtr->widget(pageIndex);
	}

	return NULL;
}


int CStackWidgetDelegate::GetCurrentPage(const QWidget& containerWidget) const
{
	const QStackedWidget* stackedWidgetPtr = dynamic_cast<const QStackedWidget*>(&containerWidget);
	if (stackedWidgetPtr != NULL){
		return stackedWidgetPtr->currentIndex();
	}

	return -1;
}


bool CStackWidgetDelegate::SetCurrentPage(QWidget& containerWidget, int pageIndex)
{
	QStackedWidget* stackedWidgetPtr = dynamic_cast<QStackedWidget*>(&containerWidget);
	if (stackedWidgetPtr != NULL){
		stackedWidgetPtr->setCurrentIndex(pageIndex);

		return true;
	}

	return false;
}


QString CStackWidgetDelegate::GetPageTitle(const QWidget& /*containerWidget*/, int /*pageIndex*/) const
{
	return QString();
}


void CStackWidgetDelegate::SetPageTitle(QWidget& /*containerWidget*/, int /*pageIndex*/, const QString& /*pageTitle*/)
{
}


QIcon CStackWidgetDelegate::GetPageIcon(const QWidget& containerWidget, int pageIndex) const
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		return pageWidgetPtr->windowIcon();
	}

	return QIcon();
}


void CStackWidgetDelegate::SetPageIcon(QWidget& containerWidget, int pageIndex, const QIcon& pageIcon)
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		QIcon usedIcon = pageIcon;
			
		if (!m_iconSize.isNull()){
			usedIcon = pageIcon.pixmap(m_iconSize);
		}

		pageWidgetPtr->setWindowIcon(usedIcon);
	}
}


QString CStackWidgetDelegate::GetPageToolTip(const QWidget& containerWidget, int pageIndex) const
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		return pageWidgetPtr->toolTip();
	}

	return QString();
}


void CStackWidgetDelegate::SetPageToolTip(QWidget& containerWidget, int pageIndex, const QString& pageToolTip)
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		pageWidgetPtr->setToolTip(pageToolTip);
	}
}


bool CStackWidgetDelegate::IsPageEnabled(const QWidget& containerWidget, int pageIndex) const
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		return pageWidgetPtr->isEnabled();
	}

	return false;
}


bool CStackWidgetDelegate::SetPageEnabled(QWidget& containerWidget, int pageIndex, bool isPageEnabled)
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		pageWidgetPtr->setEnabled(isPageEnabled);

		return true;
	}

	return false;
}


bool CStackWidgetDelegate::IsPageVisible(const QWidget& containerWidget, int pageIndex) const
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		return pageWidgetPtr->isVisible();
	}

	return false;
}


bool CStackWidgetDelegate::SetPageVisible(QWidget& containerWidget, int pageIndex, bool isPageVisible)
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		pageWidgetPtr->setVisible(isPageVisible);

		return true;
	}

	return false;
}


QSize CStackWidgetDelegate::GetPageIconSize(const QWidget& /*containerWidget*/) const
{
	return m_iconSize;
}


bool CStackWidgetDelegate::SetPageIconSize(QWidget& /*containerWidget*/, const QSize& pageIconSize)
{
	m_iconSize = pageIconSize;

	return true;
}


} // namespace iqtgui


