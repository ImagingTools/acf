#ifndef iprm_CParamsManagerComp_included
#define iprm_CParamsManagerComp_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "istd/TSmartPtr.h"

#include "imod/CMultiModelBridgeBase.h"

#include "icomp/CComponentBase.h"

#include "iprm/IParamsSet.h"
#include "iprm/ISelectionParam.h"
#include "iprm/IOptionsList.h"
#include "iprm/IParamsManager.h"


namespace iprm
{


/**
	Implementation of parameter manager.
*/
class CParamsManagerComp:
			public icomp::CComponentBase,
			virtual public IParamsManager,
			virtual public IOptionsList,
			protected imod::CMultiModelBridgeBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CParamsManagerComp);
		I_REGISTER_INTERFACE(ISelectionParam);
		I_REGISTER_INTERFACE(IParamsManager);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(IOptionsList);
		I_ASSIGN_MULTI_0(m_fixedParamSetsCompPtr, "FixedParamSets", "List of references to fixed parameter set", false);
		I_ASSIGN_MULTI_0(m_fixedSetNamesAttrPtr, "FixedSetNames", "List of fixed parameter names", false);
		I_ASSIGN(m_defaultSetNameAttrPtr, "DefaultSetName", "Default name of parameter set. Use %1 to insert automatic enumeration", true, "<noname>");
		I_ASSIGN(m_paramsSetTypeIdAttrPtr, "ParamsSetTypeId", "ID of factorisied parameter set", true, "Default");
		I_ASSIGN(m_paramSetsFactPtr, "ParamsSetFactory", "Factory of variable parameter set", false, "ParamsSet");
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

	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const;
	virtual int GetOptionsCount() const;
	virtual QString GetOptionName(int index) const;
	virtual QString GetOptionDescription(int index) const;
	virtual QByteArray GetOptionId(int index) const;
	virtual bool IsOptionEnabled(int index) const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_MULTIREF(IParamsSet, m_fixedParamSetsCompPtr);
	I_MULTIATTR(QString, m_fixedSetNamesAttrPtr);
	I_ATTR(QString, m_defaultSetNameAttrPtr);
	I_ATTR(QByteArray, m_paramsSetTypeIdAttrPtr);
	I_FACT(IParamsSet, m_paramSetsFactPtr);

	struct ParamSet
	{
		istd::TSmartPtr<IParamsSet> paramSetPtr;
		QString name;
	};
	
	int FindParamSetIndex(const QString& name) const;

	typedef QList<ParamSet> ParamSets;

	ParamSets m_paramSets;

	int m_selectedIndex;
};


} // namespace iprm


#endif // !iprm_CParamsManagerComp_included


