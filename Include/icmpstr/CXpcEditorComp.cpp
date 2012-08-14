#include "CXpcEditorComp.h"


// ACF includes
#include <QtGui/QStringListModel>
#include "generated/ui_CXpcEditorComp.h"
#include <QtGui/QItemDelegate>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include "iqt/CSystem.h"
//#include <QtGui/QCompleter>

namespace
{


/** 
	A reimplementation of flags() that disables overwriting of non-null 
	items on drag-drop 
 */
class NonOverwritableQStringListModel : public QStringListModel
{
public:


	NonOverwritableQStringListModel(QObject* object) : QStringListModel(object)
	{
	}


	virtual Qt::ItemFlags flags(const QModelIndex & index) const
	{
		int flags = QStringListModel::flags(index);

		return index.data().isNull() ? flags : flags ^ Qt::ItemIsDropEnabled;
	}
};


} // namespace


namespace icmpstr
{


CXpcEditorComp::CXpcEditorComp()
{
	for (int i = 0; i < 3; i++) {
		m_guiModel[i] = new NonOverwritableQStringListModel(this);
		QObject::connect(m_guiModel[i], SIGNAL(layoutChanged()), this, SLOT(OnModelLayoutChanged()));
		QObject::connect(m_guiModel[i], SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(OnModelDataChanged(const QModelIndex&, const QModelIndex&)));
	}
}


QStringList CXpcEditorComp::GetStringList(int tableIdx) const
{
	return m_guiModel[tableIdx]->stringList();
}


void CXpcEditorComp::UpdateModel() const
{
	icomp::CXpcModel* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

	istd::TChangeNotifier<icomp::CXpcModel> changePtr(objectPtr);

	changePtr->SetConfFilesList(GetStringList(SectionConfFiles));
	changePtr->SetPackageDirsList(GetStringList(SectionPackageDirectories));
	changePtr->SetPackagesList(GetStringList(SectionPackages));
}


void CXpcEditorComp::SetStringList(int tableIdx, QStringList list)
{
	m_guiModel[tableIdx]->setStringList(list);
}


void CXpcEditorComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	icomp::CXpcModel* objectPtr = GetObjectPtr();
	if (objectPtr != NULL) {
		SetStringList(SectionConfFiles, objectPtr->GetConfFilesList());
		SetStringList(SectionPackageDirectories, objectPtr->GetPackageDirsList());
		SetStringList(SectionPackages, objectPtr->GetPackagesList());
	}
}


void CXpcEditorComp::OnGuiCreated()
{
	listView_cf->setModel(m_guiModel[SectionConfFiles]);
	listView_pd->setModel(m_guiModel[SectionPackageDirectories]);
	listView_pk->setModel(m_guiModel[SectionPackages]);
	//	QStringList wordList;
	//	wordList << "$(ConfigurationName)" << "$(CompilerName)";
	//	QCompleter *completer = new QCompleter(wordList, this);
	//	completer->setCompletionMode(QCompleter::InlineCompletion);
	//	lineEdit_pk->setCompleter(completer);

	// Call basic functionality:
	BaseClass::OnGuiCreated();
}


void CXpcEditorComp::on_button_add_cf_clicked()
{
	m_guiModel[SectionConfFiles]->insertRow(m_guiModel[SectionConfFiles]->rowCount());
}


void CXpcEditorComp::on_button_add_pd_clicked()
{
	m_guiModel[SectionPackageDirectories]->insertRow(m_guiModel[SectionPackageDirectories]->rowCount());
}


void CXpcEditorComp::on_button_add_pk_clicked()
{
	m_guiModel[SectionPackages]->insertRow(m_guiModel[SectionPackages]->rowCount());
}


void CXpcEditorComp::on_button_remove_cf_clicked()
{
	QModelIndex index = listView_cf->currentIndex();
	if (index.isValid()) {
		m_guiModel[SectionConfFiles]->removeRow(index.row());
		DoUpdateModel();
	}
}


void CXpcEditorComp::on_button_remove_pd_clicked()
{
	QModelIndex index = listView_pd->currentIndex();
	if (index.isValid()) {
		m_guiModel[SectionPackageDirectories]->removeRow(index.row());
		DoUpdateModel();
	}
}


void CXpcEditorComp::on_button_remove_pk_clicked()
{
	QModelIndex index = listView_pk->currentIndex();
	if (index.isValid()) {
		m_guiModel[SectionPackages]->removeRow(index.row());
		DoUpdateModel();
	}
}


