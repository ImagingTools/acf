#include "iqt/CFileNameProviderGuiComp.h"


#include <QFileDialog>


namespace iqt
{

// public methods

// reimplemented (istd::IFileNameProvider)

istd::CString CFileNameProviderGuiComp::GetFileName() const
{
	return iqt::GetCString(FileNameEdit->text());
}


// protected methods
	
// reimplemented (CGuiComponentBase)

void CFileNameProviderGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	connect(FileNameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(OnFileNameChanged(const QString&)));
	connect(BrowseButton, SIGNAL(clicked()), this, SLOT(OnBrowse()));
}


// protected slots

void CFileNameProviderGuiComp::OnBrowse()
{
	QString browseTitle = tr("Select File/Directory");

	if (m_browseTitleAttrPtr.IsValid()){
		browseTitle = iqt::GetQString(m_browseTitleAttrPtr->GetValue());
	}

	QString fileName;
	QString currentFileName = FileNameEdit->text();
	if (m_isDirectoryAttrPtr.IsValid()){
		fileName = QFileDialog::getExistingDirectory(GetQtWidget(), browseTitle, currentFileName);
	}
	else{
		QString fileFilter;

		if (m_fileFilterAttrPtr.IsValid()){
			fileFilter = iqt::GetQString(m_fileFilterAttrPtr->GetValue());
		}

		QFileInfo fileInfo(currentFileName);

		fileName = QFileDialog::getOpenFileName(GetQtWidget(), browseTitle, fileInfo.absolutePath());
	}

	if (!fileName.isEmpty()){
		FileNameEdit->setText(fileName);
	}
}


void CFileNameProviderGuiComp::OnFileNameChanged(const QString& fileName)
{
	QFileInfo fileInfo(fileName);

	QPalette palette = FileNameEdit->palette();

	if (fileInfo.exists()){
		palette.setColor(QPalette::Text, Qt::black);
	}
	else{
		palette.setColor(QPalette::Text, Qt::red);
	}

	FileNameEdit->setPalette(palette);
}


} // namespace iqt


