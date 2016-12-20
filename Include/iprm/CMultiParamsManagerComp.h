#ifndef iprm_CMultiParamsManagerComp_included
#define iprm_CMultiParamsManagerComp_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include <iprm/CParamsManagerCompBase.h>


namespace iprm
{


/**
	Implementation of parameter manager.
*/
class CMultiParamsManagerComp: public CParamsManagerCompBase
{
public:
	typedef CParamsManagerCompBase BaseClass;

	I_BEGIN_COMPONENT(CMultiParamsManagerComp);
		I_ASSIGN_MULTI_0(m_paramSetsFactoriesPtr, "ParamSetsFactories", "List of factories for parameter set creation", true);		
		I_ASSIGN_MULTI_0(m_factoryNameNameAttrPtr, "ParamSetsFactorieNames", "List of names associated with the parameter factories", true);
		I_ASSIGN_MULTI_0(m_factoryDescriptionAttrPtr, "ParamSetsFactorieDesciption", "List of descriptions associated with the parameter factories", true);
	I_END_COMPONENT;

	// reimplemented (iprm::IParamsManager)
	virtual const IOptionsList* GetParamsTypeConstraints() const;

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

protected:
	struct TypeInfo
	{
		int factoryIndex;
		QByteArray id;
		QString name;
		QString description;
	};

	struct TypeInfoList: public IOptionsList
	{
		// reimplemented (iprm::IOptionsList)
		virtual int GetOptionsFlags() const;
		virtual int GetOptionsCount() const;
		virtual QString GetOptionName(int index) const;
		virtual QString GetOptionDescription(int index) const;
		virtual QByteArray GetOptionId(int index) const;
		virtual bool IsOptionEnabled(int index) const;

		QVector<TypeInfo> typeInfos;

		QMap<QByteArray, int> typeIdToIndexMap;
	};

	/**
		Ensure some parameter exist, has correct type and attributes. 
		\param	index		position in list of parameters.
		\param	typeid		ID of parameter type.
		\param	name		name of parameter (attribute).
		\param	isEnabled	enebling status of parameter (attribute).
		\return			\c true if element exists (was present or could be created).
	*/
	bool EnsureParamExist(int index, const QByteArray& typeId, const QString& name, bool isEnabled);

	// reimplemented (CParamsManagerCompBase)
	virtual bool IsParameterCreationSupported() const;
	virtual int GetCreatedParamsSetsCount() const;
	virtual iprm::IParamsSet* CreateParamsSetInstance(int typeIndex = -1) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	TypeInfoList m_typeInfoList;

private:
	I_MULTIFACT(iprm::IParamsSet, m_paramSetsFactoriesPtr);
	I_MULTITEXTATTR(m_factoryNameNameAttrPtr);
	I_MULTITEXTATTR(m_factoryDescriptionAttrPtr);
};


} // namespace iprm


#endif // !iprm_CMultiParamsManagerComp_included


