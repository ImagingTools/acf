#ifndef iwidgets_CMiniWidgetPage_included
#define iwidgets_CMiniWidgetPage_included


// ACF includes
#include <GeneratedFiles/iwidgets/ui_CMiniWidgetPage.h>


namespace iwidgets
{


class CMiniWidgetPage: public QWidget, public Ui::CMiniWidgetPage
{
	Q_OBJECT
public:
	typedef QWidget BaseClass;

	CMiniWidgetPage(QWidget* parentPtr = NULL);

	void SetTitle(const QString& text);
	void SetWidget(QWidget* pageWidgetPtr);
	void SetIconSize(const QSize& size);
	void SetIcon(const QIcon& icon);

private Q_SLOTS:
	void SetPageVisible(bool visible);

private:
	QIcon m_icon;
	QWidget* m_pageWidgetPtr;
	QSize m_iconSize;
};

} // namespace iwidgets


#endif // !iwidgets_CMiniWidgetPage_included


