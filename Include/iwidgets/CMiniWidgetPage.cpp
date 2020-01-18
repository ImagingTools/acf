#include <iwidgets/CMiniWidgetPage.h>


namespace iwidgets
{


// public methods

CMiniWidgetPage::CMiniWidgetPage(QWidget* parentPtr)
	:BaseClass(parentPtr),
	m_pageWidgetPtr(NULL),
	m_iconSize(32, 32)
{
	setupUi(this);

	PageIconToolButton->setHidden(true);
}


void CMiniWidgetPage::SetTitle(const QString& text)
{
	PageLabel->setText(text);
}


void CMiniWidgetPage::SetWidget(QWidget* pageWidgetPtr)
{
	PageFrame->layout()->addWidget(pageWidgetPtr);

	m_pageWidgetPtr = pageWidgetPtr;
}


void CMiniWidgetPage::SetIconSize(const QSize& size)
{
	PageIconToolButton->setMinimumSize(size);
	PageIconToolButton->setMaximumSize(size);

	m_iconSize = size;

	PageIconToolButton->setIconSize(m_iconSize);
	PageIconToolButton->setIcon(m_icon);
}


void CMiniWidgetPage::SetIcon(const QIcon& icon)
{
	m_icon = icon;

	PageIconToolButton->setVisible(!m_icon.isNull());

	PageIconToolButton->setIcon(m_icon);
}


// private slots

void CMiniWidgetPage::SetPageVisible(bool visible)
{
	PageFrame->setVisible(visible);

	if (visible){
		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	}
	else{
		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
	}
}


} // namespace iwidgets


