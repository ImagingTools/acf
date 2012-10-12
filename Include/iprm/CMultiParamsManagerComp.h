#ifndef iprm_CMultiParamsManagerComp_included
#define iprm_CMultiParamsManagerComp_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "istd/TSmartPtr.h"

#include "imod/CMultiModelBridgeBase.h"

#include "icomp/CComponentBase.h"

#include "iprm/IParamsSet.h"
#include "iprm/ISelectionParam.h"
#include "iprm/ISelectionConstraints.h"
#include "iprm/IParamsManager.h"


namespace iprm
{


/**
	Implementation of parameter manager.
*/
class CMultiParamsManagerComp:
			public icomp::CComponentBase,
			virtual public IParamsManager,
			virtual public ISelectionConstraints,
			protected imod::CMultiModelBridgeBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CMultiParamsManagerComp);
		I_REGISTER_INTERFACE(ISelectionParam);
		I_REGISTER_INTERFACE(IParamsManager);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(ISelectionConstraints);
		I_ASSIGN_MULTI_0(m_fixedParamSetsCompPtr, "FixedParamSets", "List of references to fixed parameter set", false);
		I_ASSIGN_MULTI_0(m_fixedSetNamesAttrPtr, "FixedSetNames", "List of fixed parameter names", false);
		I_ASSIGN(m_defaultSetNameAttrPtr, "DefaultSetName", "Default name of parameter set", true, "<noname>");		
		I_ASSIGN_MULTI_0(m_paramSetsFactoriesPtr, "ParamSetsFactories", "List of factories for parameter sets creation", true);		
		I_ASSIGN_MULTI_0(m_factoryNameNameAttrPtr, "ParamSetsFactorieNames", "List of names associated with the parameter factories", true);
		I_ASSIGN_MULTI_0(m_factoryDescriptionAttrPtr, "ParamSetsFactorieDesciption", "List of descriptions associated with the parameter factories", true);
	I_END_COMPONENT;

	CMultiParamsManagerComp();	

	// reimplemented (iprm::IParamsManager)
	virtual int GetIndexOperationFlags(int index = -1) const;
	virtual int GetParamsSetsCount() const;
	virtual const ISelectionConstraints* GetParamsTypeConstraints() const;
	virtual int InsertParamsSet(int typeIndex = -1, int index = -1);
	virtual bool RemoveParamsSet(int index);
	virtual bool SwapParamsSet(int index1, int index2);
	virtual IParamsSet* GetParamsSet(int index) const;
	virtual QString GetParamsSetName(int index) const;
	virtual bool SetParamsSetName(int index, const QString& name);

	// reimplemented (iprm::ISelectionParam)
	virtual const ISelectionConstraints* GetSelectionConstraints() const;
	virtual int GetSelectedOptionIndex() const;
	virtual bool SetSelectedOptionIndex(int index);
	virtual ISelectionParam* GetActiveSubselection() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (iprm::ISelectionConstraints)
	virtual int GetConstraintsFlags() const;
	virtual int GetOptionsCount() const;
	virtual QString GetOptionName(int index) const;
	virtual QString GetOptionDescription(int index) const;
	virtual QByteArray GetOptionId(int index) const;

protected:
	struct TypeInfo
	{
		int factoryIndex;
		QByteArray id;
		QString name;
		QString description;
	};

	struct TypeInfoList: public ISelectionConstraints
	{
		// reimplemented (iprm::ISelectionConstraints)
		virtual int GetConstraintsFlags() const;
		virtual int GetOptionsCount() const;
		virtual QString GetOptionName(int index) const;
		virtual QString GetOptionDescription(int index) const;
		virtual QByteArray GetOptionId(int index) const;

		QVector<TypeInfo> typeInfos;

		QMap<QByteArray, int> typeIdToIndexMap;
	};

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	/**
		Ensure some parameter exist and has correct type. 
		\param	index	position in list of parameters.
		\param	typeid	ID of parameter type.
		\return			\c true - deserialization was successfull or \c false - no deserialization was possible.
	*/
	bool EnsureParamExist(const QByteArray& typeId, int index, const QString& name);

private:
	I_MULTIREF(IParamsSet, m_fixedParamSetsCompPtr);
	I_MULTIATTR(QString, m_fixedSetNamesAttrPtr);
	I_ATTR(QString, m_defaultSetNameAttrPtr);	
	I_MULTIFACT(iprm::IParamsSet, m_paramSetsFactoriesPtr);
	I_MULTIATTR(QString, m_factoryNameNameAttrPtr);	
	I_MULTIATTR(QString, m_factoryDescriptionAttrPtr);	

	struct ParamSet
	{
		istd::TSmartPtr<IParamsSet> paramSetPtr;
		QString name;
		QByteArray typeId;
	};

	typedef QList<ParamSet> ParamSets;

	ParamSets m_paramSets;

	int m_selectedIndex;

	TypeInfoList m_typeInfoList;
};


} // namespace iprm


#endif // !iprm_CMultiParamsManagerComp_included


