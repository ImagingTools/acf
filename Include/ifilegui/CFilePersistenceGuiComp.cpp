#include <ifilegui/CFilePersistenceGuiComp.h>


namespace ifilegui
{


// protected slots

void CFilePersistenceGuiComp::on_LoadButton_clicked()
{
	if (m_fileLoaderCompPtr.IsValid() && m_objectCompPtr.IsValid()){
		m_fileLoaderCompPtr->LoadFromFile(*m_objectCompPtr);
	}
}


void CFilePersistenceGuiComp::on_SaveButton_clicked()
{
	if (m_fileLoaderCompPtr.IsValid() && m_objectCompPtr.IsValid()){
		m_fileLoaderCompPtr->SaveToFile(*m_objectCompPtr);
	}
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CFilePersistenceGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	LoadButton->setVisible(*m_showLoadAttrPtr);
	SaveButton->setVisible(*m_showSaveAttrPtr);

	LoadButton->setEnabled(
				m_fileLoaderCompPtr.IsValid() &&
				m_objectCompPtr.IsValid() &&
				m_fileLoaderCompPtr->IsOperationSupported(m_objectCompPtr.GetPtr(), NULL, ifile::IFilePersistence::QF_LOAD | ifile::IFilePersistence::QF_ANONYMOUS));
	SaveButton->setEnabled(
				m_fileLoaderCompPtr.IsValid() &&
				m_objectCompPtr.IsValid() &&
				m_fileLoaderCompPtr->IsOperationSupported(m_objectCompPtr.GetPtr(), NULL, ifile::IFilePersistence::QF_SAVE | ifile::IFilePersistence::QF_ANONYMOUS));
}


} // namespace ifilegui


