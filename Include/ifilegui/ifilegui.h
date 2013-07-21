#ifndef ifilegui_included
#define ifilegui_included


// Qt includes
#include <QtCore/QFile>
#include <QtCore/QTextStream>

// ACF includes
#include "iqt/iqt.h"


/**
	Standard GUI specific interfaces and components based on Qt.
*/
namespace ifilegui
{


/**
	Set style sheet to any supported object.
*/
template <class StyleSheetConsumer>
static bool SetStyleSheet(StyleSheetConsumer& styleSheetConsumer, const QString& styleSheetFilePath)
{
	if (styleSheetFilePath.isEmpty()){
		styleSheetConsumer.setStyleSheet("");

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

		styleSheetConsumer.setStyleSheet(styleSheetText);

		return true;
	}

	return false;
}


} // namespace ifilegui


#endif // !ifilegui_included


