#include "iqt/CLocalizer.h"


// Qt includes
#include <QCoreApplication>


namespace iqt
{


// public methods

// reimplemented (istd::ILocalizer)

QString CLocalizer::GetTranslatedText(
			const std::string& context,
			const QString& text,
			const std::string& disambiguation) const
{
	std::string textKey = text.toStdString();

	return QCoreApplication::translate(context.c_str(), textKey.c_str(), disambiguation.c_str());
}


} // namespace iqt


