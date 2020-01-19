#include <iwidgets/CMiniWidgetDelegate.h>


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
#include <iwidgets/CMiniWidgetPage.h>


namespace iwidgets
{


// public methods

CMiniWidgetDelegate::CMiniWidgetDelegate()
	:m_iconSize(32, 32),
	m_containerGuiFlags(CGF_NONE),
	m_orientation(Qt::Vertical)
{
}


// reimplemented (iwidgets::IMultiPageWidgetDelegate)

QWidget* CMiniWidgetDelegate::CreateContainerWidget(QWidget* parentWidgetPtr, int containerGuiFlags, int orientation)
{
	QWidget* containerPtr = new QWidget(parentWidgetPtr);
	containerPtr->setObjectName("WidgetsPanel");
	containerPtr->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	containerPtr->setStyleSheet("QWidget#WidgetsPanel{background:white;}");

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

	boxLayoutPtr->setMargin(11);
	boxLayoutPtr->setSpacing(11);

	m_containerGuiFlags = containerGuiFlags;
	m_orientation = orientation;

	return containerPtr;
}


bool CMiniWidgetDelegate::ConnectPageIndexListener(
			QWidget& /*containerWidget*/,
			QObject* /*receiverPtr*/,
			const char* /*receiverSlotName*/)
{
	return true;
}


bool CMiniWidgetDelegate::SetPageHeaderPosition(QWidget& /*containerWidget*/, PageHeaderPosition /*pageHeaderPosition*/)
{
	return false;
}


int CMiniWidgetDelegate::InsertPage(
		QWidget& containerWidget,
		QWidget* pageWidgetPtr,
		const QString& pageTitle,
		int /*pageIndex*/)
{

	QLayout* containerLayoutPtr = containerWidget.layout();
	Q_ASSERT(containerLayoutPtr != NULL);

	CMiniWidgetPage* groupPanelPtr = new CMiniWidgetPage(&containerWidget);
	groupPanelPtr->SetTitle(pageTitle);
	groupPanelPtr->SetWidget(pageWidgetPtr);
	groupPanelPtr->SetIconSize(m_iconSize);
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

		if (layoutItemPtr->spacerItem()){
			boxLayoutPtr->removeItem(layoutItemPtr);
			--elementsCount;
			--i;
		}
	}

	boxLayoutPtr->addStretch();

	return containerLayoutPtr->count() - 2;
}


void CMiniWidgetDelegate::RemovePage(QWidget& containerWidget, int pageIndex)
{
	QLayout* containerLayoutPtr = containerWidget.layout();
	Q_ASSERT(containerLayoutPtr != NULL);

	delete containerLayoutPtr->takeAt(pageIndex);
}


void CMiniWidgetDelegate::ResetPages(QWidget& containerWidget)
{
	QLayout* containerLayoutPtr = containerWidget.layout();
	Q_ASSERT(containerLayoutPtr != NULL);

	ClearLayout(containerLayoutPtr);
}


int CMiniWidgetDelegate::GetPagesCount(const QWidget& containerWidget) const
{
	QLayout* containerLayoutPtr = containerWidget.layout();
	Q_ASSERT(containerLayoutPtr != NULL);

	return containerLayoutPtr->count() - 1;
}


QWidget* CMiniWidgetDelegate::GetPageWidgetPtr(const QWidget& containerWidget, int pageIndex) const
{
	QLayout* containerLayoutPtr = containerWidget.layout();
	Q_ASSERT(containerLayoutPtr != NULL);

	return containerLayoutPtr->itemAt(pageIndex)->widget();
}


int CMiniWidgetDelegate::GetCurrentPage(const QWidget& /*containerWidget*/) const
{
	return -1;
}


bool CMiniWidgetDelegate::SetCurrentPage(QWidget& /*containerWidget*/, int /*pageIndex*/)
{
	return false;
}


QString CMiniWidgetDelegate::GetPageTitle(const QWidget& /*containerWidget*/, int /*pageIndex*/) const
{
	return QString();
}


void CMiniWidgetDelegate::SetPageTitle(QWidget& /*containerWidget*/, int /*pageIndex*/, const QString& /*pageTitle*/)
{
}


QIcon CMiniWidgetDelegate::GetPageIcon(const QWidget& containerWidget, int pageIndex) const
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		pageWidgetPtr->windowIcon();
	}

	return QIcon();
}


void CMiniWidgetDelegate::SetPageIcon(QWidget& containerWidget, int pageIndex, const QIcon& pageIcon)
{
	CMiniWidgetPage* pageWidgetPtr = dynamic_cast<CMiniWidgetPage*>(GetPageWidgetPtr(containerWidget, pageIndex));
	if (pageWidgetPtr != NULL){
		pageWidgetPtr->SetIcon(pageIcon);
	}
}


QString CMiniWidgetDelegate::GetPageToolTip(const QWidget& containerWidget, int pageIndex) const
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		return pageWidgetPtr->toolTip();
	}

	return QString();
}


void CMiniWidgetDelegate::SetPageToolTip(QWidget& containerWidget, int pageIndex, const QString& pageToolTip)
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		pageWidgetPtr->setToolTip(pageToolTip);
	}
}


bool CMiniWidgetDelegate::IsPageEnabled(const QWidget& containerWidget, int pageIndex) const
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		return pageWidgetPtr->isEnabled();
	}

	return false;
}


bool CMiniWidgetDelegate::SetPageEnabled(QWidget& containerWidget, int pageIndex, bool isPageEnabled)
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		pageWidgetPtr->setEnabled(isPageEnabled);

		return true;
	}

	return false;
}


bool CMiniWidgetDelegate::IsPageVisible(const QWidget& containerWidget, int pageIndex) const
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		return pageWidgetPtr->isVisible();
	}

	return false;
}


bool CMiniWidgetDelegate::SetPageVisible(QWidget& containerWidget, int pageIndex, bool isPageVisible)
{
	QWidget* pageWidgetPtr = GetPageWidgetPtr(containerWidget, pageIndex);
	if (pageWidgetPtr != NULL){
		pageWidgetPtr->setVisible(isPageVisible);
	}

	return false;
}


QSize CMiniWidgetDelegate::GetPageIconSize(const QWidget& /*containerWidget*/) const
{
	return m_iconSize;
}


bool CMiniWidgetDelegate::SetPageIconSize(QWidget& containerWidget, const QSize& pageIconSize)
{
	if (m_iconSize != pageIconSize){
		m_iconSize = pageIconSize;

		int pagesCount = GetPagesCount(containerWidget);
		for (int pageIndex = 0; pageIndex < pagesCount; ++pageIndex){
			CMiniWidgetPage* pageWidgetPtr = dynamic_cast<CMiniWidgetPage*>(GetPageWidgetPtr(containerWidget, pageIndex));
			if (pageWidgetPtr != NULL){
				pageWidgetPtr->SetIconSize(pageIconSize);
			}
		}
	}

	return true;
}


} // namespace iwidgets


