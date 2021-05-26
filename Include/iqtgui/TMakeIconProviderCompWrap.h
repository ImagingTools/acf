#ifndef iqtgui_TMakeIconProviderCompWrap_included
#define iqtgui_TMakeIconProviderCompWrap_included


// Qt includes
#include <QtGui/QIcon>

// ACF includes
#include <icomp/CComponentBase.h>
#include <ifile/IFileNameParam.h>


namespace iqtgui
{


template <class Base>
class TMakeIconProviderCompWrap: public Base
{
public:
	typedef Base BaseClass;

	virtual QIcon GetIcon(const QString& iconName) const;
};


template <class Base>
QIcon TMakeIconProviderCompWrap<Base>::GetIcon(const QString& iconName) const
{
	QIcon icon;
	QPixmap pixmap;

	if (pixmap.load(iconName)){
		icon.addPixmap(pixmap, QIcon::Normal, QIcon::Off);
	}

	if (pixmap.load(iconName + "_Off_Normal")){
		icon.addPixmap(pixmap, QIcon::Normal, QIcon::Off);
	}

	if (pixmap.load(iconName + "_Off_Disabled")){
		icon.addPixmap(pixmap, QIcon::Disabled, QIcon::Off);
	}

	if (pixmap.load(iconName + "_Off_Active")){
		icon.addPixmap(pixmap, QIcon::Active, QIcon::Off);
	}

	if (pixmap.load(iconName + "_Off_Selected")){
		icon.addPixmap(pixmap, QIcon::Selected, QIcon::Off);
	}

	if (pixmap.load(iconName + "_On_Normal")){
		icon.addPixmap(pixmap, QIcon::Normal, QIcon::On);
	}

	if (pixmap.load(iconName + "_On_Disabled")){
		icon.addPixmap(pixmap, QIcon::Disabled, QIcon::On);
	}

	if (pixmap.load(iconName + "_On_Active")){
		icon.addPixmap(pixmap, QIcon::Active, QIcon::On);
	}

	if (pixmap.load(iconName + "_On_Selected")){
		icon.addPixmap(pixmap, QIcon::Selected, QIcon::On);
	}

	return icon;
}


} // namespace iqtgui


#endif // !iqtgui_TMakeIconProviderCompWrap_included


