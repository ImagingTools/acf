#include "iqt/CTranslationManagerComp.h"


// Qt includes
#include <QtCore/QLocale>
#include <QtCore/QDir>
#include <QtGui/QIcon>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

// ACF includes
#include "istd/CChangeNotifier.h"

#include "iqt/iqt.h"


namespace iqt
{


// public methods

CTranslationManagerComp::CTranslationManagerComp()
:	m_selectionObserver(*this),
	m_currentLanguageIndex(-1)
{
}


// reimplemented (icomp::CComponentBase)

void CTranslationManagerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	// create and install translations:
	if (m_languagesAttrPtr.IsValid() && m_translationFilePrefixAttrPtr.IsValid()){
		int languagesCount = GetLanguagesCount();

		QString translationsPath = *m_translationFilePathAttrPtr;
		QString translationFilePrefix = *m_translationFilePrefixAttrPtr;

		for (int translatorIndex = 0; translatorIndex < languagesCount; translatorIndex++){
			QString languageId = m_languagesAttrPtr[translatorIndex];

			QString translatorFile = translationsPath + QString("/") + translationFilePrefix + QString("_") + languageId;

			istd::TDelPtr<QTranslator> qtTranslatorPtr(new QTranslator(qApp));

			if (!qtTranslatorPtr->load(translatorFile)){
				SendVerboseMessage(QString("Cannot load translator for: %1").arg(translatorFile), "TranslationManager");
			}

			m_translatorsList.PushBack(qtTranslatorPtr.PopPtr());
		}
	}

	if (m_languageSelectionModelCompPtr.IsValid() && m_languageSelectionCompPtr.IsValid()){
		m_languageSelectionModelCompPtr->AttachObserver(&m_selectionObserver);

		int languageIndex = m_languageSelectionCompPtr->GetSelectedOptionIndex();

		SwitchLanguage(languageIndex);
	}
	else{
		SetSystemLanguage();
	}
}


void CTranslationManagerComp::OnComponentDestroyed()
{
	m_selectionObserver.EnsureModelDetached();

	m_translatorsList.Reset();

	BaseClass::OnComponentDestroyed();
}


// reimplemented (iqt::ITranslationManager)

int CTranslationManagerComp::GetLanguagesCount() const
{
	if (m_languagesAttrPtr.IsValid()){
		return m_languagesAttrPtr.GetCount();
	}

	return 0;
}


int CTranslationManagerComp::GetCurrentLanguageIndex() const
{
	return m_currentLanguageIndex;
}


const QTranslator* CTranslationManagerComp::GetLanguageTranslator(int languageIndex) const
{
	Q_ASSERT(languageIndex >= 0);
	Q_ASSERT(languageIndex < m_translatorsList.GetCount());

	return m_translatorsList.GetAt(languageIndex);
}


const ITranslationManager* CTranslationManagerComp::GetSlaveTranslationManager() const
{
	return m_slaveTranslationManagerCompPtr.GetPtr();
}


void CTranslationManagerComp::SwitchLanguage(int languageIndex)
{
	if (m_currentLanguageIndex == languageIndex){
		return;
	}

	if (m_currentLanguageIndex >= 0){
		QCoreApplication::removeTranslator(m_translatorsList.GetAt(m_currentLanguageIndex));
	}

	if (languageIndex >= 0 && languageIndex < m_translatorsList.GetCount()){
		QCoreApplication::installTranslator(m_translatorsList.GetAt(languageIndex));

		m_currentLanguageIndex = languageIndex;
	}

	if (m_slaveTranslationManagerCompPtr.IsValid()){
		m_slaveTranslationManagerCompPtr->SwitchLanguage(languageIndex);
	}
}


void CTranslationManagerComp::SetSystemLanguage()
{
	QLocale local = QLocale::system();
	QString defaultSystemLanguage = local.name();

	int languagesCount = GetLanguagesCount();

	for (int translatorIndex = 0; translatorIndex < languagesCount; translatorIndex++){
		QString languageId = m_languagesAttrPtr[translatorIndex];
		if (languageId == defaultSystemLanguage){
			SwitchLanguage(translatorIndex);
			break;
		}
	}
}


// reimplemented (iprm::IOptionsList)

int CTranslationManagerComp::GetOptionsFlags() const
{
	return SCF_NONE;
}


int CTranslationManagerComp::GetOptionsCount() const
{
	return GetLanguagesCount();
}


QString CTranslationManagerComp::GetOptionName(int index) const
{
	Q_ASSERT(index < GetOptionsCount());

	if (index >= 0 && m_languagesAttrPtr.IsValid()){
		QString languageId = m_languagesAttrPtr[index];

		QLocale locale(languageId);

		return QLocale::languageToString(locale.language());
	}

	return QString();
}


QString CTranslationManagerComp::GetOptionDescription(int /*index*/) const
{
	return QString();
}


QByteArray CTranslationManagerComp::GetOptionId(int index) const
{
	Q_ASSERT(index < m_languagesAttrPtr.GetCount());

	return m_languagesAttrPtr[index];
}


bool CTranslationManagerComp::IsOptionEnabled(int /*index*/) const
{
	return true;
}


// public methods embedded class LanguageSelectionObserver

CTranslationManagerComp::LanguageSelectionObserver::LanguageSelectionObserver(CTranslationManagerComp& parent)
	:m_parent(parent)
{
}


// protected methods embedded class LanguageSelectionObserver

// reimplemented (imod::CSingleModelObserverBase)

void CTranslationManagerComp::LanguageSelectionObserver::OnUpdate(const istd::IChangeable::ChangeSet& changeSet)
{
	if (changeSet.Contains(iprm::ISelectionParam::CF_SELECTION_CHANGED)){
		iprm::ISelectionParam* objectPtr = GetObjectPtr();
		Q_ASSERT(objectPtr != NULL);
		if (objectPtr != NULL){
			int selectedIndex = objectPtr->GetSelectedOptionIndex();
			if (selectedIndex >= 0){
				m_parent.SwitchLanguage(selectedIndex);
			}
		}
	}
}


} // namespace iqt


