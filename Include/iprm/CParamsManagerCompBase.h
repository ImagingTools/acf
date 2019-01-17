#ifndef iprm_CParamsManagerCompBase_included
#define iprm_CParamsManagerCompBase_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include <istd/TDelPtr.h>
#include <imod/TModelWrap.h>
#include <imod/CModelUpdateBridge.h>
#include <icomp/CComponentBase.h>
#include <iprm/IParamsSet.h>
#include <iprm/ISelectionParam.h>
#include <iprm/IOptionsManager.h>
#include <iprm/IParamsManager.h>
#include <iprm/CNameParam.h>


namespace iprm
{


class CParamsManagerCompBaseAttr: public icomp::CComponentBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_BASE_COMPONENT(CParamsManagerCompBaseAttr);
		I_ASSIGN(m_elementIndexParamIdAttrPtr, "ElementIndexParamId", "ID of index of returned parameter set in manager list", false, "Index");
		I_ASSIGN(m_elementUuidParamIdAttrPtr, "ElementUuidParamId", "ID of object uuid of returned parameter set in manager list", false, "Uuid");
		I_ASSIGN(m_elementNameParamIdAttrPtr, "ElementNameParamId", "ID of the name of returned parameter set in manager list", false, "Name");
		I_ASSIGN(m_elementDescriptionParamIdAttrPtr, "ElementDescriptionParamId", "ID of the description of returned parameter set in manager list", false, "Description");
		I_ASSIGN_MULTI_0(m_fixedParamSetsCompPtr, "FixedParamSets", "List of references to fixed parameter set", false);
		I_ASSIGN_MULTI_0(m_fixedSetNamesAttrPtr, "FixedSetNames", "List of fixed parameter names", false);
		I_ASSIGN_MULTI_0(m_fixedSetDescriptionsAttrPtr, "FixedSetDescriptions", "List of fixed parameter descriptions", false);
		I_ASSIGN_MULTI_0(m_fixedSetIdsAttrPtr, "FixedSetIds", "List of fixed parameter IDs", false);
		I_ASSIGN(m_defaultSetNameAttrPtr, "DefaultSetName", "Default name of parameter set. Use %1 to insert automatic enumeration", true, "<noname>");
		I_ASSIGN(m_serializeSelectionAttrPtr, "SerializeSelection", "If enabled, the current parameter set selection will be serialized", true, true);
		I_ASSIGN(m_defaultSelectedIndexAttrPtr, "DefaultSelection", "If enabled, the given parameter set will be automatically selected", false, -1);
		I_ASSIGN(m_allowDisabledAttrPtr, "AllowDisabled", "Control if disabled parameters are supported", true, false);
		I_ASSIGN(m_supportEnablingAttrPtr, "SupportEnabling", "Control if enabling or disabling of parameters is allowed (works only if disabled parameters are supported)", true, true);
		I_ASSIGN(m_allowEditFixedAttrPtr, "AllowEditFixed", "If enabled, the editing of fixed parameters is enabled", true, true);
	I_END_COMPONENT;

protected:
	I_MULTIREF(IParamsSet, m_fixedParamSetsCompPtr);
	I_MULTITEXTATTR(m_fixedSetNamesAttrPtr);
	I_MULTITEXTATTR(m_fixedSetDescriptionsAttrPtr);
	I_MULTIATTR(QByteArray, m_fixedSetIdsAttrPtr);
	I_TEXTATTR(m_defaultSetNameAttrPtr);
	I_ATTR(bool, m_serializeSelectionAttrPtr);
	I_ATTR(int, m_defaultSelectedIndexAttrPtr);
	I_ATTR(bool, m_allowDisabledAttrPtr);
	I_ATTR(bool, m_supportEnablingAttrPtr);
	I_ATTR(bool, m_allowEditFixedAttrPtr);
	I_ATTR(QByteArray, m_elementIndexParamIdAttrPtr);
	I_ATTR(QByteArray, m_elementUuidParamIdAttrPtr);
	I_ATTR(QByteArray, m_elementNameParamIdAttrPtr);
	I_ATTR(QByteArray, m_elementDescriptionParamIdAttrPtr);
};


