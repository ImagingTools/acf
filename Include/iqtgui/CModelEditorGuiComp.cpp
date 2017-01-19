#include <iqtgui/CModelEditorGuiComp.h>


namespace iqtgui
{


CModelEditorGuiComp::CModelEditorGuiComp()
:	m_isConnected(false)
{
}


// reimplemented (iqtgui::IGuiObject)

bool CModelEditorGuiComp::IsGuiCreated() const
{
	if (m_slaveGuiCompPtr.IsValid()){
		return m_slaveGuiCompPtr->IsGuiCreated();
	}

	return false;
}


bool CModelEditorGuiComp::CreateGui(QWidget* parentPtr)
{
	if (m_slaveGuiCompPtr.IsValid()){
		return m_slaveGuiCompPtr->CreateGui(parentPtr);
	}

	return false;
}


bool CModelEditorGuiComp::DestroyGui()
{
	if (m_slaveGuiCompPtr.IsValid()){
		return m_slaveGuiCompPtr->DestroyGui();
	}

	return false;
}


QWidget* CModelEditorGuiComp::GetWidget() const
{
	if (m_slaveGuiCompPtr.IsValid()){
		return m_slaveGuiCompPtr->GetWidget();
	}

	return NULL;
}


void CModelEditorGuiComp::OnTryClose(bool* ignoredPtr)
{
	if (m_slaveGuiCompPtr.IsValid()){
		return m_slaveGuiCompPtr->OnTryClose(ignoredPtr);
	}
}


// reimplemented (imod::IModelEditor)

void CModelEditorGuiComp::UpdateEditor(const istd::IChangeable::ChangeSet& changeSet)
{
	Q_ASSERT(IsGuiCreated());

	if (m_slaveEditorCompPtr.IsValid()){
		m_slaveEditorCompPtr->UpdateEditor(changeSet);
	}
}


void CModelEditorGuiComp::UpdateModelFromEditor() const
{
	Q_ASSERT(IsGuiCreated());

	if (m_slaveEditorCompPtr.IsValid()){
		m_slaveEditorCompPtr->UpdateModelFromEditor();
	}
}


bool CModelEditorGuiComp::IsReadOnly() const
{
	if (m_slaveEditorCompPtr.IsValid()){
		return m_slaveEditorCompPtr->IsReadOnly();
	}

	return true;
}


void CModelEditorGuiComp::SetReadOnly(bool state)
{
	if (m_slaveEditorCompPtr.IsValid()){
		m_slaveEditorCompPtr->SetReadOnly(state);
	}
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CModelEditorGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	Q_ASSERT(!m_isConnected);

	if (m_slaveObserverCompPtr.IsValid() && m_modelCompPtr.IsValid()){
		m_isConnected = m_modelCompPtr->AttachObserver(m_slaveObserverCompPtr.GetPtr());
	}
}


void CModelEditorGuiComp::OnComponentDestroyed()
{
	if (m_isConnected){
		Q_ASSERT(m_slaveObserverCompPtr.IsValid());
		Q_ASSERT(m_modelCompPtr.IsValid());

		m_modelCompPtr->DetachObserver(m_slaveObserverCompPtr.GetPtr());
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace iqtgui


