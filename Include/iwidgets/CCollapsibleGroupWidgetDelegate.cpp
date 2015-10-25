#include <iwidgets/CCollapsibleGroupWidgetDelegate.h>


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

// ACF includes
#include "iwidgets/CCollapsiblePage.h"


namespace iwidgets
{


// public methods

CCollapsibleGroupWidgetDelegate::CCollapsibleGroupWidgetDelegate()
	:m_iconSize(32, 32),
	m_containerGuiFlags(CGF_NONE),
	m_orientation(Qt::Vertical)
{
}


// reimplemented (iwidgets::IMultiPageWidgetDelegate)

QWidget* CCollapsibleGroupWidgetDelegate::CreateContainerWidget(QWidget* parentWidgetPtr, int containerGuiFlags, int orientation)
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


bool CCollapsibleGroupWidgetDelegate::ConnectPageIndexListener(
			QWidget& /*containerWidget*/,
			QObject* /*receiverPtr*/,
			const char* /*receiverSlotName*/)
{
	return true;
}


bool CCollapsibleGroupWidgetDelegate::SetPageHeaderPosition(QWidget& /*containerWidget*/, PageHeaderPosition /*pageHeaderPosition*/)
{
	return false;
}


int CCollapsibleGroupWidgetDelegate::InsertPage(
		QWidget& containerWidget,
		QWidget* pageWidgetPtr,
		const QString& pageTitle,
		int /*pageIndex*/)
{
	QLayout* containerLayoutPtr = containerWidget.layout();
	Q_ASSERT(containerLayoutPtr != NULL);

	CCollapsiblePage* groupPanelPtr = new CCollapsiblePage(&containerWidget);
	groupPanelPtr->SetTitle(pageTitle);
	groupPanelPtr->SetWidget(pageWidgetPtr);
	groupPanelPtr->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	containerLayoutPtr->addWidget(groupPanelPtr);

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


void CCollapsibleGroupWidgetDelegate::RemovePage(QWidget& containerWidget, int pageIndex)
{
	QLayout* containerLayoutPtr = containerWidget.layout();
	Q_ASSERT(containerLayoutPtr != NULL);

	delete containerLayoutPtr->takeAt(pageIndex);
}


int CCollapsibleGroupWidgetDelegate::GetPagesCount(const QWidget& containerWidget) const
{
	QLayout* containerLayoutPtr = containerWidget.layout();
	Q_ASSERT(containerLayoutPtr != NULL);

	return containerLayoutPtr->count();
}


QWidget* CCollapsibleGroupWidgetDelegate::GetPageWidgetPtr(const QWidget& containerWidget, int pageIndex) const
{
	Q_ASSERT(pageIndex >= 0);
	Q_ASSERT(pageIndex < GetPagesCount(containerWidget));

	QLayout* containerLayoutPtr = containerWidget.layout();
	Q_ASSERT(containerLayoutPtr != NULL);

	return containerLayoutPtr->itemAt(pageIndex)->widget();
}


int CCollapsibleGroupWidgetDelegate::GetCurrentPage(const QWidget& /*containerWidget*/) const
{
	return -1;
}


bool CCollapsibleGroupWidgetDelegate::SetCurrentPage(QWidget& /*containerWidget*/, int /*pageIndex*/)
{
	return false;
}


QString CCollapsibleGroupWidgetDelegate::GetPageTitle(const QWidget& /*containerWidget*/, int /*pageIndex*/) const
{
	return QString();
}


void CCollapsibleGroupWidgetDelegate::SetPageTitle(QWidget& /*containerWidget*/, int /*pageIndex*/, const QString& /*pageTitle*/)
{
}


QIcon CCollapsibleGroupWidgetDelegate::GetPageIcon(const QWidget& containerWidget, int pageIndex) const
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		pageWidgetPtr->windowIcon();
	}

	return QIcon();
}


void CCollapsibleGroupWidgetDelegate::SetPageIcon(QWidget& containerWidget, int pageIndex, const QIcon& pageIcon)
{
	CCollapsiblePage* pageWidgetPtr = dynamic_cast<CCollapsiblePage*>(GetPageWidgetPtr(containerWidget, pageIndex));
	if (pageWidgetPtr != NULL){
		pageWidgetPtr->SetIcon(pageIcon);
	}
}


QString CCollapsibleGroupWidgetDelegate::GetPageToolTip(const QWidget& containerWidget, int pageIndex) const
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		return pageWidgetPtr->toolTip();
	}

	return QString();
}


void CCollapsibleGroupWidgetDelegate::SetPageToolTip(QWidget& containerWidget, int pageIndex, const QString& pageToolTip)
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		pageWidgetPtr->setToolTip(pageToolTip);
	}
}


bool CCollapsibleGroupWidgetDelegate::IsPageEnabled(const QWidget& containerWidget, int pageIndex) const
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		return pageWidgetPtr->isEnabled();
	}

	return false;
}


bool CCollapsibleGroupWidgetDelegate::SetPageEnabled(QWidget& containerWidget, int pageIndex, bool isPageEnabled)
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		pageWidgetPtr->setEnabled(isPageEnabled);

		return true;
	}

	return false;
}


bool CCollapsibleGroupWidgetDelegate::IsPageVisible(const QWidget& containerWidget, int pageIndex) const
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		return pageWidgetPtr->isVisible();
	}

	return false;
}


bool CCollapsibleGroupWidgetDelegate::SetPageVisible(QWidget& containerWidget, int pageIndex, bool isPageVisible)
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		pageWidgetPtr->setVisible(isPageVisible);
	}

	return false;
}


QSize CCollapsibleGroupWidgetDelegate::GetPageIconSize(const QWidget& /*containerWidget*/) const
{
	return m_iconSize;
}


bool CCollapsibleGroupWidgetDelegate::SetPageIconSize(QWidget& containerWidget, const QSize& pageIconSize)
{
	if (m_iconSize != pageIconSize){
		m_iconSize = pageIconSize;

		int pagesCount = GetPagesCount(containerWidget);
		for (int pageIndex = 0; pageIndex < pagesCount; ++pageIndex){
			CCollapsiblePage* pageWidgetPtr = dynamic_cast<CCollapsiblePage*>(GetPageWidgetPtr(containerWidget, pageIndex));
			if (pageWidgetPtr != NULL){
				pageWidgetPtr->SetIconSize(pageIconSize);
			}
		}
	}

	return true;
}


} // namespace iwidgets


