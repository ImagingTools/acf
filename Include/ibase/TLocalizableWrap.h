#ifndef ibase_TLocalizableWrap_included
#define ibase_TLocalizableWrap_included


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QCoreApplication>


namespace ibase
{


template <class Base>
class TLocalizableWrap: public Base
{
public:
	typedef Base BaseClass;

	TLocalizableWrap();

	/**
		If enabled, listen to the language change event from the application context.
	*/
	virtual void EnableLocalization(bool enableLocalization = true);

protected:
	// abstract methods
	virtual void OnLanguageChanged() = 0;

private:
	class LanguageChangeEventFilter: public QObject
	{
	public:
		typedef QObject BaseClass;

		explicit LanguageChangeEventFilter(TLocalizableWrap& parent);

	protected:
		// reimplemented (QObject)
		virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr) override;

	private:
		TLocalizableWrap& m_parent;
	};

	LanguageChangeEventFilter m_languageChangeEventFilter;
};


// public methods of class TLocalizableWrap

template <class Base>
TLocalizableWrap<Base>::TLocalizableWrap()
	:m_languageChangeEventFilter(*this)
{
}


template <class Base>
void TLocalizableWrap<Base>::EnableLocalization(bool enableLocalization)
{
	QCoreApplication* applicationPtr = QCoreApplication::instance();
	if (applicationPtr != NULL){
		if (enableLocalization){
			applicationPtr->installEventFilter(&m_languageChangeEventFilter);
		}
		else{
			applicationPtr->removeEventFilter(&m_languageChangeEventFilter);
		}
	}
}


// public methods of the embedded class LanguageChangeEventFilter

template <class Base>
TLocalizableWrap<Base>::LanguageChangeEventFilter::LanguageChangeEventFilter(TLocalizableWrap& parent)
	:m_parent(parent)
{
}


// protected methods of the embedded class LanguageChangeEventFilter

// reimplemented (QObject)

template <class Base>
bool TLocalizableWrap<Base>::LanguageChangeEventFilter::eventFilter(QObject* sourcePtr, QEvent* eventPtr)
{
	if ((eventPtr->type() == QEvent::LanguageChange) && (sourcePtr == QCoreApplication::instance())){
		m_parent.OnLanguageChanged();
	}

	return BaseClass::eventFilter(sourcePtr, eventPtr);
}


} // namespace ibase


#endif // ibase_TLocalizableWrap_included