QDir CXpcEditorComp::GetCurrentDocumentDir()
{
	QDir documentDir;
	if (m_docMgrPtr.IsValid()) {
		const istd::IPolymorphic* viewPtr = m_docMgrPtr->GetActiveView();
		if (viewPtr) {
			idoc::IDocumentManager::DocumentInfo info;
			m_docMgrPtr->GetDocumentFromView(*viewPtr, &info);
			QFileInfo fileInfo(info.filePath);
			documentDir = fileInfo.dir();
		}
	}
	return documentDir;
}


void CXpcEditorComp::PickFileNameForLineEdit(QLineEdit* lineEdit,
		bool SelectDirectories, iser::IFileTypeInfo* typeInfo)
{
	if (!lineEdit) {
		return;
	}
	QFileDialog dialog;

	if (SelectDirectories) {
		dialog.setFileMode(QFileDialog::DirectoryOnly);
		dialog.setOption(QFileDialog::ShowDirsOnly);
	} else {
		dialog.setFileMode(QFileDialog::ExistingFile);
	}

	if (typeInfo) {
		QStringList extensions;
		typeInfo->GetFileExtensions(extensions);
		QString filter = typeInfo->GetTypeDescription();
		filter += " (";
		if (extensions.size()) {
			for (int i = 0; i < extensions.size(); i++) {
				filter += "*." + extensions[i];
			}
		} else {
			filter += "*.*";
		}
		filter += ")";
		dialog.setNameFilter(filter);
	}

	// try to access IDocumentManager and ask it for the file name
	QDir baseDir = GetCurrentDocumentDir();

	QString unrolledPath = iqt::CSystem::GetEnrolledPath(lineEdit->text());
	QFileInfo fileInfo(baseDir.absoluteFilePath(unrolledPath));
	dialog.setDirectory(fileInfo.absoluteDir());

	int result = dialog.exec();
	if (result == QDialog::Accepted) {
		QStringList filenames = dialog.selectedFiles();
		if (filenames.size()) {
			lineEdit->setText(baseDir.relativeFilePath(filenames[0]));
		}
		DoUpdateModel();
	}
}


void CXpcEditorComp::on_buttonEdit_cf_clicked()
{
	PickFileNameForLineEdit(lineEdit_cf, false, m_confFileTypeInfo.GetPtr());
}


void CXpcEditorComp::on_buttonEdit_pd_clicked()
{
	PickFileNameForLineEdit(lineEdit_pd, true);
}


void CXpcEditorComp::on_buttonEdit_pk_clicked()
{
	PickFileNameForLineEdit(lineEdit_pk, false, m_packageFileTypeInfo.GetPtr());
}


void CXpcEditorComp::OnModelLayoutChanged()
{
	DoUpdateModel();
}


void CXpcEditorComp::OnModelDataChanged(const QModelIndex &, const QModelIndex &)
{
	DoUpdateModel();
}


void CXpcEditorComp::on_listView_cf_activated(const QModelIndex & index)
{
	lineEdit_cf->setText(index.data().toString());
}


void CXpcEditorComp::on_listView_pd_activated(const QModelIndex & index)
{
	lineEdit_pd->setText(index.data().toString());
}


void CXpcEditorComp::on_listView_pk_activated(const QModelIndex & index)
{
	lineEdit_pk->setText(index.data().toString());
}


void CXpcEditorComp::on_listView_cf_doubleClicked(const QModelIndex & index)
{
	QDir baseDir = GetCurrentDocumentDir();
	QString unrolledPath = iqt::CSystem::GetEnrolledPath(index.data().toString());
	QString absolutePath = baseDir.absoluteFilePath(unrolledPath);

	m_docMgrPtr->FileOpen(NULL, &absolutePath);
}


void CXpcEditorComp::on_lineEdit_cf_textChanged(const QString & text)
{
	QModelIndex index = listView_cf->currentIndex();
	if (!index.isValid()) {
		return;
	}

	listView_cf->model()->setData(index, text);
}


void CXpcEditorComp::on_lineEdit_pd_textChanged(const QString & text)
{
	QModelIndex index = listView_pd->currentIndex();
	if (!index.isValid()) {
		return;
	}

	listView_pd->model()->setData(index, text);
}


void CXpcEditorComp::on_lineEdit_pk_textChanged(const QString & text)
{
	QModelIndex index = listView_pk->currentIndex();
	if (!index.isValid()) {
		return;
	}

	listView_pk->model()->setData(index, text);
}


} // namespace icmpstr
