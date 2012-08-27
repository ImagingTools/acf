#include "CXpcEditorComp.h"


// Qt includes
#include <QtGui/QStringListModel>
#include <QtGui/QItemDelegate>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

// ACF includes
#include "iqt/CSystem.h"


namespace
{


/**
	A reimplementation of flags() that disables overwriting of non-null 
	items on drag-drop 
*/
class NonOverwritableQStringListModel: public QStringListModel
{
public:
	NonOverwritableQStringListModel(QObject* object): QStringListModel(object)
	{
	}

	virtual Qt::ItemFlags flags(const QModelIndex& index) const
	{
		Qt::ItemFlags flags = QStringListModel::flags(index);

		return index.data().isNull()? flags: flags & ~Qt::ItemIsDropEnabled;
	}
};


} // namespace


namespace icmpstr
{


CXpcEditorComp::CXpcEditorComp()
{
	for (int sectionIndex = S_FIRST; sectionIndex <= S_LAST; ++sectionIndex){
		m_guiModel[sectionIndex] = new NonOverwritableQStringListModel(this);
		QObject::connect(m_guiModel[sectionIndex], SIGNAL(layoutChanged()), this, SLOT(OnDoUpdateModel()));
		QObject::connect(m_guiModel[sectionIndex], SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(OnModelDataChanged(const QModelIndex&, const QModelIndex&)));

		SetupVariablesMenu(true, sectionIndex, m_startVariableMenus[sectionIndex]);
		QObject::connect(&m_startVariableMenus[sectionIndex], SIGNAL(triggered(QAction*)), this, SLOT(OnInsertVariable(QAction*)));

		SetupVariablesMenu(false, sectionIndex, m_variableMenus[sectionIndex]);
		QObject::connect(&m_variableMenus[sectionIndex], SIGNAL(triggered(QAction*)), this, SLOT(OnInsertVariable(QAction*)));
	}

	QObject::connect(this, SIGNAL(DoLateUpdateModel()), this, SLOT(OnDoUpdateModel()), Qt::QueuedConnection);
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

	changePtr->SetConfFilesList(GetStringList(S_CONFIG_PATH));
	changePtr->SetPackageDirsList(GetStringList(S_PACKAGE_DIR));
	changePtr->SetPackagesList(GetStringList(S_PACKAGE_PATH));
	changePtr->SetRegistryFilesList(GetStringList(S_REGISTRY_PATH));
}


void CXpcEditorComp::SetStringList(int tableIdx, QStringList list)
{
	m_guiModel[tableIdx]->setStringList(list);
}


void CXpcEditorComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	icomp::CXpcModel* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		SetStringList(S_CONFIG_PATH, objectPtr->GetConfFilesList());
		SetStringList(S_PACKAGE_DIR, objectPtr->GetPackageDirsList());
		SetStringList(S_PACKAGE_PATH, objectPtr->GetPackagesList());
		SetStringList(S_REGISTRY_PATH, objectPtr->GetRegistryFilesList());
	}
}


void CXpcEditorComp::OnGuiCreated()
{
	ConfigPathView->setModel(m_guiModel[S_CONFIG_PATH]);
	PackageDirView->setModel(m_guiModel[S_PACKAGE_DIR]);
	PackagePathView->setModel(m_guiModel[S_PACKAGE_PATH]);
	RegistryPathView->setModel(m_guiModel[S_REGISTRY_PATH]);

	QObject::connect(ConfigPathView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(OnConfigPathViewSelected(const QItemSelection&, const QItemSelection&)));
	QObject::connect(PackageDirView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(OnPackageDirViewSelected(const QItemSelection&, const QItemSelection&)));
	QObject::connect(PackagePathView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(OnPackagePathViewSelected(const QItemSelection&, const QItemSelection&)));
	QObject::connect(RegistryPathView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(OnRegistryPathViewSelected(const QItemSelection&, const QItemSelection&)));

	// Call basic functionality:
	BaseClass::OnGuiCreated();
}


void CXpcEditorComp::on_ConfigPathAddButton_clicked()
{
	m_guiModel[S_CONFIG_PATH]->insertRow(m_guiModel[S_CONFIG_PATH]->rowCount());
}


