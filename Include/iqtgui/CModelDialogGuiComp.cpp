#include "iqtgui/CModelDialogGuiComp.h"


// Qt includes
#include <QIcon>


namespace iqtgui
{


// public methods

// reimplemented (iqtgui::IDialog)

void CModelDialogGuiComp::Execute()
{
	I_ASSERT(m_dataCompPtr.IsValid());
	I_ASSERT(m_modelCompPtr.IsValid());
	I_ASSERT(m_workingDataFactoryCompPtr.IsValid());
	I_ASSERT(m_workingModelFactoryCompPtr.IsValid());

	// create working model:
	if (!m_workingDataFactoryCompPtr.IsValid()){
		return;
	}

	istd::TDelPtr<iqtgui::CGuiComponentDialog> dialogPtr(CreateDialog());
	if (!dialogPtr.IsValid()){
		return;
	}

	bool orignalModelAttached = m_modelCompPtr->IsAttached(m_editorCompPtr.GetPtr());
	if (orignalModelAttached){
		m_modelCompPtr->DetachObserver(m_editorCompPtr.GetPtr());
	}

	m_workingDataPtr.SetPtr(m_workingDataFactoryCompPtr.CreateInstance());

	if (m_workingDataPtr.IsValid()){
		if (m_workingDataPtr->CopyFrom(*m_dataCompPtr.GetPtr())){
			imod::IModel* workingModelPtr = dynamic_cast<imod::IModel*>(m_workingDataPtr.GetPtr());
			I_ASSERT(workingModelPtr != NULL);
			if (workingModelPtr != NULL){
				bool isAttached = workingModelPtr->AttachObserver(m_editorCompPtr.GetPtr());
				if (isAttached){
					int retVal = dialogPtr->exec();

					if (retVal == QDialog::Accepted){
						m_dataCompPtr->CopyFrom(*m_workingDataPtr.GetPtr());
					}

					// re-attach the original data model to the editor:
					workingModelPtr->DetachObserver(m_editorCompPtr.GetPtr());

					if (orignalModelAttached){
						m_modelCompPtr->AttachObserver(m_editorCompPtr.GetPtr());
					}
				}
			}
		}
	}

	m_workingDataPtr.Reset();
}


} // namespace iqtgui


