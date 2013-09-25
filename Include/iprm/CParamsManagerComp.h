#ifndef iprm_CParamsManagerComp_included
#define iprm_CParamsManagerComp_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "istd/TDelPtr.h"
#include "imod/TModelWrap.h"
#include "imod/CMultiModelBridgeBase.h"
#include "icomp/CComponentBase.h"
#include "iprm/IParamsSet.h"
#include "iprm/ISelectionParam.h"
#include "iprm/IOptionsManager.h"
#include "iprm/INameParam.h"
#include "iprm/IParamsManager.h"


namespace iprm
{


/**
	Implementation of parameter manager.
*/
class CParamsManagerComp:
			public icomp::CComponentBase,
			virtual public IParamsManager,
			virtual public IOptionsManager,
			protected imod::CMultiModelBridgeBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CParamsManagerComp);
		I_REGISTER_INTERFACE(ISelectionParam);
		I_REGISTER_INTERFACE(IParamsManager);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(IOptionsManager);
		I_REGISTER_INTERFACE(IOptionsList);
		I_ASSIGN_MULTI_0(m_fixedParamSetsCompPtr, "FixedParamSets", "List of references to fixed parameter set", false);
		I_ASSIGN_MULTI_0(m_fixedSetNamesAttrPtr, "FixedSetNames", "List of fixed parameter names", false);
		I_ASSIGN(m_defaultSetNameAttrPtr, "DefaultSetName", "Default name of parameter set. Use %1 to insert automatic enumeration", true, "unnamed_%1");
		I_ASSIGN(m_elementIndexParamId, "ElementIndexParamId", "ID of index of returned parameter set in manager list", false, "Index");
		I_ASSIGN(m_elementNameParamId, "ElementNameParamId", "ID of the name of returned parameter set in manager list", false, "Name");
		I_ASSIGN(m_paramSetsFactPtr, "ParamsSetFactory", "Factory of variable parameter set", false, "ParamsSet");
		I_ASSIGN(m_serializeSelectionAttrPtr, "SerializeSelection", "If enabled, the current parameter set selection will be serialized", true, true);
		I_ASSIGN(m_allowDisabledAttrPtr, "AllowDisabled", "Control if disabled parameters are supported", true, false);
		I_ASSIGN(m_supportEnablingAttrPtr, "SupportEnabling", "Control if enabling or disabling of parameters is allowed (works only if disabled parameters are supported)", true, true);
	I_END_COMPONENT;

	CParamsManagerComp();

	bool SetSetsCount(int count);

	// reimplemented (iprm::IParamsManager)
	virtual int GetIndexOperationFlags(int index = -1) const;
	virtual int GetParamsSetsCount() const;
	virtual const IOptionsList* GetParamsTypeConstraints() const;
	virtual int InsertParamsSet(int typeIndex = -1, int index = -1);
	virtual bool RemoveParamsSet(int index);
	virtual bool SwapParamsSet(int index1, int index2);
	virtual IParamsSet* GetParamsSet(int index) const;
	virtual QString GetParamsSetName(int index) const;
	virtual bool SetParamsSetName(int index, const QString& name);

	// reimplemented (iprm::ISelectionParam)
	virtual const IOptionsList* GetSelectionConstraints() const;
	virtual int GetSelectedOptionIndex() const;
	virtual bool SetSelectedOptionIndex(int index);
	virtual ISelectionParam* GetSubselection(int index) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (iprm::IOptionsManager)
	virtual int GetOptionOperationFlags(int index = -1) const;
	virtual bool SetOptionEnabled(int index, bool isEnabled = true);
	virtual bool RemoveOption(int index);
	virtual bool InsertOption(
				const QString& optionName,
				const QByteArray& optionId,
				const QString& optionDescription = QString(),
				int index = -1);
	virtual bool SwapOptions(int index1, int index2);
	virtual bool SetOptionName(int optionIndex, const QString& optionName);
	virtual bool SetOptionDescription(int optionIndex, const QString& optionDescription);

	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const;
	virtual int GetOptionsCount() const;
	virtual QString GetOptionName(int index) const;
	virtual QString GetOptionDescription(int index) const;
	virtual QByteArray GetOptionId(int index) const;
	virtual bool IsOptionEnabled(int index) const;

protected:
	QString GetNewSetName() const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_MULTIREF(IParamsSet, m_fixedParamSetsCompPtr);
	I_MULTIATTR(QString, m_fixedSetNamesAttrPtr);
	I_ATTR(QString, m_defaultSetNameAttrPtr);
	I_ATTR(QByteArray, m_elementIndexParamId);
	I_ATTR(QByteArray, m_elementNameParamId);
	I_FACT(IParamsSet, m_paramSetsFactPtr);
	I_ATTR(bool, m_serializeSelectionAttrPtr);
	I_ATTR(bool, m_allowDisabledAttrPtr);
	I_ATTR(bool, m_supportEnablingAttrPtr);

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
		QString name;
		bool isEnabled;
		const CParamsManagerComp* parentPtr;
	};
	
	int FindParamSetIndex(const QString& name) const;

	typedef istd::TDelPtr<ParamSet> ParamSetPtr;

	typedef QList<ParamSetPtr> ParamSets;

	ParamSets m_paramSets;

	int m_selectedIndex;
};


} // namespace iprm


#endif // !iprm_CParamsManagerComp_included


