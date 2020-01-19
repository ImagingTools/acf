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

	connect(this, SIGNAL(PostUpdatePalette()), this, SLOT(OnUpdatePalette()), Qt::QueuedConnection);
}


void CMiniWidgetPage::SetTitle(const QString& text)
{
	PageLabel->setText(text);
}


void CMiniWidgetPage::SetWidget(QWidget* pageWidgetPtr)
{
	PageFrame->layout()->addWidget(pageWidgetPtr);

	m_pageWidgetPtr = pageWidgetPtr;

	Q_EMIT PostUpdatePalette();
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


void CMiniWidgetPage::OnUpdatePalette()
{
	QPalette currentPalette = palette();

	currentPalette.setBrush(QPalette::Active, QPalette::Text, QColor("#335777"));
	currentPalette.setBrush(QPalette::WindowText, QColor("#335777"));
	currentPalette.setBrush(QPalette::ButtonText, QColor("#335777"));
	currentPalette.setBrush(QPalette::BrightText, QColor("#335777"));

	currentPalette.setBrush(QPalette::Active, QPalette::Button, QColor("#ebebee"));
	currentPalette.setBrush(QPalette::Inactive, QPalette::Button, QColor("#ebebee"));
	currentPalette.setBrush(QPalette::Disabled, QPalette::Button, QColor("#ebebee"));

	currentPalette.setBrush(QPalette::Active, QPalette::Shadow, QColor("#7d785b"));
	currentPalette.setBrush(QPalette::Inactive, QPalette::Shadow, QColor("#7d785b"));
	currentPalette.setBrush(QPalette::Disabled, QPalette::Shadow, QColor("#7d785b"));

	currentPalette.setBrush(QPalette::Active, QPalette::Highlight, QColor("#7d785b"));
	currentPalette.setBrush(QPalette::Inactive, QPalette::Highlight, QColor("#7d785b"));
	currentPalette.setBrush(QPalette::Disabled, QPalette::Highlight, QColor("#7d785b"));
	
	currentPalette.setBrush(QPalette::Active, QPalette::Base, QColor("#ebebee"));
	currentPalette.setBrush(QPalette::Inactive, QPalette::Base, QColor("#ebebee"));
	currentPalette.setBrush(QPalette::Disabled, QPalette::Base, QColor("#ebebee"));

	currentPalette.setBrush(QPalette::Active, QPalette::Background, QColor("#ebebee"));
	currentPalette.setBrush(QPalette::Inactive, QPalette::Background, QColor("#ebebee"));
	currentPalette.setBrush(QPalette::Disabled, QPalette::Background, QColor("#ebebee"));

	m_pageWidgetPtr->setPalette(currentPalette);

	QList<QWidget*> widgets = m_pageWidgetPtr->findChildren<QWidget*>();
	Q_FOREACH(QWidget* w, widgets){
		w->setPalette(currentPalette);
	}
}


} // namespace iwidgets


