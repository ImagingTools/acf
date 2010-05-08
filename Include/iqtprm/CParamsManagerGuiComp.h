#ifndef iqtprm_CParamsManagerGuiComp_included
#define iqtprm_CParamsManagerGuiComp_included


#include "iprm/IParamsManager.h"
#include "iprm/ISelectionParam.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtprm/Generated/ui_CParamsManagerGuiComp.h"


namespace iqtprm
{


class CParamsManagerGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CParamsManagerGuiComp,
			iprm::IParamsManager>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CParamsManagerGuiComp,
				iprm::IParamsManager> BaseClass;

	I_BEGIN_COMPONENT(CParamsManagerGuiComp)
		I_ASSIGN(m_paramsGuiCompPtr, "ParamsGui", "GUI object used to display parameters", false, "ParamsGui")
		I_ASSIGN(m_paramsObserverCompPtr, "ParamsGui", "GUI object used to display parameters", false, "ParamsGui")
		I_ASSIGN(m_allowAddRemoveAttrPtr, "AllowAddRemove", "If it is false, 'Add' and 'Remove' buttons will be always hidden", true, true)
	I_END_COMPONENT

	CParamsManagerGuiComp();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

protected slots:
	void on_AddButton_clicked();
	void on_RemoveButton_clicked();
	void on_ParamsTree_itemSelectionChanged();
	void on_ParamsTree_itemChanged(QTreeWidgetItem* item, int column);

protected:
	void UpdateActions();
	void UpdateTree();
	void UpdateParamsView(int selectedIndex);
	int GetSelectedIndex() const;
	void EnsureParamsGuiDetached();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

private:
	I_REF(iqtgui::IGuiObject, m_paramsGuiCompPtr);
	I_REF(imod::IObserver, m_paramsObserverCompPtr);
	I_ATTR(bool, m_allowAddRemoveAttrPtr);

	imod::IModel* m_lastConnectedModelPtr;
};


} // namespace iqtprm


#endif // !iqtprm_CParamsManagerGuiComp_included


