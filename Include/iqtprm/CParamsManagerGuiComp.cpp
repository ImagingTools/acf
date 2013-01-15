#include "iqtprm/CParamsManagerGuiComp.h"


namespace iqtprm
{


// protected methods

// reimplemented (CParamsManagerGuiCompBase)

imod::IObserver* CParamsManagerGuiComp::GetObserverPtr(const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	return m_paramsObserverCompPtr.GetPtr();
}


iqtgui::IGuiObject* CParamsManagerGuiComp::GetEditorGuiPtr(const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	return m_paramsGuiCompPtr.GetPtr();
}


// reimplemented (iqtgui::CComponentBase)

void CParamsManagerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_paramsGuiCompPtr.IsValid()){
		m_paramsGuiCompPtr->CreateGui(ParamsFrame);
	}
}


void CParamsManagerGuiComp::OnGuiDestroyed()
{
	if (m_paramsGuiCompPtr.IsValid()){
		m_paramsGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


} // namespace iqtprm


