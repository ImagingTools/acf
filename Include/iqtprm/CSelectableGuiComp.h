#ifndef iqtprm_CSelectableGuiComp_included
#define iqtprm_CSelectableGuiComp_included


// ACF includes
#include "istd/TPointerVector.h"

#include "iprm/ISelectionParam.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtprm/Generated/ui_CSelectableGuiComp.h"


namespace iqtprm
{


/**
	Component to show any selected GUI, implemented as observer of iprm::ISelectionParam.
	The view for no selection
*/
class CSelectableGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CSelectableGuiComp,
			iprm::ISelectionParam>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CSelectableGuiComp,
				iprm::ISelectionParam> BaseClass;

	I_BEGIN_COMPONENT(CSelectableGuiComp);
		I_ASSIGN_MULTI_0(m_slaveGuisCompPtr, "SlaveGuis", "List of the slave GUI's", true);
		I_ASSIGN(m_noSelectionLabelAttrPtr, "NoSelectionLabel", "Text to be shown for empty selection", false, "Nothing selected");
		I_ASSIGN(m_noSelectionIconAttrPtr, "NoSelectionIcon", "File path to the image to be shown for empty selection", false, "");
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

private:
	typedef std::map<QWidget*, iqtgui::IGuiObject*> WidgetGuiMap;

	WidgetGuiMap m_widgetToGuiMap;

	I_MULTIREF(iqtgui::IGuiObject, m_slaveGuisCompPtr);
	I_ATTR(istd::CString, m_noSelectionLabelAttrPtr);
	I_ATTR(istd::CString, m_noSelectionIconAttrPtr);
};


} // namespace iqtprm


#endif // !iqtprm_CSelectableGuiComp_included


