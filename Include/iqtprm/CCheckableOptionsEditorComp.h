#ifndef iqtprm_CCheckableOptionsEditorComp_included
#define iqtprm_CCheckableOptionsEditorComp_included


// ACF includes
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iprm/IOptionsManager.h"

#include "GeneratedFiles/iqtprm/ui_CCheckableOptionsEditorComp.h"


namespace iqtprm
{


class CCheckableOptionsEditorComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CCheckableOptionsEditorComp,
						iprm::IOptionsManager>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CCheckableOptionsEditorComp,
				iprm::IOptionsManager> BaseClass;

	I_BEGIN_COMPONENT(CCheckableOptionsEditorComp);
		I_ASSIGN(m_useHorizontalLayoutAttrPtr, "UseHorizontalLayout", "Horizontal mode for GUI", true, true);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected Q_SLOTS:
	void OnParameterChanged();

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

private:
	I_ATTR(bool, m_useHorizontalLayoutAttrPtr);
};


} // namespace iqtprm


#endif // !iqtprm_CCheckableOptionsEditorComp_included


