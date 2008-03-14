#ifndef iqt_ITranslationManager_included
#define iqt_ITranslationManager_included


#include "iqt/iqt.h"


#include <QString>

#include "istd/IPolymorphic.h"



namespace iqt
{


class ITranslationManager: virtual public istd::IPolymorphic
{
public:
	/**
		Returns number of languages supported by translation manager.
	*/
	virtual	int GetLanguageCount() const = 0;

	/**
		Returns language ID for the language with the index \index.
	*/
	virtual	QString GetLanguageId(int index) const = 0;

	/**
		Returns language icon for the language with the index \index.
	*/
	virtual	QIcon GetLanguageIcon(int index) const = 0;

	/**
		Returns currently selected language ID.
	*/
	virtual	QString GetSelectedLanguageId() const = 0;

	/**
		Select a language.
	*/
	virtual	void SetSelectedLanguage(const QString& languageId) = 0;
	
	/**
		Sets default language.
	*/
	virtual void SetDefaultLanguage(const QString& laguageId) = 0;
};


} // namespace iqt


#endif // !iqt_ITranslationManager_included