void CXpcEditorComp::on_PackageDirAddButton_clicked()
{
	m_guiModel[S_PACKAGE_DIR]->insertRow(m_guiModel[S_PACKAGE_DIR]->rowCount());
}


void CXpcEditorComp::on_PackagePathAddButton_clicked()
{
	m_guiModel[S_PACKAGE_PATH]->insertRow(m_guiModel[S_PACKAGE_PATH]->rowCount());
}

void CXpcEditorComp::on_RegistryPathAddButton_clicked()
{
	m_guiModel[S_REGISTRY_PATH]->insertRow(m_guiModel[S_REGISTRY_PATH]->rowCount());
}


void CXpcEditorComp::on_ConfigPathRemoveButton_clicked()
{
	QModelIndex index = ConfigPathView->currentIndex();
	if (index.isValid()){
		m_guiModel[S_CONFIG_PATH]->removeRow(index.row());
		DoUpdateModel();
	}
}


void CXpcEditorComp::on_PackageDirRemoveButton_clicked()
{
	QModelIndex index = PackageDirView->currentIndex();
	if (index.isValid()){
		m_guiModel[S_PACKAGE_DIR]->removeRow(index.row());
		DoUpdateModel();
	}
}


void CXpcEditorComp::on_PackagePathRemoveButton_clicked()
{
	QModelIndex index = PackagePathView->currentIndex();
	if (index.isValid()){
		m_guiModel[S_PACKAGE_PATH]->removeRow(index.row());
		DoUpdateModel();
	}
}

void CXpcEditorComp::on_RegistryPathRemoveButton_clicked()
{
	QModelIndex index = RegistryPathView->currentIndex();
	if (index.isValid()){
		m_guiModel[S_REGISTRY_PATH]->removeRow(index.row());
		DoUpdateModel();
	}
}


QDir CXpcEditorComp::GetCurrentDocumentDir()
{
	QDir documentDir;
	if (m_docMgrPtr.IsValid()){
		const istd::IPolymorphic* viewPtr = m_docMgrPtr->GetActiveView();
		if (viewPtr){
			idoc::IDocumentManager::DocumentInfo info;
			m_docMgrPtr->GetDocumentFromView(*viewPtr, &info);
			QFileInfo fileInfo(info.filePath);
			documentDir = fileInfo.dir();
		}
	}
	return documentDir;
}


