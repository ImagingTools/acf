#ifndef iqtgui_CWidgetUpdateBlocker_included
#define iqtgui_CWidgetUpdateBlocker_included



namespace iqtgui
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


} // namespace iqtgui


#endif // !iqtgui_CWidgetUpdateBlocker_included


