#include <iwidgets/CSplitterDelegate.h>


// Qt includes
#include <QtCore/QtGlobal>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGroupBox>
#else
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGroupBox>
#endif

namespace iwidgets
{


// public methods

// reimplemented (IMultiPageWidgetDelegate)

QWidget* CSplitterDelegate::CreateContainerWidget(QWidget* parentWidgetPtr, int /*containerGuiFlags*/, int orientation)
{	
	QSplitter* splitterPtr = new QSplitter(parentWidgetPtr);

	splitterPtr->setOrientation(Qt::Orientation(orientation));

	return splitterPtr;
}


bool CSplitterDelegate::ConnectPageIndexListener(
			QWidget& /*containerWidget*/,
			QObject* /*receiverPtr*/,
			const char* /*receiverSlotName*/)
{
	return true;
}


bool CSplitterDelegate::SetPageHeaderPosition(QWidget& /*containerWidget*/, PageHeaderPosition /*pageHeaderPosition*/)
{
	return false;
}


int CSplitterDelegate::InsertPage(
		QWidget& containerWidget,
		QWidget* pageWidgetPtr,
		const QString& pageTitle,
		int pageIndex)
{
	QSplitter* splitterPtr = dynamic_cast<QSplitter*>(&containerWidget);
	if (splitterPtr == NULL){
		return -1;
	}

	QWidget* panelPtr = pageTitle.isEmpty() ? new QWidget(splitterPtr) : new QGroupBox(pageTitle, splitterPtr);
	QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
	panelLayoutPtr->addWidget(pageWidgetPtr);

	if (pageTitle.isEmpty()){
		panelLayoutPtr->setMargin(0);
	}

	splitterPtr->insertWidget(pageIndex, panelPtr);

	int insertPosition = pageIndex;
	if (insertPosition < 0){
		insertPosition = splitterPtr->count() - 1;
	}

	if (insertPosition >= splitterPtr->count()){
		insertPosition = splitterPtr->count() - 1;
	}

	return insertPosition;
}


void CSplitterDelegate::RemovePage(QWidget& containerWidget, int pageIndex)
{
	QSplitter* splitterPtr = dynamic_cast<QSplitter*>(&containerWidget);
	if (splitterPtr != NULL){
		QWidget* pageWidgetPtr = splitterPtr->widget(pageIndex);
		if (pageWidgetPtr != NULL){
			delete pageWidgetPtr;
		}
	}
}


int CSplitterDelegate::GetPagesCount(const QWidget& containerWidget) const
{
	const QSplitter* splitterPtr = dynamic_cast<const QSplitter*>(&containerWidget);
	if (splitterPtr != NULL){
		return splitterPtr->count();
	}

	return 0;
}


QWidget* CSplitterDelegate::GetPageWidgetPtr(const QWidget& containerWidget, int pageIndex) const
{
	const QSplitter* splitterPtr = dynamic_cast<const QSplitter*>(&containerWidget);
	if (splitterPtr != NULL){
		return splitterPtr->widget(pageIndex);
	}

	return NULL;
}


int CSplitterDelegate::GetCurrentPage(const QWidget& /*containerWidget*/) const
{
	return -1;
}


bool CSplitterDelegate::SetCurrentPage(QWidget& /*containerWidget*/, int /*pageIndex*/)
{
	return false;
}


QString CSplitterDelegate::GetPageTitle(const QWidget& /*containerWidget*/, int /*pageIndex*/) const
{
	return QString();
}


void CSplitterDelegate::SetPageTitle(QWidget& /*containerWidget*/, int /*pageIndex*/, const QString& /*pageTitle*/)
{
}


QIcon CSplitterDelegate::GetPageIcon(const QWidget& containerWidget, int pageIndex) const
{
	const QSplitter* splitterPtr = dynamic_cast<const QSplitter*>(&containerWidget);
	if (splitterPtr != NULL){
		QWidget* widgetPtr = splitterPtr->widget(pageIndex);
		if (widgetPtr != NULL){
			return widgetPtr->windowIcon();
		}
	}

	return QIcon();
}


void CSplitterDelegate::SetPageIcon(QWidget& containerWidget, int pageIndex, const QIcon& pageIcon)
{
	QSplitter* splitterPtr = dynamic_cast<QSplitter*>(&containerWidget);
	if (splitterPtr != NULL){
		QWidget* widgetPtr = splitterPtr->widget(pageIndex);
		if (widgetPtr != NULL){

			QIcon usedIcon = pageIcon;
			
			if (!m_iconSize.isNull()){
				usedIcon = pageIcon.pixmap(m_iconSize);
			}

			return widgetPtr->setWindowIcon(usedIcon);
		}
	}
}


QString CSplitterDelegate::GetPageToolTip(const QWidget& containerWidget, int pageIndex) const
{
	const QSplitter* splitterPtr = dynamic_cast<const QSplitter*>(&containerWidget);
	if (splitterPtr != NULL){
		QWidget* widgetPtr = splitterPtr->widget(pageIndex);
		if (widgetPtr != NULL){
			return widgetPtr->toolTip();
		}
	}

	return QString();
}


void CSplitterDelegate::SetPageToolTip(QWidget& containerWidget, int pageIndex, const QString& pageToolTip)
{
	QSplitter* splitterPtr = dynamic_cast<QSplitter*>(&containerWidget);
	if (splitterPtr != NULL){
		QWidget* widgetPtr = splitterPtr->widget(pageIndex);
		if (widgetPtr != NULL){
			return widgetPtr->setToolTip(pageToolTip);
		}
	}
}


bool CSplitterDelegate::IsPageEnabled(const QWidget& containerWidget, int pageIndex) const
{
	const QSplitter* splitterPtr = dynamic_cast<const QSplitter*>(&containerWidget);
	if (splitterPtr != NULL){
		QWidget* widgetPtr = splitterPtr->widget(pageIndex);
		if (widgetPtr != NULL){
			return widgetPtr->isEnabled();
		}
	}

	return false;
}


bool CSplitterDelegate::SetPageEnabled(QWidget& containerWidget, int pageIndex, bool isPageEnabled)
{
	QSplitter* splitterPtr = dynamic_cast<QSplitter*>(&containerWidget);
	if (splitterPtr != NULL){
		QWidget* widgetPtr = splitterPtr->widget(pageIndex);
		if (widgetPtr != NULL){
			widgetPtr->setEnabled(isPageEnabled);

			return true;
		}
	}

	return false;
}


bool CSplitterDelegate::IsPageVisible(const QWidget& containerWidget, int pageIndex) const
{
	const QSplitter* splitterPtr = dynamic_cast<const QSplitter*>(&containerWidget);
	if (splitterPtr != NULL){
		QWidget* widgetPtr = splitterPtr->widget(pageIndex);
		if (widgetPtr != NULL){
			return widgetPtr->isVisible();
		}

		return true;
	}

	return false;
}


bool CSplitterDelegate::SetPageVisible(QWidget& containerWidget, int pageIndex, bool isPageVisible)
{
	QSplitter* splitterPtr = dynamic_cast<QSplitter*>(&containerWidget);
	if (splitterPtr != NULL){
		QWidget* widgetPtr = splitterPtr->widget(pageIndex);
		if (widgetPtr != NULL){
			widgetPtr->setVisible(isPageVisible);

			return true;
		}
	}

	return false;
}


QSize CSplitterDelegate::GetPageIconSize(const QWidget& /*containerWidget*/) const
{
	return m_iconSize;
}


bool CSplitterDelegate::SetPageIconSize(QWidget& /*containerWidget*/, const QSize& pageIconSize)
{
	m_iconSize = pageIconSize;

	return true;
}


} // namespace iwidgets


