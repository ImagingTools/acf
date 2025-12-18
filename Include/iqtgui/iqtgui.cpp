#include <iqtgui/iqtgui.h>


namespace iqtgui
{


QMap<QString, QIcon> g_iconCache;


QIcon GetIconFromCache(const QString& iconName)
{
	if (g_iconCache.contains(iconName)){
		return g_iconCache[iconName];
	}

	return QIcon();
}


void AddIconToCache(const QString & iconName, const QIcon & icon)
{
	g_iconCache[iconName] = icon;
}


} // namespace iqtgui


