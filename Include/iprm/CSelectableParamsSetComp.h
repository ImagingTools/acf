#ifndef iprm_CSelectableParamsSetComp_included
#define iprm_CSelectableParamsSetComp_included


#include <icomp/CComponentBase.h>

#include <imod/CSingleModelObserverBase.h>
#include <imod/CModelUpdateBridge.h>

#include <iprm/IOptionsList.h>
#include <iprm/IParamsSet.h>
#include <iprm/ISelectionParam.h>
#include <iprm/IParamsManager.h>


namespace iprm
{


/**
	Interface allowing to select single option from list of options.
*/
class CSelectableParamsSetComp:
			public icomp::CComponentBase,
			virtual public iprm::IParamsSet
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSelectableParamsSetComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(IParamsSet);
		I_ASSIGN(m_selectionParamIdAttrPtr, "SelectionParamId", "ID of selection in this parameter set", false, "Input");
		I_ASSIGN(m_currentSelectionCompPtr, "CurrentSelection", "Current selected index, if unspecified the selection from manager will be used", false, "CurrentSelection");
		I_ASSIGN_TO(m_currentSelectionModelCompPtr, m_currentSelectionCompPtr, false);
		I_ASSIGN(m_paramsManagerCompPtr, "ParamsManager", "Manager of parameter set used to realize selection", true, "ParamsManager");
		I_ASSIGN_TO(m_paramsManagerModelCompPtr, m_paramsManagerCompPtr, false);
		I_ASSIGN(m_allowEditingAttrPtr, "AllowEditing", "If enabled, the selected parameter set will be editable", true, false);
	I_END_COMPONENT;

	CSelectableParamsSetComp();

	// reimplemented (iprm::IParamsSet)
	virtual Ids GetParamIds(bool editableOnly = false) const override;
	virtual const iser::ISerializable* GetParameter(const QByteArray& id) const override;
	virtual iser::ISerializable* GetEditableParameter(const QByteArray& id) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

private:
	I_ATTR(QByteArray, m_selectionParamIdAttrPtr);
	I_ATTR(bool, m_allowEditingAttrPtr);
	I_REF(iprm::ISelectionParam, m_currentSelectionCompPtr);
	I_REF(imod::IModel, m_currentSelectionModelCompPtr);
	I_REF(IParamsManager, m_paramsManagerCompPtr);
	I_REF(imod::IModel, m_paramsManagerModelCompPtr);

	imod::CModelUpdateBridge m_updateBridge;
};


} // namespace iprm


#endif // !iprm_CSelectableParamsSetComp_included


