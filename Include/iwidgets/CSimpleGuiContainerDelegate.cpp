#include <iwidgets/CSimpleGuiContainerDelegate.h>


// Qt includes
#include <QtCore/QtGlobal>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGroupBox>
#else
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGroupBox>
#endif


namespace iwidgets
{


CSimpleGuiContainerDelegate::CSimpleGuiContainerDelegate()
	:m_containerGuiFlags(CGF_NONE),
	m_orientation(Qt::Vertical)
{
}


// public methods

// reimplemented (iwidgets::IMultiPageWidgetDelegate)

QWidget* CSimpleGuiContainerDelegate::CreateContainerWidget(QWidget* parentWidgetPtr, int containerGuiFlags, int orientation)
{	
	QWidget* containerPtr = new QWidget(parentWidgetPtr);

	QBoxLayout* boxLayoutPtr = NULL;

	switch (orientation){
		case Qt::Vertical:
			boxLayoutPtr = new QVBoxLayout(containerPtr);
			break;
		case Qt::Horizontal:
			boxLayoutPtr = new QHBoxLayout(containerPtr);
			break;
	}

	Q_ASSERT(boxLayoutPtr != NULL);

	boxLayoutPtr->setMargin(0);

	m_containerGuiFlags = containerGuiFlags;
	m_orientation = orientation;

	return containerPtr;
}


bool CSimpleGuiContainerDelegate::ConnectPageIndexListener(
			QWidget& /*containerWidget*/,
			QObject* /*receiverPtr*/,
			const char* /*receiverSlotName*/)
{
	return true;
}


bool CSimpleGuiContainerDelegate::SetPageHeaderPosition(QWidget& /*containerWidget*/, PageHeaderPosition /*pageHeaderPosition*/)
{
	return false;
}


int CSimpleGuiContainerDelegate::InsertPage(
		QWidget& containerWidget,
		QWidget* pageWidgetPtr,
		const QString& pageTitle,
		int pageIndex)
{
	QLayout* containerLayoutPtr = containerWidget.layout();
	Q_ASSERT(containerLayoutPtr != NULL);

	QWidget* panelPtr = pageTitle.isEmpty() ? new QWidget(&containerWidget) : new QGroupBox(pageTitle, &containerWidget);
	panelPtr->setObjectName(QString("Page %1").arg(pageIndex));
	QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);

	if (pageTitle.isEmpty()){
		panelLayoutPtr->setMargin(0);
	}

	panelLayoutPtr->addWidget(pageWidgetPtr);

	containerLayoutPtr->addWidget(panelPtr);

	QBoxLayout* boxLayoutPtr = dynamic_cast<QBoxLayout*>(containerLayoutPtr);
	Q_ASSERT(boxLayoutPtr != NULL);

	int elementsCount = containerLayoutPtr->count();
	for (int i = 0; i < elementsCount; ++i){
		QLayoutItem* layoutItemPtr = boxLayoutPtr->itemAt(i);

		if ((layoutItemPtr != NULL) && (m_containerGuiFlags & CGF_UNIFORM_SIZES)){
			QWidget* widgetPtr = layoutItemPtr->widget();
			if (widgetPtr != NULL){
				boxLayoutPtr->setStretchFactor(widgetPtr, 1);
			}
		}
	}

	return containerLayoutPtr->count() - 1;
}


void CSimpleGuiContainerDelegate::RemovePage(QWidget& containerWidget, int pageIndex)
{
	QLayout* containerLayoutPtr = containerWidget.layout();
	Q_ASSERT(containerLayoutPtr != NULL);

	delete containerLayoutPtr->takeAt(pageIndex);
}


void CSimpleGuiContainerDelegate::ResetPages(QWidget& containerWidget)
{
	QLayout* containerLayoutPtr = containerWidget.layout();
	Q_ASSERT(containerLayoutPtr != NULL);

	ClearLayout(containerLayoutPtr);
}


int CSimpleGuiContainerDelegate::GetPagesCount(const QWidget& containerWidget) const
{
	QLayout* containerLayoutPtr = containerWidget.layout();
	Q_ASSERT(containerLayoutPtr != NULL);

	return containerLayoutPtr->count();
}


QWidget* CSimpleGuiContainerDelegate::GetPageWidgetPtr(const QWidget& containerWidget, int pageIndex) const
{
	Q_ASSERT(pageIndex >= 0);
	Q_ASSERT(pageIndex < GetPagesCount(containerWidget));

	QLayout* containerLayoutPtr = containerWidget.layout();
	Q_ASSERT(containerLayoutPtr != NULL);

	return containerLayoutPtr->itemAt(pageIndex)->widget();
}


int CSimpleGuiContainerDelegate::GetCurrentPage(const QWidget& /*containerWidget*/) const
{
	return -1;
}


bool CSimpleGuiContainerDelegate::SetCurrentPage(QWidget& /*containerWidget*/, int /*pageIndex*/)
{
	return false;
}


QString CSimpleGuiContainerDelegate::GetPageTitle(const QWidget& /*containerWidget*/, int /*pageIndex*/) const
{
	return QString();
}


void CSimpleGuiContainerDelegate::SetPageTitle(QWidget& /*containerWidget*/, int /*pageIndex*/, const QString& /*pageTitle*/)
{
}


QIcon CSimpleGuiContainerDelegate::GetPageIcon(const QWidget& containerWidget, int pageIndex) const
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		pageWidgetPtr->windowIcon();
	}

	return QIcon();
}


void CSimpleGuiContainerDelegate::SetPageIcon(QWidget& containerWidget, int pageIndex, const QIcon& pageIcon)
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		QIcon usedIcon = pageIcon;

		if (!m_iconSize.isNull()){
			usedIcon = pageIcon.pixmap(m_iconSize);
		}

		return pageWidgetPtr->setWindowIcon(usedIcon);
	}
}


QString CSimpleGuiContainerDelegate::GetPageToolTip(const QWidget& containerWidget, int pageIndex) const
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		return pageWidgetPtr->toolTip();
	}

	return QString();
}


void CSimpleGuiContainerDelegate::SetPageToolTip(QWidget& containerWidget, int pageIndex, const QString& pageToolTip)
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		pageWidgetPtr->setToolTip(pageToolTip);
	}
}


bool CSimpleGuiContainerDelegate::IsPageEnabled(const QWidget& containerWidget, int pageIndex) const
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		return pageWidgetPtr->isEnabled();
	}

	return false;
}


bool CSimpleGuiContainerDelegate::SetPageEnabled(QWidget& containerWidget, int pageIndex, bool isPageEnabled)
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		pageWidgetPtr->setEnabled(isPageEnabled);

		return true;
	}

	return false;
}


bool CSimpleGuiContainerDelegate::IsPageVisible(const QWidget& containerWidget, int pageIndex) const
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		return pageWidgetPtr->isVisible();
	}

	return false;
}


bool CSimpleGuiContainerDelegate::SetPageVisible(QWidget& containerWidget, int pageIndex, bool isPageVisible)
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		pageWidgetPtr->setVisible(isPageVisible);
	}

	return false;
}


QSize CSimpleGuiContainerDelegate::GetPageIconSize(const QWidget& /*containerWidget*/) const
{
	return m_iconSize;
}


bool CSimpleGuiContainerDelegate::SetPageIconSize(QWidget& /*containerWidget*/, const QSize& pageIconSize)
{
	m_iconSize = pageIconSize;

	return true;
}


} // namespace iwidgets


