#include "iqtdoc/CSingleDocumentWorkspaceGuiComp.h"


// Qt includes
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QVBoxLayout>

#include "istd/TChangeNotifier.h"

#include "idoc/IDocumentTemplate.h"

#include "iqtgui/CFileDialogLoaderComp.h"

#include "iser/CXmlFileWriteArchive.h"
#include "iser/CXmlFileReadArchive.h"


namespace iqtdoc
{


CSingleDocumentWorkspaceGuiComp::CSingleDocumentWorkspaceGuiComp()
:	m_lastViewPtr(NULL)
{
}


// reimplemented (iqtgui::IGuiObject)

void CSingleDocumentWorkspaceGuiComp::OnTryClose(bool* ignoredPtr)
{
	FileClose(ignoredPtr);

	if (ignoredPtr != NULL){
		*ignoredPtr = (GetDocumentsCount() > 0);
	}
}


// reimplemented (icomp::IComponent)

void CSingleDocumentWorkspaceGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetDocumentTemplate(m_documentTemplateCompPtr.GetPtr());

	if (m_documentTemplateCompPtr.IsValid()){
		idoc::IDocumentTemplate::Ids ids = m_documentTemplateCompPtr->GetDocumentTypeIds();

		if (!ids.empty()){
			FileNew(ids.front());
		}
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
			titleName = QFileInfo(iqt::GetQString(info.filePath)).fileName();
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


QString CSingleDocumentWorkspaceGuiComp::CreateFileDialogFilter(const std::string* documentTypeIdPtr) const
{
	QString retVal;

	const idoc::IDocumentTemplate* templatePtr = GetDocumentTemplate();
	if (templatePtr != NULL){
		idoc::IDocumentTemplate::Ids docTypeIds = templatePtr->GetDocumentTypeIds();

		QString allExt;
		int filtersCount = 0;

		if (documentTypeIdPtr != NULL){
			iser::IFileLoader* loaderPtr = templatePtr->GetFileLoader(*documentTypeIdPtr);
			if (loaderPtr != NULL){
				filtersCount += iqtgui::CFileDialogLoaderComp::AppendLoaderFilterList(*loaderPtr, allExt, retVal);
			}
		}
		else{
			for (		idoc::IDocumentTemplate::Ids::const_iterator docTypeIter = docTypeIds.begin();
						docTypeIter != docTypeIds.end();
						++docTypeIter){
				iser::IFileLoader* loaderPtr = templatePtr->GetFileLoader(*docTypeIter);
				if (loaderPtr != NULL){
					filtersCount += iqtgui::CFileDialogLoaderComp::AppendLoaderFilterList(*loaderPtr, allExt, retVal);
				}
			}
		}

		if (filtersCount > 1){
			retVal += "\n";
			retVal += tr("All known documents (%1)").arg(allExt);
		}
	}

	return retVal;
}


// reimplemented (idoc::CSingleDocumentManagerBase)

istd::CString CSingleDocumentWorkspaceGuiComp::GetOpenFileName(const std::string* documentTypeIdPtr) const
{
	QString filter = CreateFileDialogFilter(documentTypeIdPtr);

	QStringList files = QFileDialog::getOpenFileNames(NULL, tr("Open Files..."), m_lastDirectory, filter);

	if (!files.isEmpty()){
		UpdateLastDirectory(files.at(0));

		return iqt::GetCString(files.at(0));
	}

	return "";
}


istd::CString CSingleDocumentWorkspaceGuiComp::GetSaveFileName(const std::string& documentTypeId) const
{
	QString filter = CreateFileDialogFilter(&documentTypeId);

	QString filePath = QFileDialog::getSaveFileName(NULL, tr("Save..."), m_lastDirectory, filter);

	UpdateLastDirectory(filePath);

	return iqt::GetCString(filePath);
}


void CSingleDocumentWorkspaceGuiComp::OnViewRegistered(istd::IPolymorphic* viewPtr)
{
	I_ASSERT(viewPtr != NULL);

	istd::CChangeNotifier changePtr(this, ViewActivationChanged);

	iqtgui::IGuiObject* guiObjectPtr = dynamic_cast<iqtgui::IGuiObject*>(viewPtr);
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

		QFileInfo fileInfo(iqt::GetQString(info.filePath));
		QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No;

		if (ignoredPtr != NULL){
			buttons |= QMessageBox::Cancel;
		}

		int response = QMessageBox::information(
					NULL,
					QObject::tr("Close document"),
					QObject::tr("Do you want to save your changes made in document\n%1").arg(fileInfo.fileName()),
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

	OnRetranslate();
}


void CSingleDocumentWorkspaceGuiComp::OnGuiDestroyed()
{
	FileClose();
}


// reimplemented (istd:IChangeable)

void CSingleDocumentWorkspaceGuiComp::OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass2::OnEndChanges(changeFlags, changeParamsPtr);

	if (IsGuiCreated()){
		UpdateTitle();
	}
}


// private methods

void CSingleDocumentWorkspaceGuiComp::UpdateLastDirectory(const QString& filePath) const
{
	QFileInfo fileInfo(filePath);

	m_lastDirectory = fileInfo.absolutePath();
}


} // namespace iqtdoc


