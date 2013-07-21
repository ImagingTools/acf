#include <iqtgui/CToolBoxDelegate.h>


// Qt includes
#include <QtCore/QVariant>
#include <QtGui/QToolBox>
#include <QtGui/QVBoxLayout>


namespace iqtgui
{


// public methods

QWidget* CToolBoxDelegate::CreateContainerWidget(QWidget* parentWidgetPtr, int /*orientation*/)
{	
	QToolBox* toolBoxPtr = new QToolBox(parentWidgetPtr);

	toolBoxPtr->setBackgroundRole(QPalette::Window);

	return toolBoxPtr;
}


bool CToolBoxDelegate::SetPageHeaderPosition(QWidget& /*containerWidget*/, PageHeaderPosition /*pageHeaderPosition*/)
{
	return false;
}


int CToolBoxDelegate::InsertPage(
		QWidget& containerWidget,
		QWidget* pageWidgetPtr,
		const QString& pageTitle,
		int pageIndex)
{
	QToolBox* toolBoxPtr = dynamic_cast<QToolBox*>(&containerWidget);
	if (toolBoxPtr == NULL){
		return -1;
	}

	QWidget* panelPtr = new QWidget(toolBoxPtr);
	QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
	panelLayoutPtr->setContentsMargins(6, 0, 6, 0);

	pageIndex = toolBoxPtr->insertItem(pageIndex, panelPtr, pageTitle);

	panelLayoutPtr->addWidget(pageWidgetPtr);

	// Add a spacer on the bottom the tab page:
	QSpacerItem* spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
	panelLayoutPtr->addItem(spacerPtr);

	return pageIndex;
}


void CToolBoxDelegate::RemovePage(QWidget& containerWidget, int pageIndex)
{
	QToolBox* toolBoxPtr = dynamic_cast<QToolBox*>(&containerWidget);
	if (toolBoxPtr != NULL){
		toolBoxPtr->removeItem(pageIndex);
	}
}


int CToolBoxDelegate::GetPagesCount(const QWidget& containerWidget) const
{
	const QToolBox* toolBoxPtr = dynamic_cast<const QToolBox*>(&containerWidget);
	if (toolBoxPtr != NULL){
		return toolBoxPtr->count();
	}

	return 0;
}


QWidget* CToolBoxDelegate::GetPageWidgetPtr(const QWidget& containerWidget, int pageIndex) const
{
	const QToolBox* toolBoxPtr = dynamic_cast<const QToolBox*>(&containerWidget);
	if (toolBoxPtr != NULL){
		return toolBoxPtr->widget(pageIndex);
	}

	return NULL;
}


int CToolBoxDelegate::GetCurrentPage(const QWidget& containerWidget) const
{
	return containerWidget.property("currentIndex").toInt();
}


bool CToolBoxDelegate::SetCurrentPage(QWidget& containerWidget, int pageIndex)
{
	return containerWidget.setProperty("currentIndex", pageIndex);
}


QString CToolBoxDelegate::GetPageTitle(const QWidget& containerWidget, int pageIndex) const
{
	const QToolBox* toolBoxPtr = dynamic_cast<const QToolBox*>(&containerWidget);
	if (toolBoxPtr != NULL){
		return toolBoxPtr->itemText(pageIndex);
	}

	return QString();
}


void CToolBoxDelegate::SetPageTitle(QWidget& containerWidget, int pageIndex, const QString& pageTitle)
{
	QToolBox* toolBoxPtr = dynamic_cast<QToolBox*>(&containerWidget);
	if (toolBoxPtr != NULL){
		toolBoxPtr->setItemText(pageIndex, pageTitle);
	}
}


QIcon CToolBoxDelegate::GetPageIcon(const QWidget& containerWidget, int pageIndex) const
{
	const QToolBox* toolBoxPtr = dynamic_cast<const QToolBox*>(&containerWidget);
	if (toolBoxPtr != NULL){
		return toolBoxPtr->itemIcon(pageIndex);
	}

	return QIcon();
}


void CToolBoxDelegate::SetPageIcon(QWidget& containerWidget, int pageIndex, const QIcon& pageIcon)
{
	QToolBox* toolBoxPtr = dynamic_cast<QToolBox*>(&containerWidget);
	if (toolBoxPtr != NULL){
		toolBoxPtr->setItemIcon(pageIndex, pageIcon);
	}
}


QString CToolBoxDelegate::GetPageToolTip(const QWidget& containerWidget, int pageIndex) const
{
	const QToolBox* toolBoxPtr = dynamic_cast<const QToolBox*>(&containerWidget);
	if (toolBoxPtr != NULL){
		return toolBoxPtr->itemToolTip(pageIndex);
	}

	return QString();
}


void CToolBoxDelegate::SetPageToolTip(QWidget& containerWidget, int pageIndex, const QString& pageToolTip)
{
	QToolBox* toolBoxPtr = dynamic_cast<QToolBox*>(&containerWidget);
	if (toolBoxPtr != NULL){
		toolBoxPtr->setItemToolTip(pageIndex, pageToolTip);
	}
}


bool CToolBoxDelegate::IsPageEnabled(const QWidget& containerWidget, int pageIndex) const
{
	const QToolBox* toolBoxPtr = dynamic_cast<const QToolBox*>(&containerWidget);
	if (toolBoxPtr != NULL){
		return toolBoxPtr->isItemEnabled(pageIndex);
	}

	return false;
}


bool CToolBoxDelegate::SetPageEnabled(QWidget& containerWidget, int pageIndex, bool isPageEnabled)
{
	QToolBox* toolBoxPtr = dynamic_cast<QToolBox*>(&containerWidget);
	if (toolBoxPtr != NULL){
		toolBoxPtr->setItemEnabled(pageIndex, isPageEnabled);

		return true;
	}

	return false;
}


bool CToolBoxDelegate::IsPageVisible(const QWidget& /*containerWidget*/, int /*pageIndex*/) const
{
	return true;
}


bool CToolBoxDelegate::SetPageVisible(QWidget& /*containerWidget*/, int /*pageIndex*/, bool /*isPageVisible*/)
{
	return false;
}


QSize CToolBoxDelegate::GetPageIconSize(const QWidget& containerWidget) const
{
	return containerWidget.property("iconSize").toSize();
}


bool CToolBoxDelegate::SetPageIconSize(QWidget& containerWidget, const QSize& pageIconSize)
{
	return containerWidget.setProperty("iconSize", pageIconSize);
}


} // namespace iqtgui


