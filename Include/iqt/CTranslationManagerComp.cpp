#include <iqt/CTranslationManagerComp.h>


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
#include <istd/CChangeNotifier.h>
#include <iqt/iqt.h>


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

	LoadTranslations();

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

	m_translatorsList.clear();

	BaseClass::OnComponentDestroyed();
}


// reimplemented (iqt::ITranslationManager)

const iprm::IOptionsList& CTranslationManagerComp::GetLanguagesInfo() const
{
	return *this;
}


int CTranslationManagerComp::GetCurrentLanguageIndex() const
{
	return m_currentLanguageIndex;
}


const QTranslator* CTranslationManagerComp::GetLanguageTranslator(int languageIndex) const
{
	Q_ASSERT(languageIndex >= 0);
	Q_ASSERT(languageIndex < m_translatorsList.count());

	return m_translatorsList[languageIndex].translatorPtr.GetPtr();
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
		QCoreApplication::removeTranslator(m_translatorsList[m_currentLanguageIndex].translatorPtr.GetPtr());
	}

	if (languageIndex >= 0 && languageIndex < m_translatorsList.count()){
		QCoreApplication::installTranslator(m_translatorsList[languageIndex].translatorPtr.GetPtr());

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

	for (int translatorIndex = 0; translatorIndex < m_translatorsList.count(); translatorIndex++){
		QString languageId = m_translatorsList[translatorIndex].languageId;
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
	return m_translatorsList.count();
}


QString CTranslationManagerComp::GetOptionName(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < m_translatorsList.count());

	return m_translatorsList[index].languageName;
}


QString CTranslationManagerComp::GetOptionDescription(int /*index*/) const
{
	return QString();
}


QByteArray CTranslationManagerComp::GetOptionId(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < m_translatorsList.count());

	return m_translatorsList[index].languageId;
}


bool CTranslationManagerComp::IsOptionEnabled(int /*index*/) const
{
	return true;
}


// protected methods

void CTranslationManagerComp::LoadTranslations()
{
	if (m_languagesAttrPtr.IsValid() && m_translationFilePrefixAttrPtr.IsValid()){
		int languagesCount = m_languagesAttrPtr.GetCount();

		QString translationsPath = *m_translationFilePathAttrPtr;
		QString translationFilePrefix = *m_translationFilePrefixAttrPtr;

		for (int translatorIndex = 0; translatorIndex < languagesCount; translatorIndex++){
			QByteArray languageId = m_languagesAttrPtr[translatorIndex];

			if (*m_ignoreLocaleAttrPtr){
				int pos = languageId.indexOf("_");
				if (pos >= 0){
					languageId.chop(languageId.count() - pos);
				}
			}

			QString translatorFile = translationsPath + QString("/") + translationFilePrefix + QString("_") + languageId;

			istd::TDelPtr<QTranslator> qtTranslatorPtr(new QTranslator(qApp));

			if (!qtTranslatorPtr->load(translatorFile)){
				SendVerboseMessage(QString("Cannot load translator for: %1").arg(translatorFile), "TranslationManager");
			}

			Translator translator;

			translator.translatorPtr.SetPtr(qtTranslatorPtr.PopPtr());
			translator.languageId = languageId;

			if (translatorIndex < m_languageNamesAttrPtr.GetCount()){
				translator.languageName = m_languageNamesAttrPtr[translatorIndex];
			}
			else{
				QLocale locale(languageId);

				translator.languageName = QLocale::languageToString(locale.language());
			}

			m_translatorsList.push_back(translator);
		}
	}
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
		iprm::ISelectionParam* objectPtr = GetObservedObject();
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


