#include "iqtdoc/CModelEditorGuiComp.h"


namespace iqtdoc
{


CModelEditorGuiComp::CModelEditorGuiComp()
:	m_isConnected(false)
{
}


// reimplemented (icomp::IComponent)

void CModelEditorGuiComp::OnComponentCreated()
{
	I_ASSERT(!m_isConnected);

	if (m_slaveObserverCompPtr.IsValid() && m_modelCompPtr.IsValid()){
		m_isConnected = m_modelCompPtr->AttachObserver(m_slaveObserverCompPtr.GetPtr());
	}
}


void CModelEditorGuiComp::OnComponentDestroyed()
{
	if (m_isConnected){
		I_ASSERT(m_slaveObserverCompPtr.IsValid());
		I_ASSERT(m_modelCompPtr.IsValid());

		m_modelCompPtr->DetachObserver(m_slaveObserverCompPtr.GetPtr());
	}
}


// reimplemented (iqt::IGuiObject)

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

void CModelEditorGuiComp::UpdateEditor()
{
	if (m_slaveEditorCompPtr.IsValid()){
		m_slaveEditorCompPtr->UpdateEditor();
	}
}


void CModelEditorGuiComp::UpdateModel() const
{
	if (m_slaveEditorCompPtr.IsValid()){
		m_slaveEditorCompPtr->UpdateModel();
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


// reimplemented (iqt2d::ISceneExtender)

void CModelEditorGuiComp::AddItemsToScene(iqt2d::ISceneProvider* providerPtr, int flags)
{
	if (m_slaveExtenderCompPtr.IsValid()){
		m_slaveExtenderCompPtr->AddItemsToScene(providerPtr, flags);
	}
}


void CModelEditorGuiComp::RemoveItemsFromScene(iqt2d::ISceneProvider* providerPtr)
{
	if (m_slaveExtenderCompPtr.IsValid()){
		m_slaveExtenderCompPtr->RemoveItemsFromScene(providerPtr);
	}
}


} // namespace iqtdoc


