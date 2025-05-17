#pragma once


// Qt includes
#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtCore/QTextStream>
#include <QtGui/QIcon>
#include <QtGui/QPixmap>

#if QT_VERSION > 0x050000
#include <QtWidgets/QWidget>
#include <QtWidgets/QStyle>
#else
#include <QtGui/QWidget>
#include <QtGui/QStyle>
#endif

// ACF includes
#include <iqt/iqt.h>


/**
	Standard GUI specific interfaces and components based on Qt.
*/
namespace iqtgui
{


QIcon GetIconFromCache(const QString& iconName);
void AddIconToCache(const QString& iconName, const QIcon& icon);


/**
	Set style sheet to any supported object.
*/
template <class StyleSheetConsumer>
static bool SetStyleSheetFromFile(StyleSheetConsumer* styleSheetConsumer, const QString& styleSheetFilePath)
{
	if (styleSheetConsumer == NULL){
		return false;
	}

	if (styleSheetFilePath.isEmpty()){
		styleSheetConsumer->setStyleSheet("");

		return true;
	}

	QFile styleSheetFile(styleSheetFilePath);
	if (styleSheetFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		QTextStream stream(&styleSheetFile);
		QString styleSheetText;
		QString textLine;
		do{
			textLine = stream.readLine();
			styleSheetText += textLine;
		} while (!textLine.isNull());

		styleSheetConsumer->setStyleSheet(styleSheetText);

		return true;
	}
	else{
		qDebug(qPrintable(QString("Style sheet file could not be opened: '%1'").arg(styleSheetFilePath)));
	}

	return false;
}


} // namespace iqtgui


