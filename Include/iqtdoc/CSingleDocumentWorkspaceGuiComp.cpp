#include <iqtdoc/CSingleDocumentWorkspaceGuiComp.h>


// Qt includes
#include <QtCore/QFileInfo>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>
#else
#include <QtGui/QMessageBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMainWindow>
#include <QtGui/QApplication>
#endif

// ACF includes
#include <istd/CChangeNotifier.h>

#include <idoc/IDocumentTemplate.h>

#include <iqt/CSettingsWriteArchive.h>
#include <iqt/CSettingsReadArchive.h>


namespace iqtdoc
{


// public methods


CSingleDocumentWorkspaceGuiComp::CSingleDocumentWorkspaceGuiComp()
	:m_lastViewPtr(NULL)
{
}


// reimplemented (idoc::IDocumentManager)

void CSingleDocumentWorkspaceGuiComp::SetActiveView(istd::IPolymorphic* /*viewPtr*/)
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

	CloseDocument(-1, false, ignoredPtr);

	if (ignoredPtr != NULL){
		*ignoredPtr = (GetDocumentsCount() > 0);
	}
}


// protected members

void CSingleDocumentWorkspaceGuiComp::UpdateTitle()
{
	QWidget* widgetPtr = GetQtWidget();
	if (widgetPtr != NULL){
		QString titleName = GenerateDocumentTitle();

		widgetPtr->setWindowTitle(titleName);

		QList<QWidget*> widgets = QApplication::allWidgets();
		for (QList<QWidget*>::Iterator widgetIter = widgets.begin(); widgetIter != widgets.end(); ++widgetIter){
			QMainWindow* mainWindowPtr = qobject_cast<QMainWindow*>(*widgetIter);
			if (mainWindowPtr != NULL){
				mainWindowPtr->setWindowTitle(m_applicationName + QString(" - [%1]").arg(titleName));
			}
		}
	}
}


QString CSingleDocumentWorkspaceGuiComp::GenerateDocumentTitle() const
{
	QString titleName = tr("<no name>");

	QString documentFilePath = GetCurrentDocumentFilePath();

	int documentsCount = GetDocumentsCount();
	if (documentsCount > 0){
		if (!documentFilePath.isEmpty()){
			titleName = QFileInfo(documentFilePath).fileName();
		}

		if (HasDocumentPendingChanges()){
			titleName += " *";
		}
	}
	
	return titleName;
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
	Q_ASSERT(viewPtr != NULL);

	static const istd::IChangeable::ChangeSet changeSet(CF_VIEW_ACTIVATION_CHANGED);
	istd::CChangeNotifier notifier(this, &changeSet);
	Q_UNUSED(notifier);

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
	Q_ASSERT(viewPtr != NULL);

	iqtgui::IGuiObject* guiObjectPtr = CompCastPtr<iqtgui::IGuiObject>(viewPtr);
	if (m_lastViewPtr == guiObjectPtr){
		m_lastViewPtr->DestroyGui();

		m_lastViewPtr = NULL;
	}
}


bool CSingleDocumentWorkspaceGuiComp::QueryDocumentSave(bool* ignoredPtr)
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
					GetQtWidget(),
					tr("Close document"),
					tr("Do you want to save your changes made in document\n%1").arg(fileInfo.fileName()),
					buttons,
					QMessageBox::Yes);

		if (response == QMessageBox::Yes){
			return true;
		}
		else if ((ignoredPtr != NULL) && (response == QMessageBox::Cancel)){
			*ignoredPtr = true;
		}
	}

	return false;
}


// reimplemented (iqt:CGuiObjectBase)

void CSingleDocumentWorkspaceGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QWidget* widgetPtr = GetQtWidget();
	Q_ASSERT(widgetPtr != NULL);

	QVBoxLayout* layoutPtr = new QVBoxLayout(widgetPtr);
	layoutPtr->setContentsMargins(0, 0, 0, 0);
	widgetPtr->setLayout(layoutPtr);

	int documentsCount = GetDocumentsCount();
	for (int docIndex = 0; docIndex < documentsCount; ++docIndex){
		int viewsCount = GetViewsCount(docIndex);

		for (int viewIndex = 0; viewIndex < viewsCount; ++viewIndex){
			istd::IPolymorphic* viewPtr = GetViewFromIndex(docIndex, viewIndex);
			Q_ASSERT(viewPtr != NULL);

			OnViewRegistered(viewPtr);
		}
	}
}


void CSingleDocumentWorkspaceGuiComp::OnGuiDestroyed()
{
	CloseDocument();

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
			InsertNewDocument(ids.front());
		}
	}
}


// reimplemented (istd:IChangeable)

void CSingleDocumentWorkspaceGuiComp::OnEndChanges(const ChangeSet& changeSet)
{
	BaseClass::OnEndChanges(changeSet);

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