void CXpcEditorComp::PickFileNameForLineEdit(
			QLineEdit& lineEdit,
			bool SelectDirectories,
			iser::IFileTypeInfo* typeInfo)
{
	QFileDialog dialog;
	if (SelectDirectories){
		dialog.setFileMode(QFileDialog::DirectoryOnly);
		dialog.setOption(QFileDialog::ShowDirsOnly);
	}
	else{
		dialog.setFileMode(QFileDialog::ExistingFile);
	}

	// configure file filter
	if (typeInfo){
		QStringList extensions;
		typeInfo->GetFileExtensions(extensions);
		QString filter = typeInfo->GetTypeDescription();
		filter += " (";
		if (extensions.size()){
			for (int i = 0; i < extensions.size(); i++){
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

	QString unrolledPath = iqt::CSystem::GetEnrolledPath(lineEdit.text());
	QFileInfo fileInfo(baseDir.absoluteFilePath(unrolledPath));
	dialog.setDirectory(fileInfo.absoluteDir());

	if (dialog.exec() == QDialog::Accepted){
		QStringList filenames = dialog.selectedFiles();
		if (filenames.size()){
			lineEdit.setText(baseDir.relativeFilePath(filenames[0]));
		}
		DoUpdateModel();
	}
}


void CXpcEditorComp::on_ConfigPathBrowseButton_clicked()
{
	PickFileNameForLineEdit(*ConfigPathEdit, false, m_configFileTypeInfo.GetPtr());

	on_ConfigPathEdit_editingFinished();
}


void CXpcEditorComp::on_PackageDirBrowseButton_clicked()
{
	PickFileNameForLineEdit(*PackageDirEdit, true);

	on_PackageDirEdit_editingFinished();
}


void CXpcEditorComp::on_PackagePathBrowseButton_clicked()
{
	PickFileNameForLineEdit(*PackagePathEdit, true, m_packageFileTypeInfo.GetPtr());

	on_PackagePathEdit_editingFinished();
}

void CXpcEditorComp::on_RegistryPathBrowseButton_clicked()
{
	PickFileNameForLineEdit(*RegistryPathEdit, false, m_registryFileTypeInfo.GetPtr());

	on_RegistryPathEdit_editingFinished();
}


void CXpcEditorComp::OnDoUpdateModel()
{
	DoUpdateModel();
}


void CXpcEditorComp::OnModelDataChanged(const QModelIndex& /*topLeft*/, const QModelIndex& /*bottomRight*/)
{
	Q_EMIT DoLateUpdateModel();
}


void CXpcEditorComp::OnConfigPathViewSelected(const QItemSelection& selected, const QItemSelection& /*deselected*/)
{
	QModelIndexList selectedIndices = selected.indexes();
	if (!selectedIndices.isEmpty()){
		ConfigPathEdit->setText(selectedIndices.front().data().toString());
		ConfigPathEdit->setProperty("selectionLength", 0);
		ConfigPathEdit->deselect();

		ConfigPathEditFrame->setEnabled(true);
	}
	else{
		ConfigPathEditFrame->setEnabled(false);
	}
}


void CXpcEditorComp::OnPackageDirViewSelected(const QItemSelection& selected, const QItemSelection& /*deselected*/)
{
	QModelIndexList selectedIndices = selected.indexes();
	if (!selectedIndices.isEmpty()){
		PackageDirEdit->setText(selectedIndices.front().data().toString());
		PackageDirEdit->setProperty("selectionLength", 0);
		PackageDirEdit->deselect();

		PackageDirEditFrame->setEnabled(true);
	}
	else{
		PackageDirEditFrame->setEnabled(false);
	}
}


void CXpcEditorComp::OnPackagePathViewSelected(const QItemSelection& selected, const QItemSelection& /*deselected*/)
{
	QModelIndexList selectedIndices = selected.indexes();
	if (!selectedIndices.isEmpty()){
		PackagePathEdit->setText(selectedIndices.front().data().toString());
		PackagePathEdit->setProperty("selectionLength", 0);
		PackagePathEdit->deselect();

		PackagePathEditFrame->setEnabled(true);
	}
	else{
		PackagePathEditFrame->setEnabled(false);
	}
}

void CXpcEditorComp::OnRegistryPathViewSelected(const QItemSelection& selected, const QItemSelection& /*deselected*/)
{
	QModelIndexList selectedIndices = selected.indexes();
	if (!selectedIndices.isEmpty()){
		RegistryPathEdit->setText(selectedIndices.front().data().toString());
		RegistryPathEdit->setProperty("selectionLength", 0);
		RegistryPathEdit->deselect();

		RegistryPathEditFrame->setEnabled(true);
	}
	else{
		RegistryPathEditFrame->setEnabled(false);
	}
}


void CXpcEditorComp::on_ConfigPathView_doubleClicked(const QModelIndex& index)
{
	QDir baseDir = GetCurrentDocumentDir();
	QString unrolledPath = iqt::CSystem::GetEnrolledPath(index.data().toString());
	QString absolutePath = baseDir.absoluteFilePath(unrolledPath);

	m_docMgrPtr->FileOpen(NULL, &absolutePath);
}


void CXpcEditorComp::on_RegistryPathView_doubleClicked(const QModelIndex& index)
{
	QDir baseDir = GetCurrentDocumentDir();
	QString unrolledPath = iqt::CSystem::GetEnrolledPath(index.data().toString());
	QString absolutePath = baseDir.absoluteFilePath(unrolledPath);

	m_docMgrPtr->FileOpen(NULL, &absolutePath);
}


void CXpcEditorComp::on_ConfigPathEdit_editingFinished()
{
	QModelIndex index = ConfigPathView->currentIndex();
	if (!index.isValid()){
		return;
	}

	ConfigPathView->model()->setData(index, ConfigPathEdit->text());
}


void CXpcEditorComp::on_PackageDirEdit_editingFinished()
{
	QModelIndex index = PackageDirView->currentIndex();
	if (!index.isValid()){
		return;
	}

	PackageDirView->model()->setData(index, PackageDirEdit->text());
}


void CXpcEditorComp::on_PackagePathEdit_editingFinished()
{
	QModelIndex index = PackagePathView->currentIndex();
	if (!index.isValid()){
		return;
	}

	PackagePathView->model()->setData(index, PackagePathEdit->text());
}


void CXpcEditorComp::on_RegistryPathEdit_editingFinished()
{
	QModelIndex index = RegistryPathView->currentIndex();
	if (!index.isValid()){
		return;
	}

	RegistryPathView->model()->setData(index, RegistryPathEdit->text());
}


void CXpcEditorComp::SetupVariablesMenu(bool isBegin, int sectionIndex, QMenu& menu)
{
	menu.clear();

	bool allowFilePath = (sectionIndex == S_CONFIG_PATH) || (sectionIndex == S_PACKAGE_PATH);

	if (isBegin){
		QMap<QString, QString> envVariables = iqt::CSystem::GetEnvironmentVariables();
		for (		QMap<QString, QString>::ConstIterator i = envVariables.constBegin();
					i != envVariables.constEnd();
					i++){
			QString value = i.value();

			QFileInfo dirInfo(value);
			if (dirInfo.isAbsolute() && (dirInfo.isDir() || (allowFilePath && dirInfo.isFile()))){
				QAction* newAction = menu.addAction(i.key());
				newAction->setData(sectionIndex);
				newAction->setStatusTip(value);
			}
		}
	}
	else{
		// special variables are considered relative
		static QString specialVariables[] = {"ConfigName", "CompilerName"};
		for (int i = 0; i < 2; i++){
			QAction* newAction = menu.addAction(specialVariables[i]);
			newAction->setData(sectionIndex);
			newAction->setStatusTip(iqt::CSystem::FindVariableValue(specialVariables[i]));
		}
	}
}


void CXpcEditorComp::on_ConfigPathEdit_selectionChanged()
{
	MaintainLineEditSelection(ConfigPathEdit);

	if (ConfigPathEdit->selectionStart() > 0){
		ConfigPathInsertVariableButton->setMenu(&m_variableMenus[S_CONFIG_PATH]);
	}
	else{
		ConfigPathInsertVariableButton->setMenu(&m_startVariableMenus[S_CONFIG_PATH]);
	}
}


void CXpcEditorComp::on_PackageDirEdit_selectionChanged()
{
	MaintainLineEditSelection(PackageDirEdit);

	if (PackageDirEdit->selectionStart() > 0){
		PackageDirInsertVariableButton->setMenu(&m_variableMenus[S_PACKAGE_DIR]);
	}
	else{
		PackageDirInsertVariableButton->setMenu(&m_startVariableMenus[S_PACKAGE_DIR]);
	}
}


void CXpcEditorComp::on_PackagePathEdit_selectionChanged()
{
	MaintainLineEditSelection(PackagePathEdit);

	if (PackagePathEdit->selectionStart() > 0){
		PackagePathInsertVariableButton->setMenu(&m_variableMenus[S_PACKAGE_PATH]);
	}
	else{
		PackagePathInsertVariableButton->setMenu(&m_startVariableMenus[S_PACKAGE_PATH]);
	}
}

void CXpcEditorComp::on_RegistryPathEdit_selectionChanged()
{
	MaintainLineEditSelection(RegistryPathEdit);

	if (RegistryPathEdit->selectionStart() > 0){
		RegistryPathInsertVariableButton->setMenu(&m_variableMenus[S_REGISTRY_PATH]);
	}
	else{
		RegistryPathInsertVariableButton->setMenu(&m_startVariableMenus[S_REGISTRY_PATH]);
	}
}


void CXpcEditorComp::on_ConfigPathEdit_cursorPositionChanged(int /*oldPosition*/, int newPosition)
{
	MaintainLineEditSelection(PackagePathEdit);

	if (newPosition > 0){
		ConfigPathInsertVariableButton->setMenu(&m_variableMenus[S_CONFIG_PATH]);
	}
	else{
		ConfigPathInsertVariableButton->setMenu(&m_startVariableMenus[S_CONFIG_PATH]);
	}
}


void CXpcEditorComp::on_PackageDirEdit_cursorPositionChanged(int /*oldPosition*/, int newPosition)
{
	MaintainLineEditSelection(PackagePathEdit);

	if (newPosition > 0){
		PackageDirInsertVariableButton->setMenu(&m_variableMenus[S_PACKAGE_DIR]);
	}
	else{
		PackageDirInsertVariableButton->setMenu(&m_startVariableMenus[S_PACKAGE_DIR]);
	}
}


void CXpcEditorComp::on_PackagePathEdit_cursorPositionChanged(int /*oldPosition*/, int newPosition)
{
	MaintainLineEditSelection(PackagePathEdit);

	if (newPosition > 0){
		PackagePathInsertVariableButton->setMenu(&m_variableMenus[S_PACKAGE_PATH]);
	}
	else{
		PackagePathInsertVariableButton->setMenu(&m_startVariableMenus[S_PACKAGE_PATH]);
	}
}

void CXpcEditorComp::on_RegistryPathEdit_cursorPositionChanged(int /*oldPosition*/, int newPosition)
{
	MaintainLineEditSelection(RegistryPathEdit);

	if (newPosition > 0){
		RegistryPathInsertVariableButton->setMenu(&m_variableMenus[S_REGISTRY_PATH]);
	}
	else{
		RegistryPathInsertVariableButton->setMenu(&m_startVariableMenus[S_REGISTRY_PATH]);
	}
}


void CXpcEditorComp::OnInsertVariable(QAction* action)
{
	QLineEdit* lineEditPtr = NULL;

	int sectionIndex = action->data().toInt();
	switch (sectionIndex){
	case S_CONFIG_PATH:
		lineEditPtr = ConfigPathEdit;
		break;

	case S_PACKAGE_DIR:
		lineEditPtr = PackageDirEdit;
		break;

	case S_PACKAGE_PATH:
		lineEditPtr = PackagePathEdit;
		break;

	case S_REGISTRY_PATH:
		lineEditPtr = RegistryPathEdit;
		break;

	default:
		return;
	}
	Q_ASSERT(lineEditPtr != NULL);

	int insertPoint = lineEditPtr->selectedText().size()?
				lineEditPtr->selectionStart():
				lineEditPtr->cursorPosition();

	QString editText = lineEditPtr->text();
	QString variableToInsert = "$(" + action->text() + ")";
	editText.insert(insertPoint, variableToInsert);
	lineEditPtr->setText(editText);

	// update selection to match the length of variable name
	lineEditPtr->setProperty("selectionLength", variableToInsert.size());
	lineEditPtr->setSelection(lineEditPtr->selectionStart(), variableToInsert.size());

	switch (sectionIndex){
	case S_CONFIG_PATH:
		on_ConfigPathEdit_editingFinished();
		break;

	case S_PACKAGE_DIR:
		on_PackageDirEdit_editingFinished();
		break;

	case S_PACKAGE_PATH:
		on_PackagePathEdit_editingFinished();
		break;

	case S_REGISTRY_PATH:
		on_RegistryPathEdit_editingFinished();
		break;

	default:
		break;
	}
}


// static methods

void CXpcEditorComp::MaintainLineEditSelection(QLineEdit* lineEdit)
{
	if (lineEdit->hasFocus()){
		lineEdit->setProperty("selectionStart", lineEdit->selectionStart());
		lineEdit->setProperty("selectionLength", lineEdit->selectedText().size());
	}
	else{
		int selectionStart = lineEdit->property("selectionStart").toInt();
		int selectionLength = lineEdit->property("selectionLength").toInt();

		if (selectionStart + selectionLength <= lineEdit->text().size()){
			lineEdit->setSelection(selectionStart, selectionLength);
		}
	}
}


} // namespace icmpstr


