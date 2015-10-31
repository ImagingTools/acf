#include <iwidgets/CCollapsiblePage.h>


namespace iwidgets
{


// public methods
CCollapsiblePage::CCollapsiblePage(QWidget* parentPtr)
	:BaseClass(parentPtr),
	m_pageWidgetPtr(NULL),
	m_iconSize(32, 32)
{
	setupUi(this);

	CollapsiblePageIconLabel->setHidden(true);

	connect(CollapseButton, SIGNAL(toggled(bool)), this, SLOT(SetPageVisible(bool)));
}


void CCollapsiblePage::SetTitle(const QString& text)
{
	CollapseButton->setText(text);
}


void CCollapsiblePage::SetWidget(QWidget* pageWidgetPtr)
{
	CollapsiblePageFrame->layout()->addWidget(pageWidgetPtr);

	m_pageWidgetPtr = pageWidgetPtr;
}


void CCollapsiblePage::SetIconSize(const QSize& size)
{
	CollapsiblePageIconLabel->setMinimumSize(size);
	CollapsiblePageIconLabel->setMaximumSize(size);

	m_iconSize = size;

	CollapsiblePageIconLabel->setPixmap(m_icon.pixmap(size));
}


void CCollapsiblePage::SetIcon(const QIcon& icon)
{
	m_icon = icon;

	CollapsiblePageIconLabel->setVisible(!m_icon.isNull());

	CollapsiblePageIconLabel->setPixmap(m_icon.pixmap(m_iconSize));
}


// private slots

void CCollapsiblePage::SetPageVisible(bool visible)
{
	CollapsiblePageFrame->setVisible(visible);

	if (visible){
		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	}
	else{
		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
	}
}


} // namespace iwidgets


