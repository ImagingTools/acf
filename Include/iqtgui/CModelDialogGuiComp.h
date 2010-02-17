#ifndef iqtgui_CModelDialogGuiComp_included
#define iqtgui_CModelDialogGuiComp_included


// ACF includes
#include "imod/IObserver.h"
#include "imod/IModel.h"

#include "iqtgui/IDialog.h"
#include "iqtgui/CGuiComponentDialog.h"


namespace iqtgui
{


/**
	Component for the dialog based model editing.

*/
class CModelDialogGuiComp: public icomp::CComponentBase, virtual public iqtgui::IDialog
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CModelDialogGuiComp)
		I_REGISTER_INTERFACE(iqtgui::IDialog);
		I_ASSIGN(m_editorGuiCompPtr, "ModelEditor", "Editor for the model data", true, "ModelEditor");
		I_ASSIGN(m_editorCompPtr, "ModelEditor", "Editor for the model data", true, "ModelEditor");
		I_ASSIGN(m_modelCompPtr, "Model", "Model data", true, "Model");
		I_ASSIGN(m_dataCompPtr, "Model", "Model data", true, "Model");
		I_ASSIGN(m_workingModelCompPtr, "WorkingModel", "Working model data", true, "WorkingModel");
		I_ASSIGN(m_workingDataCompPtr, "WorkingModel", "Working model data", true, "WorkingModel");
	I_END_COMPONENT

	// reimplemented (iqtgui::IDialog)
	virtual void Execute();

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

private:
	I_REF(iqtgui::IGuiObject, m_editorGuiCompPtr);
	I_REF(imod::IObserver, m_editorCompPtr);
	I_REF(imod::IModel, m_modelCompPtr);
	I_REF(istd::IChangeable, m_dataCompPtr);
	I_REF(imod::IModel, m_workingModelCompPtr);
	I_REF(istd::IChangeable, m_workingDataCompPtr);

	istd::TDelPtr<iqtgui::CGuiComponentDialog> m_dialogPtr;
};


} // namespace iqtgui


#endif // !iqtgui_CModelDialogGuiComp_included

