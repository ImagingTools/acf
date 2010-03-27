#ifndef icmpstr_CRegistryPropEditorComp_included
#define icmpstr_CRegistryPropEditorComp_included


// ACF includes
#include "icomp/IRegistry.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/CCheckableComboBox.h"

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

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

protected slots:
	void on_DescriptionEdit_editingFinished();
	void on_KeywordsEdit_editingFinished();
	void OnCategoriesChanged(const QStringList& categories);

private:
	void CreateOverview();

	QStringList GetCategoryStringList(int category) const;

	typedef QMap<int, QString> CategoriesMap;
	CategoriesMap m_categoriesMap;

	iqtgui::CCheckableComboBox* m_categoryComboBox;
};


} // namespace icmpstr


#endif // !icmpstr_CRegistryPropEditorComp_included

