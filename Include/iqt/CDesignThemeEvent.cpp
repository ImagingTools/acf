#include <iqt/CDesignThemeEvent.h>


namespace iqt
{


// public static methods

int CDesignThemeEvent::s_eventType = QEvent::MaxUser - 1;


// public methods

CDesignThemeEvent::CDesignThemeEvent(const QByteArray& themeId)
	:BaseClass(QEvent::Type(s_eventType)),
	m_themeId(themeId)
{
}


QByteArray CDesignThemeEvent::GetThemeId() const
{
	return m_themeId;
}


// reimplemented (QEvent)

#if QT_VERSION >= 0x600000

QEvent* CDesignThemeEvent::clone() const
{
	return new CDesignThemeEvent;
}
#endif


} // namespace iqt


