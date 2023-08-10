#pragma once


// Qt includes
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtGui/QIcon>

// ACF includes
#include <iqtgui/TDesignSchemaHandlerWrap.h>


namespace iqtgui
{


template <class Base>
class TMakeIconProviderCompWrap: public TDesignSchemaHandlerWrap<Base>
{
public:
	typedef Base BaseClass;

	virtual QIcon GetIcon(const QString& iconName) const;
	virtual QString GetIconPath(const QString& iconName) const;
	virtual QString GetStyleSheetPath(const QString& styleSheetPath) const;

	QByteArray GetCurrentThemeId() const;

	// reimplemented (ibase::TDesignSchemaHandlerWrap)
	virtual void OnDesignSchemaChanged(const QByteArray& themeId) override;

private:
	QByteArray m_themeId;
};


template <class Base>
QByteArray TMakeIconProviderCompWrap<Base>::GetCurrentThemeId() const
{
	return m_themeId;
}


template <class Base>
QIcon TMakeIconProviderCompWrap<Base>::GetIcon(const QString& iconName) const
{
	QString themeIconName = GetIconPath(iconName);

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

template <class Base>
QString TMakeIconProviderCompWrap<Base>::GetIconPath(const QString& iconName) const
{
	QString retVal = iconName;

	QCoreApplication* applicationPtr = QCoreApplication::instance();
	if (applicationPtr != nullptr){
		QByteArray currentThemeId = QCoreApplication::instance()->property("ThemeId").toByteArray();
		if (!currentThemeId.isEmpty()) {
			QFileInfo fileInfo(iconName);

			QString path = fileInfo.dir().path();
			QString baseIconName = fileInfo.baseName();

			retVal = path + "/" + currentThemeId + "/" + baseIconName;
		}
	}

	return retVal;
}


template <class Base>
QString TMakeIconProviderCompWrap<Base>::GetStyleSheetPath(const QString& styleSheetPath) const
{
	QString retVal = styleSheetPath;

	QCoreApplication* applicationPtr = QCoreApplication::instance();
	if (applicationPtr != nullptr){
		QByteArray currentThemeId = QCoreApplication::instance()->property("ThemeId").toByteArray();
		if (!currentThemeId.isEmpty()) {
			QFileInfo fileInfo(styleSheetPath);

			QString path = fileInfo.dir().path();
			QString baseName = fileInfo.baseName();

			retVal = path + "/" + currentThemeId + "/" + baseName;
		}
	}

	return retVal;
}


// reimplemented (ibase::TDesignSchemaHandlerWrap)

template <class Base>
void TMakeIconProviderCompWrap<Base>::OnDesignSchemaChanged(const QByteArray& themeId)
{
	m_themeId = themeId;
}


} // namespace iqtgui


