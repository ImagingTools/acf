#ifndef iwidgets_CCollapsiblePage_included
#define iwidgets_CCollapsiblePage_included


// ACF includes
#include "GeneratedFiles/iwidgets/ui_CCollapsiblePage.h"


namespace iwidgets
{


class CCollapsiblePage: public QWidget
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
	Ui::CCollapsiblePage m_ui;
	QIcon m_icon;
	QWidget* m_pageWidgetPtr;
};

} // namespace iwidgets


#endif // !iwidgets_CCollapsiblePage_included


