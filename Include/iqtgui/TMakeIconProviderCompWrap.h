#ifndef iqtgui_TMakeIconProviderCompWrap_included
#define iqtgui_TMakeIconProviderCompWrap_included


// Qt includes
#include <QtCore/QFileInfo>
#include <QtGui/QIcon>


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

	const bool isDefaultIconAvailable = QFileInfo(iconName).exists();

	if (QFileInfo(iconName + "_Off_Normal").exists()){
		icon.addFile(iconName + "_Off_Normal", QSize(), QIcon::Normal, QIcon::Off);
	}
	else if (isDefaultIconAvailable){
		icon.addFile(iconName, QSize(), QIcon::Normal, QIcon::Off);
	}

	if (QFileInfo(iconName + "_On_Normal").exists()){
		icon.addFile(iconName + "_On_Normal", QSize(), QIcon::Normal, QIcon::On);
	}
	else if (isDefaultIconAvailable){
		icon.addFile(iconName, QSize(), QIcon::Normal, QIcon::On);
	}

	if (QFileInfo(iconName + "_Off_Disabled").exists()){
		icon.addFile(iconName + "_Off_Disabled", QSize(), QIcon::Disabled, QIcon::Off);
	}

	if (QFileInfo(iconName + "_Off_Active").exists()){
		icon.addFile(iconName + "_Off_Active", QSize(), QIcon::Active, QIcon::Off);
	}

	if (QFileInfo(iconName + "_Off_Selected").exists()){
		icon.addFile(iconName + "_Off_Selected", QSize(), QIcon::Selected, QIcon::Off);
	}

	if (QFileInfo(iconName + "_On_Disabled").exists()){
		icon.addFile(iconName + "_On_Disabled", QSize(), QIcon::Disabled, QIcon::On);
	}

	if (QFileInfo(iconName + "_On_Active").exists()){
		icon.addFile(iconName + "_On_Active", QSize(), QIcon::Active, QIcon::On);
	}

	if (QFileInfo(iconName + "_On_Selected").exists()){
		icon.addFile(iconName + "_On_Selected", QSize(), QIcon::Selected, QIcon::On);
	}

	return icon;
}


} // namespace iqtgui


#endif // !iqtgui_TMakeIconProviderCompWrap_included


