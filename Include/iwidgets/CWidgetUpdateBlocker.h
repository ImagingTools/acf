#ifndef iwidgets_CWidgetUpdateBlocker_included
#define iwidgets_CWidgetUpdateBlocker_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif


namespace iwidgets
{


/**
	Help class used to block widget update till it is destructed.
*/
class CWidgetUpdateBlocker
{
public:
	explicit CWidgetUpdateBlocker(QWidget* widgetPtr);
	~CWidgetUpdateBlocker();

private:
	QWidget* m_widgetPtr;
};


// inline methods

inline CWidgetUpdateBlocker::CWidgetUpdateBlocker(QWidget* widgetPtr)
	:m_widgetPtr(widgetPtr)
{
	Q_ASSERT(widgetPtr != NULL);

	m_widgetPtr->setUpdatesEnabled(false);
}


inline CWidgetUpdateBlocker::~CWidgetUpdateBlocker()
{
	m_widgetPtr->setUpdatesEnabled(true);
}


} // namespace iwidgets


#endif // !iwidgets_CWidgetUpdateBlocker_included


