#include "iqtdoc/CSingleDocumentWorkspaceGuiComp.h"


// Qt includes
#include <QtGui/QMessageBox>
#include <QtCore/QFileInfo>
#include <QtGui/QVBoxLayout>

// ACF includes
#include "istd/TChangeNotifier.h"

#include "idoc/IDocumentTemplate.h"

#include "iqt/CSettingsWriteArchive.h"
#include "iqt/CSettingsReadArchive.h"


namespace iqtdoc
{


// public methods


CSingleDocumentWorkspaceGuiComp::CSingleDocumentWorkspaceGuiComp()
	:m_lastViewPtr(NULL)
{
}


// reimplemented (iqtgui::IGuiObject)

void CSingleDocumentWorkspaceGuiComp::OnTryClose(bool* ignoredPtr)
{
	// Save open document information before exit:
	if (m_rememberOpenDocumentParamPtr.IsValid() && m_rememberOpenDocumentParamPtr->IsEnabled()){
		if (!m_organizationName.isEmpty() && !m_applicationName.isEmpty()){
			iqt::CSettingsWriteArchive archive(
							m_organizationName,
							m_applicationName,
							"OpenDocument",
							QSettings::UserScope);

			SerializeOpenDocument(archive);
		}
	}

	FileClose(-1, ignoredPtr);

	if (ignoredPtr != NULL){
		*ignoredPtr = (GetDocumentsCount() > 0);
	}
}


// protected members

void CSingleDocumentWorkspaceGuiComp::UpdateTitle()
{
	QString titleName = tr("<no name>");
/*
	int documentsCount = GetDocumentsCount();
	if (documentsCount > 0){
		const SingleDocumentData& info = GetSingleDocumentData(0);

		if (!info.filePath.IsEmpty()){
			titleName = QFileInfo(info.filePath)).fileName();
		}

		if (info.isDirty){
			titleName += " *";
		}
	}

	QWidget* widgetPtr = GetQtWidget();
	if (widgetPtr != NULL){
		widgetPtr->setWindowTitle(titleName);
	}
*/
	// TODO: implement setting of document title for CSingleDocumentWorkspaceGuiComp
}


// reimplemented (idoc::CSingleDocumentManagerBase)

QString CSingleDocumentWorkspaceGuiComp::GetOpenFilePath(const QByteArray* documentTypeIdPtr) const
{
	QStringList files = GetOpenFilePathesFromDialog(documentTypeIdPtr);

	if (!files.isEmpty()){
		UpdateLastDirectory(files.at(0));

		return files.at(0);
	}

	return QString();
}


void CSingleDocumentWorkspaceGuiComp::OnViewRegistered(istd::IPolymorphic* viewPtr)
{
	I_ASSERT(viewPtr != NULL);

	istd::CChangeNotifier changePtr(this, CF_VIEW_ACTIVATION_CHANGED);

	iqtgui::IGuiObject* guiObjectPtr = CompCastPtr<iqtgui::IGuiObject>(viewPtr);
	QWidget* widgetPtr = GetQtWidget();
	if ((guiObjectPtr != NULL) && (widgetPtr != NULL)){
		if (m_lastViewPtr != NULL){
			m_lastViewPtr->DestroyGui();
		}

		guiObjectPtr->CreateGui(widgetPtr);

		m_lastViewPtr = guiObjectPtr;
	}
}


void CSingleDocumentWorkspaceGuiComp::OnViewRemoved(istd::IPolymorphic* viewPtr)
{
	I_ASSERT(viewPtr != NULL);

	if (m_lastViewPtr == viewPtr){
		m_lastViewPtr->DestroyGui();

		m_lastViewPtr = NULL;
	}
}


void CSingleDocumentWorkspaceGuiComp::QueryDocumentClose(bool* ignoredPtr)
{
	if (ignoredPtr != NULL){
		*ignoredPtr = false;
	}

	if (GetDocumentsCount() > 0){
		DocumentInfo info;

		GetDocumentFromIndex(0, &info);

		QFileInfo fileInfo(info.filePath);
		QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No;

		if (ignoredPtr != NULL){
			buttons |= QMessageBox::Cancel;
		}

		int response = QMessageBox::information(
					NULL,
					tr("Close document"),
					tr("Do you want to save your changes made in document\n%1").arg(fileInfo.fileName()),
					buttons,
					QMessageBox::Yes);

		if (response == QMessageBox::Yes){
			bool wasSaved = FileSave();

			if (ignoredPtr != NULL){
				*ignoredPtr = !wasSaved;
			}
		}
		else if ((ignoredPtr != NULL) && (response == QMessageBox::Cancel)){
			*ignoredPtr = true;
		}
	}
}


// reimplemented (iqt:CGuiObjectBase)

void CSingleDocumentWorkspaceGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QWidget* widgetPtr = GetQtWidget();
	I_ASSERT(widgetPtr != NULL);

	QVBoxLayout* layoutPtr = new QVBoxLayout(widgetPtr);
	layoutPtr->setContentsMargins(0, 0, 0, 0);
	widgetPtr->setLayout(layoutPtr);

	int documentsCount = GetDocumentsCount();
	for (int docIndex = 0; docIndex < documentsCount; ++docIndex){
		int viewsCount = GetViewsCount(docIndex);

		for (int viewIndex = 0; viewIndex < viewsCount; ++viewIndex){
			istd::IPolymorphic* viewPtr = GetViewFromIndex(docIndex, viewIndex);
			I_ASSERT(viewPtr != NULL);

			OnViewRegistered(viewPtr);
		}
	}
}


void CSingleDocumentWorkspaceGuiComp::OnGuiDestroyed()
{
	FileClose();

	BaseClass::OnGuiDestroyed();
}


// reimplemented (icomp::CComponentBase)

void CSingleDocumentWorkspaceGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetDocumentTemplate(m_documentTemplateCompPtr.GetPtr());

	if (m_documentTemplateCompPtr.IsValid()){
		idoc::IDocumentTemplate::Ids ids = m_documentTemplateCompPtr->GetDocumentTypeIds();
		if (!ids.isEmpty()){
			FileNew(ids.front());
		}
	}
}


// reimplemented (istd:IChangeable)

void CSingleDocumentWorkspaceGuiComp::OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass::OnEndChanges(changeFlags, changeParamsPtr);

	if (IsGuiCreated()){
		UpdateTitle();
	}
}


// reimplemented (TRestorableGuiWrap)

void CSingleDocumentWorkspaceGuiComp::OnRestoreSettings(const QSettings& settings)
{
	BaseClass::OnRestoreSettings(settings);

	Q_ASSERT(IsGuiCreated());

	m_organizationName = settings.organizationName();
	m_applicationName = settings.applicationName();

	if (m_rememberOpenDocumentParamPtr.IsValid() && m_rememberOpenDocumentParamPtr->IsEnabled()){
		iqt::CSettingsReadArchive archive(
					m_organizationName,
					m_applicationName,
					"OpenDocument");

		SerializeOpenDocument(archive);
	}
}


} // namespace iqtdoc


