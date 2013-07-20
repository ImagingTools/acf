#include <iqtgui/CSplitterDelegate.h>


// Qt includes
#include <QtCore/QVariant>
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>
#include <QtGui/QIcon>
#include <QtGui/QGroupBox>


namespace iqtgui
{


// public methods

QWidget* CSplitterDelegate::CreateContainerWidget(QWidget* parentWidgetPtr, int orientation)
{	
	QSplitter* splitterPtr = new QSplitter(parentWidgetPtr);

	splitterPtr->setOrientation(Qt::Orientation(orientation));

	return splitterPtr;
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
	panelLayoutPtr->setMargin(0);
	panelLayoutPtr->addWidget(pageWidgetPtr);

	splitterPtr->insertWidget(pageIndex, panelPtr);

	return pageIndex;
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


int CSplitterDelegate::GetCurrentPage(QWidget& /*containerWidget*/) const
{
	return -1;
}


bool CSplitterDelegate::SetCurrentPage(QWidget& /*containerWidget*/, int /*pageIndex*/)
{
	return false;
}


QString CSplitterDelegate::GetPageTitle(QWidget& /*containerWidget*/, int /*pageIndex*/) const
{
	return QString();
}


void CSplitterDelegate::SetPageTitle(QWidget& /*containerWidget*/, int /*pageIndex*/, const QString& /*pageTitle*/)
{
}


QIcon CSplitterDelegate::GetPageIcon(QWidget& containerWidget, int pageIndex) const
{
	QSplitter* splitterPtr = dynamic_cast<QSplitter*>(&containerWidget);
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


QString CSplitterDelegate::GetPageToolTip(QWidget& containerWidget, int pageIndex) const
{
	QSplitter* splitterPtr = dynamic_cast<QSplitter*>(&containerWidget);
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


bool CSplitterDelegate::IsPageEnabled(QWidget& containerWidget, int pageIndex) const
{
	QSplitter* splitterPtr = dynamic_cast<QSplitter*>(&containerWidget);
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


bool CSplitterDelegate::IsPageVisible(QWidget& containerWidget, int pageIndex) const
{
	QSplitter* splitterPtr = dynamic_cast<QSplitter*>(&containerWidget);
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


QSize CSplitterDelegate::GetPageIconSize(QWidget& /*containerWidget*/) const
{
	return m_iconSize;
}


bool CSplitterDelegate::SetPageIconSize(QWidget& /*containerWidget*/, const QSize& pageIconSize)
{
	m_iconSize = pageIconSize;

	return true;
}


} // namespace iqtgui


