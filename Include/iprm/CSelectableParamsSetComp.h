#ifndef iprm_CSelectableParamsSetComp_included
#define iprm_CSelectableParamsSetComp_included


#include "icomp/CComponentBase.h"

#include "imod/CSingleModelObserverBase.h"
#include "imod/CMultiModelBridgeBase.h"

#include "iprm/IOptionsList.h"
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
			virtual protected iprm::IOptionsList
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
		I_ASSIGN(m_serializeParamsManagerAttrPtr, "SerializeManager", "If enabled, the connected parameter manager will be part of the object persistence", false, false);
		I_ASSIGN(m_typeIdAttrPtr, "TypeId", "ID of this parameter set", true, "Default");
	I_END_COMPONENT;

	CSelectableParamsSetComp();

	// reimplemented (iser::IObject)
	virtual QByteArray GetFactoryId() const;

	// reimplemented (iprm::IParamsSet)
	virtual Ids GetParamIds(bool editableOnly = false) const;
	virtual const iser::ISerializable* GetParameter(const QByteArray& id) const;
	virtual iser::ISerializable* GetEditableParameter(const QByteArray& id);

	// reimplemented (iprm::ISelectionParam)
	virtual const IOptionsList* GetSelectionConstraints() const;
	virtual int GetSelectedOptionIndex() const;
	virtual bool SetSelectedOptionIndex(int index);
	virtual ISelectionParam* GetSubselection(int index) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	/**
		Set the bridge to the currently selected parameter set.
		Over this mechanism the changes in the slave parameter set will be reflected by this object.
		\sa CurrentParamsSetObserver
	*/
	void SetupCurrentParamsSetBridge();

	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const;
	virtual int GetOptionsCount() const;
	virtual QString GetOptionName(int index) const;
	virtual QString GetOptionDescription(int index) const;
	virtual QByteArray GetOptionId(int index) const;
	virtual bool IsOptionEnabled(int index) const;

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

	I_ATTR(QByteArray, m_selectionIdAttrPtr);
	I_ATTR(int, m_defaultIndexAttrPtr);
	I_REF(IParamsManager, m_paramsManagerCompPtr);
	I_ATTR(bool, m_serializeParamsManagerAttrPtr);
	I_ATTR(QByteArray, m_typeIdAttrPtr);
};


} // namespace iprm


#endif // !iprm_CSelectableParamsSetComp_included


