#ifndef iqtprm_CParamsManagerGuiComp_included
#define iqtprm_CParamsManagerGuiComp_included


#include "iprm/IParamsManager.h"
#include "iprm/ISelectionParam.h"

#include "iqt/TDesignerGuiObserverCompBase.h"

#include "iqtprm/Generated/ui_CParamsManagerGuiComp.h"


namespace iqtprm
{


class CParamsManagerGuiComp: public iqt::TDesignerGuiObserverCompBase<
			Ui::CParamsManagerGuiComp,
			iprm::ISelectionParam>
{
	Q_OBJECT

public:
	typedef iqt::TDesignerGuiObserverCompBase<
				Ui::CParamsManagerGuiComp,
				iprm::ISelectionParam> BaseClass;

	I_BEGIN_COMPONENT(CParamsManagerGuiComp)
		I_ASSIGN(m_paramsGuiCompPtr, "ParamsGui", "GUI object used to display parameters", false, "ParamsGui")
		I_ASSIGN(m_paramsObserverCompPtr, "ParamsGui", "GUI object used to display parameters", false, "ParamsGui")
		I_ASSIGN(m_paramsManagerCompPtr, "ParamsManager", "Manager of parameter set controled by this GUI", true, "ParamsManager")
		I_ASSIGN(m_paramsManagerModelCompPtr, "ParamsManager", "Manager of parameter set controled by this GUI", true, "ParamsManager")
		I_ASSIGN(m_allowAddRemoveAttrPtr, "AllowAddRemove", "If it is false, 'Add' and 'Remove' buttons will be always hidden", true, true)
	I_END_COMPONENT

	enum ChangeFlags
	{
		CF_PARAMS_ADD = 0x40,
		CF_PARAMS_REMOVE = 0x80,
	};

	CParamsManagerGuiComp();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor();

protected slots:
	void on_AddButton_clicked();
	void on_RemoveButton_clicked();
	void on_ParamsTree_itemSelectionChanged();

protected:
	class ManagerObserver: public imod::TSingleModelObserverBase<iprm::IParamsManager>
	{
	public:
		ManagerObserver(CParamsManagerGuiComp* parentPtr);

	protected:
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CParamsManagerGuiComp& m_parent;
	};

	void UpdateActions();
	void UpdateTree();
	void UpdateParamsView(int selectedIndex);
	int GetSelectedIndex() const;
	void EnsureParamsGuiDetached();

	// reimplemented (iqt::TGuiObserverWrap)
	virtual void OnGuiModelShown();
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(iqt::IGuiObject, m_paramsGuiCompPtr);
	I_REF(imod::IObserver, m_paramsObserverCompPtr);
	I_REF(iprm::IParamsManager, m_paramsManagerCompPtr);
	I_REF(imod::IModel, m_paramsManagerModelCompPtr);
	I_ATTR(bool, m_allowAddRemoveAttrPtr);

	imod::IModel* m_lastConnectedModelPtr;
	ManagerObserver m_managerObserver;
};


} // namespace iqtprm


#endif // !iqtprm_CParamsManagerGuiComp_included


