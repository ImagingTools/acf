#include <ifilegui/CSimpleFilePathParamGuiComp.h>


// Qt includes
#include <QtCore/QFileInfo>
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QLineEdit>
#else
#include <QtGui/QFileDialog>
#include <QtGui/QLineEdit>
#endif

// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/CSystem.h>
#include <iqt/CSignalBlocker.h>
#include <ifile/IRelativeFilePath.h>
#include <ifilegui/CFileDialogLoaderComp.h>


namespace ifilegui
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CSimpleFilePathParamGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ifile::IFileNameParam* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		int pathType = objectPtr->GetPathType();

		BrowseButton->setVisible(
				!*m_readOnlyAttrPtr &&
				*m_showBrowseButtonAttrPtr && 
				((pathType == ifile::IFileNameParam::PT_DIRECTORY) || (pathType == ifile::IFileNameParam::PT_FILE)));
	}
}


void CSimpleFilePathParamGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	ifile::IFileNameParam* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	objectPtr->SetPath(GetPathFromEditor());
}


void CSimpleFilePathParamGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	ifile::IFileNameParam* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		QString path = objectPtr->GetPath();
		ifile::IRelativeFilePath* relativeFilePathPtr = dynamic_cast<ifile::IRelativeFilePath*>(objectPtr);
		if (relativeFilePathPtr != NULL){
			QString basePath = relativeFilePathPtr->GetBasePath();

			QDir baseDir(basePath);

			path = baseDir.relativeFilePath(path);
		}

		SetPathToEditor(path);
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CSimpleFilePathParamGuiComp::OnGuiCreated()
{
	DirectoryLabel->setVisible(false);
	PathLabel->setVisible(m_pathLabelAttrPtr.IsValid());
	UrlLabel->setVisible(false);
	BrowseButton->setVisible(*m_showBrowseButtonAttrPtr);

	QString startHint;
	if (m_startHintAttrPtr.IsValid()){
		startHint = *m_startHintAttrPtr;
	}

	PathEdit->setPlaceholderText(startHint);

#if QT_VERSION >= 0x040800
	if (m_filePathValidationExpressionAttrPtr.IsValid()){
		QRegExp validationExpr = QRegExp(*m_filePathValidationExpressionAttrPtr);

		PathEdit->setValidator(new QRegExpValidator(validationExpr, this));
	}
#endif

	if (m_labelWidthAttrPtr.IsValid()){
		int width = *m_labelWidthAttrPtr;
		if (width > 0){
			DirectoryLabel->setFixedWidth(width);
			PathLabel->setFixedWidth(width);
			UrlLabel->setFixedWidth(width);
		}
	}

	PathEdit->setReadOnly(*m_readOnlyAttrPtr);
	BrowseButton->setHidden(*m_readOnlyAttrPtr);

	BaseClass::OnGuiCreated();
}


void CSimpleFilePathParamGuiComp::OnGuiRetranslate()
{
	if (m_pathLabelAttrPtr.IsValid()){
		DirectoryLabel->setText(*m_pathLabelAttrPtr);
		PathLabel->setText(*m_pathLabelAttrPtr);
		UrlLabel->setText(*m_pathLabelAttrPtr);
	}
}


// protected slots

void CSimpleFilePathParamGuiComp::on_BrowseButton_clicked()
{
	ifile::IFileNameParam* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		int pathType = objectPtr->GetPathType();

		if (pathType == ifile::IFileNameParam::PT_DIRECTORY){
			QString pathToSearch = GetPathFromEditor();
			if (!QFile::exists(pathToSearch) && m_defaultDirPtr.IsValid()){
				pathToSearch = m_defaultDirPtr->GetPath();
			}

			QString filePath = QFileDialog::getExistingDirectory(
				GetQtWidget(),
				tr("Select directory"),
				pathToSearch);
			if (!filePath.isEmpty()){
				OnPathEdited(filePath);
			}
		}
		else if (pathType == ifile::IFileNameParam::PT_FILE){
			QStringList filterList;

			if (m_fileTypeInfoPtr.IsValid()){
				QStringList allExt;
				ifilegui::CFileDialogLoaderComp::AppendLoaderFilterList(
							*m_fileTypeInfoPtr,
							NULL,
							0,
							allExt,
							filterList);

				if (allExt.size() > 1){
					filterList += tr("All supported files (%1)").arg("*." + allExt.join(" *."));
				}
			}

			QString pathToSearch = GetPathFromEditor();
			if (!pathToSearch.isEmpty()){
				pathToSearch = QFileInfo(pathToSearch).absoluteFilePath();
			}
			if (!QFile::exists(pathToSearch) && m_defaultDirPtr.IsValid()){
				pathToSearch = m_defaultDirPtr->GetPath();
			}

			// if we want to save: use save dialog
			if (m_useSaveDialogAttrPtr.IsValid()){
				QString filePath = QFileDialog::getSaveFileName(
					GetQtWidget(),
					tr("Select file to save"),
					pathToSearch,
					filterList.join("\n"));

				if (!filePath.isEmpty()){
					OnPathEdited(filePath);
				}
			}
			else{	// we want to load: use open dialog
				QString filePath = QFileDialog::getOpenFileName(
					GetQtWidget(),
					tr("Select file"),
					pathToSearch,
					filterList.join("\n"));

				if (!filePath.isEmpty()){
					OnPathEdited(filePath);
				}
			}
		}
	}
}


void CSimpleFilePathParamGuiComp::on_PathEdit_textChanged(const QString& text)
{
	OnPathEdited(text);
}


// private methods

void CSimpleFilePathParamGuiComp::SetPathToEditor(const QString& path) const
{
	ifile::IFileNameParam* objectPtr = GetObservedObject();
	if (objectPtr == NULL){
		return;
	}

	iqt::CSignalBlocker blocker(PathEdit);

	int cursorPosition = PathEdit->cursorPosition();

	QString normalizedPath;

	int pathType = objectPtr->GetPathType();
	if (pathType == ifile::IFileNameParam::PT_URL){
		normalizedPath = path;
	}
	else{
		PathEdit->clear();

		normalizedPath = QDir::fromNativeSeparators(path);
	}

	QString separator = QDir::fromNativeSeparators("/");

	bool addSeparator = false;
	if (normalizedPath.endsWith(separator)){
		addSeparator = true;
	}

	ifile::IRelativeFilePath* relativeFilePathPtr = dynamic_cast<ifile::IRelativeFilePath*>(objectPtr);
	if (relativeFilePathPtr != NULL){
		QString basePath = relativeFilePathPtr->GetBasePath();

		QDir baseDir(basePath);

		normalizedPath = baseDir.relativeFilePath(normalizedPath);
		if (addSeparator){
			normalizedPath += separator;
		}
	}

	PathEdit->setText(normalizedPath);
	PathEdit->setCursorPosition(PathEdit->isReadOnly() ? normalizedPath.length() : cursorPosition);
}


void CSimpleFilePathParamGuiComp::OnPathEdited(const QString& path)
{
	SetPathToEditor(path);

	DoUpdateModel();
}


QString CSimpleFilePathParamGuiComp::GetPathFromEditor() const
{
	return PathEdit->text();
}


bool CSimpleFilePathParamGuiComp::IsLabelNeeded() const
{
	return (m_pathLabelAttrPtr.IsValid() && !(*m_pathLabelAttrPtr).isEmpty());
}


} // namespace ifilegui


