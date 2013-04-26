#include "iqtgui/CFileTreeProviderGuiComp.h"


namespace iqtgui
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CFileTreeProviderGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_fileModelUpdateAllowed = true;

	if (m_currentFileCompPtr.IsValid() && m_currentFileModelCompPtr.IsValid()){
		RegisterModel(m_currentFileModelCompPtr.GetPtr());
	}
}


void CFileTreeProviderGuiComp::OnComponentDestroyed()
{
	UnregisterAllModels();

	BaseClass::OnComponentDestroyed();
}


// reimplemented (CGuiComponentBase)

void CFileTreeProviderGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	ibase::IQtItemModelProvider* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		FileList->setModel(const_cast<QAbstractItemModel*>(objectPtr->GetItemModel()));

		QItemSelectionModel* selectionModelPtr = FileList->selectionModel();
		if (selectionModelPtr != NULL){
			connect(selectionModelPtr,
				SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
				this,
				SLOT(OnSelectionChanged(const QItemSelection&, const QItemSelection&)));
		}
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CFileTreeProviderGuiComp::UpdateGui(int /*updateFlags*/)
{
	Q_ASSERT(IsGuiCreated());

	// update selection
	OnModelChanged(0, 0, NULL);
}


// reimplemented (imod::IModelEditor)

void CFileTreeProviderGuiComp::UpdateModel() const
{
	istd::CChangeNotifier updatePtr(dynamic_cast<istd::IChangeable*>(GetObjectPtr()));
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CFileTreeProviderGuiComp::OnModelChanged(int /*modelId*/, int /*changeFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (!m_fileModelUpdateAllowed){
		return;
	}

	UpdateBlocker updateBlocker(this);

	ibase::IQtItemModelProvider* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const QAbstractItemModel* modelPtr = objectPtr->GetItemModel();
		if (modelPtr != NULL){
			QString currentPath = m_currentFileCompPtr->GetPath();

			QModelIndexList indexes = modelPtr->match(modelPtr->index(0,0), DR_PATH, currentPath, 1, 
				Qt::MatchFixedString | Qt::MatchRecursive | Qt::MatchWrap);
			if (!indexes.isEmpty()){
				QItemSelectionModel* selectionModelPtr = FileList->selectionModel();
				if (selectionModelPtr != NULL){
					selectionModelPtr->setCurrentIndex(indexes.first(), QItemSelectionModel::ClearAndSelect);
				}			
			}
		}
	}
}


// private slots

void CFileTreeProviderGuiComp::OnSelectionChanged(const QItemSelection& selected, const QItemSelection&/* deselected*/)
{
	UpdateBlocker updateBlocker(this);

	if (!selected.indexes().isEmpty() && m_currentFileCompPtr.IsValid()){
		ibase::IQtItemModelProvider* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			const QAbstractItemModel* modelPtr = objectPtr->GetItemModel();
			if (modelPtr != NULL){
				QModelIndex selectedIndex = selected.indexes().at(0);

				QString currentFilePath = modelPtr->data(selectedIndex, DR_PATH).toString();
				QFileInfo fileInfo(currentFilePath);

				bool isFile = fileInfo.isFile();
				bool isDir = fileInfo.isDir();

				int selectedFileType = ifile::IFileNameParam::PT_UNKNOWN;

				if (isFile){
					selectedFileType = ifile::IFileNameParam::PT_FILE;
				}

				if (isDir){
					selectedFileType = ifile::IFileNameParam::PT_DIRECTORY;
				}

				int supportedPathType = m_currentFileCompPtr->GetPathType();
				if ((supportedPathType == ifile::IFileNameParam::PT_UNKNOWN) || (supportedPathType == selectedFileType)){
					m_fileModelUpdateAllowed = false;

					m_currentFileCompPtr->SetPath(currentFilePath);

					m_fileModelUpdateAllowed = true;

					return;
				}
			}
		}
	}

	m_currentFileCompPtr->SetPath("");
}


void CFileTreeProviderGuiComp::on_Refresh_clicked()
{
	// update model
	DoUpdateModel();

	// update selection
	OnModelChanged(0, 0, NULL);
}



} // namespace iqtgui


