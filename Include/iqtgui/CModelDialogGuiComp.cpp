#include "iqtgui/CModelDialogGuiComp.h"



namespace iqtgui
{


// public methods

// reimplemented (iqtgui::IDialog)

void CModelDialogGuiComp::Execute()
{
	I_ASSERT(m_dataCompPtr.IsValid());
	I_ASSERT(m_modelCompPtr.IsValid());
	I_ASSERT(m_workingModelCompPtr.IsValid());
	I_ASSERT(m_workingDataCompPtr.IsValid());

	if (!m_workingModelCompPtr.IsValid()){
		return;
	}

	if (!m_dialogPtr.IsValid()){
		return;
	}

	if (m_workingDataCompPtr.IsValid() && m_workingModelCompPtr.IsValid()){
		if (m_workingDataCompPtr->CopyFrom(*m_dataCompPtr.GetPtr())){
			m_workingModelCompPtr->AttachObserver(m_editorCompPtr.GetPtr());
		}
	}

	if (m_workingModelCompPtr->IsAttached(m_editorCompPtr.GetPtr())){
		int retVal = m_dialogPtr->exec();

		if (retVal == QDialog::Accepted){
			m_dataCompPtr->CopyFrom(*m_workingDataCompPtr.GetPtr());
		}

		m_workingModelCompPtr->DetachObserver(m_editorCompPtr.GetPtr());
	}
}


// reimplemented (icomp::IComponent)

void CModelDialogGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_dialogPtr.SetPtr(
				new iqtgui::CGuiComponentDialog(
							m_editorGuiCompPtr.GetPtr(),
							QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
							true));
}


} // namespace iqtgui


