#pragma once


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <iqtgui/iqtgui.h>


namespace iqtgui
{


class CIconProvider
{
public:
	virtual QIcon GetIcon(const QString& iconName) const;
	virtual QString GetIconPath(const QString& iconName) const;
	virtual QString GetStyleSheetPath(const QString& styleSheetPath) const;

	QByteArray GetCurrentThemeId() const;
	void SetThemeId(const QByteArray& themeId);

protected:
	virtual QIcon CreateIcon(const QString& iconName) const;

private:
	QByteArray m_themeId;
};


}