/**
	Implementation of parameter manager.
*/
class CParamsManagerCompBase:
			public CParamsManagerCompBaseAttr,
			virtual public IParamsManager,
			virtual public IOptionsManager
{
public:
	typedef CParamsManagerCompBaseAttr BaseClass;

	I_BEGIN_BASE_COMPONENT(CParamsManagerCompBase);
		I_REGISTER_INTERFACE(ISelectionParam);
		I_REGISTER_INTERFACE(IParamsManager);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(IOptionsManager);
		I_REGISTER_INTERFACE(IOptionsList);
		I_REGISTER_SUBELEMENT(SelectedParams);
		I_REGISTER_SUBELEMENT_INTERFACE(SelectedParams, iprm::IParamsSet, ExtractCurrentParams);
		I_REGISTER_SUBELEMENT_INTERFACE(SelectedParams, iser::IObject, ExtractCurrentParams);
		I_REGISTER_SUBELEMENT_INTERFACE(SelectedParams, iser::ISerializable, ExtractCurrentParams);
		I_REGISTER_SUBELEMENT_INTERFACE(SelectedParams, istd::IChangeable, ExtractCurrentParams);
		I_REGISTER_SUBELEMENT_INTERFACE(SelectedParams, imod::IModel, ExtractCurrentParams);
	I_END_COMPONENT;

	CParamsManagerCompBase();

	// reimplemented (iprm::IParamsManager)
	virtual int InsertParamsSet(int typeIndex = -1, int index = -1);
	virtual bool RemoveParamsSet(int index);
	virtual bool SwapParamsSet(int index1, int index2);
	virtual IParamsSet* GetParamsSet(int index) const;
	virtual IParamsSet* CreateParameterSet(int typeIndex = -1, int index = -1) const;
	virtual int GetIndexOperationFlags(int index = -1) const;
	virtual int GetParamsSetsCount() const;
	virtual QString GetParamsSetName(int index) const;
	virtual bool SetParamsSetName(int index, const QString& name);
	virtual QString GetParamsSetDescription(int index) const;
	virtual void SetParamsSetDescription(int index, const QString& description);

	// reimplemented (iprm::ISelectionParam)
	virtual const IOptionsList* GetSelectionConstraints() const;
	virtual int GetSelectedOptionIndex() const;
	virtual bool SetSelectedOptionIndex(int index);
	virtual ISelectionParam* GetSubselection(int index) const;

	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const;
	virtual int GetOptionsCount() const;
	virtual QString GetOptionName(int index) const;
	virtual QString GetOptionDescription(int index) const;
	virtual QByteArray GetOptionId(int index) const;
	virtual bool IsOptionEnabled(int index) const;

protected:
	void EnsureParamsSetModelDetached(iprm::IParamsSet* paramsSetPtr) const;
	QString CalculateNewDefaultName() const;
	int FindParamSetIndex(const QString& name) const;
	int FindFixedParamSetIndex(const QString& name) const;

	// abstract methods

	/**
		Return \c true if creation of parameter sets is supported by the implementation.
	*/
	virtual bool IsParameterCreationSupported() const = 0;

	/**
		Get the number of parameter sets created at the run time.
	*/
	virtual int GetCreatedParamsSetsCount() const = 0;

	/**
		Create a new instance of the parameter set.
	*/
	virtual iprm::IParamsSet* CreateParamsSetInstance(int typeIndex = -1) const = 0;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

protected:
	int m_selectedIndex;

	class ParamSet:
				virtual public IParamsSet,
				virtual public ISelectionParam,
				virtual public INameParam
	{
	public:
		ParamSet();

		// reimplemented (iprm::IParamsSet)
		virtual Ids GetParamIds(bool editableOnly = false) const;
		virtual const iser::ISerializable* GetParameter(const QByteArray& id) const;
		virtual iser::ISerializable* GetEditableParameter(const QByteArray& id);

		// reimplemented (iprm::ISelectionParam)
		virtual const IOptionsList* GetSelectionConstraints() const;
		virtual int GetSelectedOptionIndex() const;
		virtual bool SetSelectedOptionIndex(int index);
		virtual ISelectionParam* GetSubselection(int index) const;

		// reimplemented (iprm::INameParam)
		virtual const QString& GetName() const;
		virtual void SetName(const QString& name);
		virtual bool IsNameFixed() const;

		// reimplemented (iser::IObject)
		virtual QByteArray GetFactoryId() const;

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive);

		class UuidParam: virtual public INameParam
		{
		public:
			UuidParam(const ParamSet& parent) : m_parent(parent) {}

			// reimplemented (iprm::INameParam)
			virtual const QString& GetName() const 
			{
				m_uuid = m_parent.uuid;
				return m_uuid;
			}
			virtual void SetName(const QString& /*name*/) {}
			virtual bool IsNameFixed() const { return true; }

			// reimplemented (iser::ISerializable)
			virtual bool Serialize(iser::IArchive& /*archive*/) { return true; }
		private:
			mutable QString m_uuid;
			const ParamSet& m_parent;
		};

		istd::TDelPtr<IParamsSet> paramSetPtr;
		QByteArray uuid;
		QString name;
		iprm::CNameParam description;
		bool isEnabled;
		CParamsManagerCompBase* parentPtr;
		UuidParam uuidParam;
		imod::CModelUpdateBridge updateBridge;
	};
	
	class SelectedParams: virtual public IParamsSet
	{
	public:
		SelectedParams();

		// reimplemented (iprm::IParamsSet)
		virtual Ids GetParamIds(bool editableOnly = false) const;
		virtual const iser::ISerializable* GetParameter(const QByteArray& id) const;
		virtual iser::ISerializable* GetEditableParameter(const QByteArray& id);

		// reimplemented (iser::IObject)
		virtual QByteArray GetFactoryId() const;

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive);

		CParamsManagerCompBase* parentPtr;
	};

	typedef istd::TDelPtr<ParamSet> ParamSetPtr;

	typedef QList<ParamSetPtr> ParamSets;

	ParamSets m_paramSets;

	imod::CModelUpdateBridge m_updateBridge;

private:
	// static template methods for subelement access
	template <class InterfaceType>
	static InterfaceType* ExtractCurrentParams(CParamsManagerCompBase& component)
	{
		return &component.m_selectedParams;
	}

	imod::TModelWrap<SelectedParams> m_selectedParams;
};


} // namespace iprm


#endif // !iprm_CParamsManagerCompBase_included


