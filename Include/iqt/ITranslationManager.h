#ifndef iqt_ITranslationManager_included
#define iqt_ITranslationManager_included


// ACF includes
#include <iprm/IOptionsList.h>


class QTranslator;


namespace iqt
{

/**
	Common interface for the localization manager.
*/
class ITranslationManager: virtual public istd::IPolymorphic
{
public:
	/**
		Get the list of supported languages given as a list of options.
	*/
	virtual const iprm::IOptionsList& GetLanguagesInfo() const = 0;

	/**
		Get current language index.
	*/
	virtual int GetCurrentLanguageIndex() const = 0;

	/**
		Get Qt translator for a given language.
	*/
	virtual const QTranslator* GetLanguageTranslator(int languageIndex) const = 0;

	/**
		Get slave translation manager, if set.
	*/
	virtual const ITranslationManager* GetSlaveTranslationManager() const = 0;

	/**
		Switch the current language to the language with the index \c languageIndex.
	*/
	virtual void SwitchLanguage(int languageIndex) = 0;

	/**
		Set default system's language.
	*/
	virtual void SetSystemLanguage() = 0;
};


} // namespace iqt


#endif // !iqt_ITranslationManager_included


