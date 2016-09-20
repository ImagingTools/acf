#ifndef iwidgets_CCollapsiblePage_included
#define iwidgets_CCollapsiblePage_included


// ACF includes
#include <GeneratedFiles/iwidgets/ui_CCollapsiblePage.h>


namespace iwidgets
{


class CCollapsiblePage: public QWidget, public Ui::CCollapsiblePage
{
	Q_OBJECT
public:
	typedef QWidget BaseClass;

	CCollapsiblePage(QWidget* parentPtr = NULL);

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


#endif // !iwidgets_CCollapsiblePage_included


