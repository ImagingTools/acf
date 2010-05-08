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

	// create working model:
	istd::TDelPtr<iqtgui::CGuiComponentDialog> dialogPtr(CreateComponentDialog());
	if (!dialogPtr.IsValid()){
		return;
	}

	bool orignalModelAttached = m_modelCompPtr->IsAttached(m_editorCompPtr.GetPtr());
	if (orignalModelAttached){
		m_modelCompPtr->DetachObserver(m_editorCompPtr.GetPtr());
	}

	m_workingDataPtr.SetPtr(m_dataCompPtr->CloneMe());

	if (m_workingDataPtr.IsValid()){
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

	m_workingDataPtr.Reset();
}


} // namespace iqtgui


