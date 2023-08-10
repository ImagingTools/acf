#pragma once


// Qt includes
#include <QtCore/QEvent>


namespace iqt
{


class CDesignThemeEvent: public QEvent
{
public:
	typedef QEvent BaseClass;

	static int s_eventType;

	CDesignThemeEvent(const QByteArray& themeId);

	QByteArray GetThemeId() const;

#if QT_VERSION >= 0x600000
	// reimplemented (QEvent)
	virtual QEvent* clone() const override;
#endif
private:
	QByteArray m_themeId;
};


} // namespace iqt


