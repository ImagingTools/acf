#ifndef iprm_CComposedParamsSetComp_included
#define iprm_CComposedParamsSetComp_included


#include "istd/TIHierarchical.h"

#include "icomp/CComponentBase.h"

#include "iprm/CParamsSet.h"


namespace iprm
{


/**
	Implementation of interface IParamsSet as component.
	This implementation allows to register list of objects as editable parameters and list of slave parameter sets.
*/
class CComposedParamsSetComp:
			public icomp::CComponentBase,
			public CParamsSet,
			virtual public istd::IHierarchical
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef CParamsSet BaseClass2;

	I_BEGIN_COMPONENT(CComposedParamsSetComp);
		I_REGISTER_INTERFACE(istd::IHierarchical);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(IParamsSet);
		I_ASSIGN_MULTI_0(m_slaveParamsCompPtr, "SlaveSets", "List of slave parameter sets", false);
		I_ASSIGN_MULTI_0(m_parametersCompPtr, "Parameters", "Parameters", true);
		I_ASSIGN_MULTI_0(m_parametersIdAttrPtr, "ParametersId", "ID of each paremeter in 'Parameters'", true);
	I_END_COMPONENT;

	// reimplemented (iprm::IParamsSet)
	virtual Ids GetParamIds(bool editableOnly = false) const;
	virtual const iser::ISerializable* GetParameter(const QByteArray& id) const;

	// reimplemented (istd::IHierarchical)
	virtual int GetHierarchicalFlags() const;
	virtual int GetChildsCount() const;
	virtual istd::IPolymorphic* GetChild(int index) const;
	virtual istd::IPolymorphic* GetParent() const;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object);

protected:
	iprm::IParamsSet* GetSlaveParamsSet(int index) const;
	int GetSlaveParamsSetCount() const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_MULTIREF(IParamsSet, m_slaveParamsCompPtr);
	I_MULTIREF(iser::ISerializable, m_parametersCompPtr);
	I_MULTIATTR(QByteArray, m_parametersIdAttrPtr);
};


} // namespace iprm


#endif // !iprm_CComposedParamsSetComp_included
