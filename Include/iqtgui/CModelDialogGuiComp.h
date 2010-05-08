#ifndef iqtgui_CModelDialogGuiComp_included
#define iqtgui_CModelDialogGuiComp_included


// ACF includes
#include "imod/IObserver.h"
#include "imod/IModel.h"

#include "iqtgui/CDialogGuiComp.h"


namespace iqtgui
{


/**
	Component for the dialog based model editing.
*/
class CModelDialogGuiComp: public iqtgui::CDialogGuiComp
{
public:
	typedef iqtgui::CDialogGuiComp BaseClass;

	I_BEGIN_COMPONENT(CModelDialogGuiComp)
		I_ASSIGN(m_editorCompPtr, "Gui", "Editor for the model data", true, "ModelObserver");
		I_ASSIGN(m_modelCompPtr, "Model", "Model data", true, "Model");
		I_ASSIGN(m_dataCompPtr, "Model", "Model data", true, "Model");
	I_END_COMPONENT

	// reimplemented (iqtgui::IDialog)
	virtual void Execute();

private:
	I_REF(imod::IObserver, m_editorCompPtr);
	I_REF(imod::IModel, m_modelCompPtr);
	I_REF(istd::IChangeable, m_dataCompPtr);

	istd::TDelPtr<istd::IChangeable> m_workingDataPtr;
};


} // namespace iqtgui


#endif // !iqtgui_CModelDialogGuiComp_included

