#ifndef iprm_CSelectableParamsSetComp_included
#define iprm_CSelectableParamsSetComp_included


#include "icomp/CComponentBase.h"

#include "imod/CSingleModelObserverBase.h"
#include "imod/CMultiModelBridgeBase.h"

#include "iprm/ISelectionConstraints.h"
#include "iprm/IParamsSet.h"
#include "iprm/ISelectionParam.h"
#include "iprm/IParamsManager.h"


namespace iprm
{


/**
	Interface allowing to select single option from list of options.
*/
class CSelectableParamsSetComp:
			public icomp::CComponentBase,
			protected imod::CMultiModelBridgeBase,
			virtual public iprm::IParamsSet,
			virtual public iprm::ISelectionParam,
			virtual protected iprm::ISelectionConstraints
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSelectableParamsSetComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(IParamsSet);
		I_REGISTER_INTERFACE(ISelectionParam);
		I_ASSIGN(m_selectionIdAttrPtr, "SelectionId", "ID of selection in parameter set", true, "Input");
		I_ASSIGN(m_defaultIndexAttrPtr, "DefaultIndex", "Default selected index", false, 0);
		I_ASSIGN(m_paramsManagerCompPtr, "ParamsManager", "Manager of parameter set used to realize selection", true, "ParamsManager");
	I_END_COMPONENT;

	CSelectableParamsSetComp();

	// reimplemented (iprm::IParamsSet)
	virtual const iser::ISerializable* GetParameter(const std::string& id) const;
	virtual iser::ISerializable* GetEditableParameter(const std::string& id);

	// reimplemented (iprm::ISelectionParam)
	virtual const ISelectionConstraints* GetSelectionConstraints() const;
	virtual int GetSelectedOptionIndex() const;
	virtual bool SetSelectedOptionIndex(int index);
	virtual ISelectionParam* GetActiveSubselection() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	/**
		Set the bridge to the currently selected parameter set.
		Over this mechanism the changes in the slave parameter set will be reflected by this object.
		\sa CurrentParamsSetObserver
	*/
	void SetupCurrentParamsSetBridge();

	// reimplemented (iprm::ISelectionConstraints)
	virtual int GetConstraintsFlags() const;
	virtual int GetOptionsCount() const;
	virtual istd::CString GetOptionName(int index) const;
	virtual istd::CString GetOptionDescription(int index) const;
	virtual std::string GetOptionId(int index) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	/**
		Observer for the selected parameter set.
		On changes in the observed model the change event will be delegated to the observers of the CSelectableParamsSetComp object.
	*/
	class CurrentParamsSetObserver: public imod::CSingleModelObserverBase
	{
	public:
		CurrentParamsSetObserver(CSelectableParamsSetComp& parent);

		// reimplemented (imod::CSingleModelObserverBase)
		virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
		virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CSelectableParamsSetComp& m_parent;
	};

	int m_selectedIndex;
	CurrentParamsSetObserver m_currentParamsSetObserver;

	I_ATTR(istd::CString, m_selectionIdAttrPtr);
	I_ATTR(int, m_defaultIndexAttrPtr);
	I_REF(IParamsManager, m_paramsManagerCompPtr);
};


} // namespace iprm


#endif // !iprm_CSelectableParamsSetComp_included


