#ifndef icmpstr_CRegistryPropEditorComp_included
#define icmpstr_CRegistryPropEditorComp_included


// ACF includes
#include "icomp/IRegistry.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "icmpstr/icmpstr.h"

#include "icmpstr/Generated/ui_CRegistryPropEditorComp.h"


namespace icmpstr
{


class CRegistryPropEditorComp: public iqtgui::TDesignerGuiObserverCompBase<Ui::CRegistryPropEditorComp, icomp::IRegistry>
{
    Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<Ui::CRegistryPropEditorComp, icomp::IRegistry> BaseClass;

	I_BEGIN_COMPONENT(CRegistryPropEditorComp);
	I_END_COMPONENT;

	// reimplmented (imod::IModelEditor)
	virtual void UpdateEditor(int updateFlags = 0);
	virtual void UpdateModel() const;

protected slots:
	void on_DescriptionEdit_editingFinished();
	void on_KeywordsEdit_editingFinished();
};


} // namespace icmpstr


#endif // !icmpstr_CRegistryPropEditorComp_included

