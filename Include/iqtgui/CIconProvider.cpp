#include <iqtgui/CIconProvider.h>


// Qt includes
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtGui/QIcon>
#include <QtCore/QCoreApplication>


namespace iqtgui
{


QByteArray CIconProvider::GetCurrentThemeId() const
{
	QByteArray currentThemeId = m_themeId;
	if (currentThemeId.isEmpty()){
		QCoreApplication* applicationPtr = QCoreApplication::instance();
		if (applicationPtr != nullptr){
			currentThemeId = applicationPtr->property("ThemeId").toByteArray();
		}
	}

	return currentThemeId;
}

QIcon CIconProvider::GetIcon(const QString& iconName) const
{
	QString themeIconName = GetIconPath(iconName);

	QIcon icon = GetIconFromCache(themeIconName);
	if (icon.isNull()){
		icon = CreateIcon(themeIconName);
		if (icon.isNull()){
			qDebug(qPrintable(QString("Icon was not found: '%1'").arg(themeIconName)));
		}
		else{
			AddIconToCache(themeIconName, icon);
		}
	}

	return icon;
}

QString CIconProvider::GetIconPath(const QString& iconName) const
{
	QString retVal = iconName;

	QByteArray currentThemeId = GetCurrentThemeId();
	if (!currentThemeId.isEmpty()){
		int index = retVal.indexOf(":/");
		if (index >= 0){
			retVal = retVal.insert(index + 1, "/" + currentThemeId);
		}
		else{
			retVal = currentThemeId + "/" + retVal;
		}
	}

	return retVal;
}

QString CIconProvider::GetStyleSheetPath(const QString& styleSheetPath) const
{
	QString retVal = styleSheetPath;

	QByteArray currentThemeId = GetCurrentThemeId();
	if (!currentThemeId.isEmpty()){
		int index = retVal.indexOf(":/");
		if (index >= 0){
			retVal = retVal.insert(index + 1, "/" + currentThemeId);
		}
		else{
			retVal = currentThemeId + "/" + retVal;
		}
	}

	return retVal;
}

void CIconProvider::SetThemeId(const QByteArray& themeId)
{
	m_themeId = themeId;
}

QIcon CIconProvider::CreateIcon(const QString& themeIconName) const
{
	QString defaultIconName = themeIconName;
	const bool isDefaultIconAvailable = QFileInfo(defaultIconName).exists();

	QIcon icon;

	if (QFileInfo(themeIconName + "_Off_Normal").exists()){
		icon.addFile(themeIconName + "_Off_Normal", QSize(), QIcon::Normal, QIcon::Off);
	}
	else if (isDefaultIconAvailable){
		icon.addFile(defaultIconName, QSize(), QIcon::Normal, QIcon::Off);
	}

	if (QFileInfo(themeIconName + "_On_Normal").exists()){
		icon.addFile(themeIconName + "_On_Normal", QSize(), QIcon::Normal, QIcon::On);
	}
	else if (isDefaultIconAvailable){
		icon.addFile(defaultIconName, QSize(), QIcon::Normal, QIcon::On);
	}

	if (QFileInfo(themeIconName + "_Off_Disabled").exists()){
		icon.addFile(themeIconName + "_Off_Disabled", QSize(), QIcon::Disabled, QIcon::Off);
	}
	else if (isDefaultIconAvailable){
		icon.addFile(defaultIconName, QSize(), QIcon::Disabled, QIcon::Off);
	}

	if (QFileInfo(themeIconName + "_Off_Active").exists()){
		icon.addFile(themeIconName + "_Off_Active", QSize(), QIcon::Active, QIcon::Off);
	}
	else if (isDefaultIconAvailable){
		icon.addFile(defaultIconName, QSize(), QIcon::Active, QIcon::Off);
	}

	if (QFileInfo(themeIconName + "_Off_Selected").exists()){
		icon.addFile(themeIconName + "_Off_Selected", QSize(), QIcon::Selected, QIcon::Off);
	}
	else if (isDefaultIconAvailable){
		icon.addFile(defaultIconName, QSize(), QIcon::Selected, QIcon::Off);
	}

	if (QFileInfo(themeIconName + "_On_Disabled").exists()){
		icon.addFile(themeIconName + "_On_Disabled", QSize(), QIcon::Disabled, QIcon::On);
	}
	else if (isDefaultIconAvailable){
		icon.addFile(defaultIconName, QSize(), QIcon::Disabled, QIcon::On);
	}

	if (QFileInfo(themeIconName + "_On_Active").exists()){
		icon.addFile(themeIconName + "_On_Active", QSize(), QIcon::Active, QIcon::On);
	}
	else if (isDefaultIconAvailable){
		icon.addFile(defaultIconName, QSize(), QIcon::Active, QIcon::On);
	}

	if (QFileInfo(themeIconName + "_On_Selected").exists()){
		icon.addFile(themeIconName + "_On_Selected", QSize(), QIcon::Selected, QIcon::On);
	}
	else if (isDefaultIconAvailable){
		icon.addFile(defaultIconName, QSize(), QIcon::Selected, QIcon::On);
	}

	return icon;
}


}

