#ifndef iqtprm_CComboParamsManagerGuiComp_included
#define iqtprm_CComboParamsManagerGuiComp_included


#include "iprm/IParamsManager.h"
#include "iprm/ISelectionParam.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtprm/Generated/ui_CComboParamsManagerGuiComp.h"


namespace iqtprm
{


class CComboParamsManagerGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CComboParamsManagerGuiComp,
			iprm::IParamsManager>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CComboParamsManagerGuiComp,
				iprm::IParamsManager> BaseClass;

	I_BEGIN_COMPONENT(CComboParamsManagerGuiComp);
		I_ASSIGN(m_paramsGuiCompPtr, "ParamsGui", "GUI object used to display parameters", false, "ParamsGui");
		I_ASSIGN(m_paramsObserverCompPtr, "ParamsGui", "GUI object used to display parameters", false, "ParamsGui");
		I_ASSIGN(m_selectionGuiCompPtr, "SelectionGui", "UI for selection change", false, "SelectionGui");
		I_ASSIGN_TO(m_selectionObserverCompPtr, m_selectionGuiCompPtr, false);
	I_END_COMPONENT;

	CComboParamsManagerGuiComp();

protected:
	void UpdateParamsView(int selectedIndex);
	void EnsureParamsGuiDetached();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_REF(iqtgui::IGuiObject, m_paramsGuiCompPtr);
	I_REF(imod::IObserver, m_paramsObserverCompPtr);
	I_REF(iqtgui::IGuiObject, m_selectionGuiCompPtr);
	I_REF(imod::IObserver, m_selectionObserverCompPtr);
	
	imod::IModel* m_lastConnectedModelPtr;
};


} // namespace iqtprm


#endif // !iqtprm_CComboParamsManagerGuiComp_included


