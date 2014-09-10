#ifndef iprm_CParamsManagerCompBase_included
#define iprm_CParamsManagerCompBase_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "istd/TDelPtr.h"
#include "imod/CMultiModelBridgeBase.h"
#include "icomp/CComponentBase.h"
#include "iprm/IParamsSet.h"
#include "iprm/ISelectionParam.h"
#include "iprm/IOptionsManager.h"
#include "iprm/IParamsManager.h"
#include "iprm/INameParam.h"


namespace iprm
{


/**
	Implementation of parameter manager.
*/
class CParamsManagerCompBase:
			public icomp::CComponentBase,
			virtual public IParamsManager,
			virtual public IOptionsManager,
			protected imod::CMultiModelBridgeBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_BASE_COMPONENT(CParamsManagerCompBase);
		I_REGISTER_INTERFACE(ISelectionParam);
		I_REGISTER_INTERFACE(IParamsManager);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(IOptionsManager);
		I_REGISTER_INTERFACE(IOptionsList);
		I_ASSIGN(m_elementIndexParamId, "ElementIndexParamId", "ID of index of returned parameter set in manager list", false, "Index");
		I_ASSIGN(m_elementNameParamId, "ElementNameParamId", "ID of the name of returned parameter set in manager list", false, "Name");
		I_ASSIGN_MULTI_0(m_fixedParamSetsCompPtr, "FixedParamSets", "List of references to fixed parameter set", false);
		I_ASSIGN_MULTI_0(m_fixedSetNamesAttrPtr, "FixedSetNames", "List of fixed parameter names", false);
		I_ASSIGN_MULTI_0(m_fixedSetDescriptionsAttrPtr, "FixedSetDescriptions", "List of fixed parameter descriptions", false);
		I_ASSIGN_MULTI_0(m_fixedSetIdsAttrPtr, "FixedSetIds", "List of fixed parameter IDs", false);
		I_ASSIGN(m_defaultSetNameAttrPtr, "DefaultSetName", "Default name of parameter set. Use %1 to insert automatic enumeration", true, "<noname>");
		I_ASSIGN(m_serializeSelectionAttrPtr, "SerializeSelection", "If enabled, the current parameter set selection will be serialized", true, true);
		I_ASSIGN(m_allowDisabledAttrPtr, "AllowDisabled", "Control if disabled parameters are supported", true, false);
		I_ASSIGN(m_supportEnablingAttrPtr, "SupportEnabling", "Control if enabling or disabling of parameters is allowed (works only if disabled parameters are supported)", true, true);
		I_ASSIGN(m_allowEditFixedAttrPtr, "AllowEditFixed", "If enabled, the editing of fixed parameters is enabled", true, true);
	I_END_COMPONENT;

	CParamsManagerCompBase();

	// reimplemented (iprm::IParamsManager)
	virtual int InsertParamsSet(int typeIndex = -1, int index = -1);
	virtual bool RemoveParamsSet(int index);
	virtual bool SwapParamsSet(int index1, int index2);
	virtual IParamsSet* GetParamsSet(int index) const;
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

protected:
	void EnsureParamsSetModelDetached(iprm::IParamsSet* paramsSetPtr) const;
	QString CalculateNewDefaultName() const;
	int FindParamSetIndex(const QString& name) const;

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
	virtual iprm::IParamsSet* CreateParamsSet(int typeIndex = -1) const = 0;

protected:
	int m_selectedIndex;

	I_MULTIREF(IParamsSet, m_fixedParamSetsCompPtr);
	I_MULTIATTR(QString, m_fixedSetNamesAttrPtr);
	I_MULTIATTR(QString, m_fixedSetDescriptionsAttrPtr);
	I_MULTIATTR(QByteArray, m_fixedSetIdsAttrPtr);
	I_ATTR(QString, m_defaultSetNameAttrPtr);
	I_ATTR(bool, m_serializeSelectionAttrPtr);
	I_ATTR(bool, m_allowDisabledAttrPtr);
	I_ATTR(bool, m_supportEnablingAttrPtr);
	I_ATTR(bool, m_allowEditFixedAttrPtr);
	I_ATTR(QByteArray, m_elementIndexParamId);
	I_ATTR(QByteArray, m_elementNameParamId);

	class ParamSet:
				public CMultiModelBridgeBase,
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

		// reimplemented (iser::INameParam)
		virtual const QString& GetName() const;
		virtual void SetName(const QString& name);
		virtual bool IsNameFixed() const;

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive);

		istd::TDelPtr<IParamsSet> paramSetPtr;
		QByteArray typeId;
		QString name;
		QString description;
		bool isEnabled;
		const CParamsManagerCompBase* parentPtr;
	};
	
	typedef istd::TDelPtr<ParamSet> ParamSetPtr;

	typedef QList<ParamSetPtr> ParamSets;

	ParamSets m_paramSets;
};


} // namespace iprm


#endif // !iprm_CParamsManagerCompBase_included


