// Qt includes
#include <QHeaderView>
#include <QToolButton>


// ACF includes
#include "iqtgui/CFileSystemExplorerGuiComp.h"


namespace iqtgui
{


// public methods

// protected methods

// reimplemented (CGuiComponentBase)

void CFileSystemExplorerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	FileTree->setModel(&m_fileSystemModel);
	m_fileSystemModel.setRootPath(QDir::currentPath());
	FileTree->setRootIndex(m_fileSystemModel.setRootPath(m_fileSystemModel.myComputer().toString()));

	QStringList fileFilters;
	if (m_fileFilterAttrPtr.IsValid()){
		fileFilters << iqt::GetQString(*m_fileFilterAttrPtr);
	}

	m_fileSystemModel.setNameFilters(fileFilters);

	QLayout* filterLayoutPtr = FilterFrame->layout();
	if (filterLayoutPtr != NULL){
		m_filterEdit = new iqtgui::CExtLineEdit(tr("Enter text to filter file items"), 2, FilterFrame);

		// add "clear" button:
		QToolButton* clearButton = new QToolButton(m_filterEdit);
		clearButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
		clearButton->setIcon(QIcon(":/Icons/Clear"));
		connect(clearButton, SIGNAL(clicked()), m_filterEdit, SLOT(clear()));

		m_filterEdit->AddWidget(clearButton, Qt::AlignRight);

		filterLayoutPtr->addWidget(m_filterEdit);

		connect(m_filterEdit, SIGNAL(editingFinished()), this, SLOT(OnFilterChanged()));
	}

	FileTree->setDragDropMode(QTreeView::DragDrop);
	FileTree->setDragEnabled(true);

	QHeaderView* headerPtr = FileTree->header();
	if (headerPtr != NULL){
		if (m_showFileTypeAttrPtr.IsValid() && *m_showFileTypeAttrPtr){
			headerPtr->setSectionHidden(2, false);
		}
		else{
			headerPtr->setSectionHidden(2, true);
		}

		headerPtr->setSectionHidden(1, true);

		if (m_showFileModificationTimeAttrPtr.IsValid() && *m_showFileModificationTimeAttrPtr){
			headerPtr->setSectionHidden(3, false);
		}
		else{
			headerPtr->setSectionHidden(3, true);
		}
	}
}


void CFileSystemExplorerGuiComp::OnGuiDestroyed()
{
	BaseClass::OnGuiDestroyed();
}


// private slots:

void CFileSystemExplorerGuiComp::OnFilterChanged()
{
	QString filterText = m_filterEdit->GetText();

	QStringList fileFilters;

	if (!filterText.isEmpty()){
		fileFilters << filterText;
	}

	if (m_fileFilterAttrPtr.IsValid()){
		fileFilters << iqt::GetQString(*m_fileFilterAttrPtr);
	}

	m_fileSystemModel.setNameFilters(fileFilters);
}


} // namespace iqtgui


