#ifndef iwidgets_CWidgetUpdateBlocker_included
#define iwidgets_CWidgetUpdateBlocker_included



namespace iwidgets
{


/**
	Help class used to block widget update till it is destructed.
*/
class CWidgetUpdateBlocker
{
public:
	CWidgetUpdateBlocker(QWidget* widgetPtr);
	~CWidgetUpdateBlocker();

private:
	QWidget* m_widgetPtr;
	bool m_updatesEnabled;
};


// inline methods

inline CWidgetUpdateBlocker::CWidgetUpdateBlocker(QWidget* widgetPtr)
	:m_widgetPtr(widgetPtr)
{
	Q_ASSERT(widgetPtr != NULL);

	m_updatesEnabled = m_widgetPtr->updatesEnabled();
	m_widgetPtr->setUpdatesEnabled(false);
}


inline CWidgetUpdateBlocker::~CWidgetUpdateBlocker()
{
	m_widgetPtr->setUpdatesEnabled(m_updatesEnabled);
}


} // namespace iwidgets


#endif // !iwidgets_CWidgetUpdateBlocker_included


