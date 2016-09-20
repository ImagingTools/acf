#ifndef iqtgui_included
#define iqtgui_included


// Qt includes
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#if QT_VERSION > 0x040704
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


/**
	Set style sheet to any supported object.
*/
template <class StyleSheetConsumer>
static bool SetStyleSheetFromFile(StyleSheetConsumer& styleSheetConsumer, const QString& styleSheetFilePath)
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


} // namespace iqtgui


#endif // !iqtgui_included


