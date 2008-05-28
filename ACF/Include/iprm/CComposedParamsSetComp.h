#ifndef iprm_CComposedParamsSetComp_included
#define iprm_CComposedParamsSetComp_included


#include "iprm/IParamsSet.h"

#include "icomp/CComponentBase.h"


namespace iprm
{


/**
	Implementation of interface IParamsSet as component.
	This implementation allows to register list of objects as editable parameters and list of slave parameter sets.
*/
class CComposedParamsSetComp: public icomp::CComponentBase, virtual public IParamsSet
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CComposedParamsSetComp)
		I_REGISTER_INTERFACE(IParamsSet)
		I_ASSIGN_MULTI_0(m_slaveParamsCompPtr, "SlaveSets", "List of slave parameter sets", true)
		I_ASSIGN_MULTI_0(m_parametersCompPtr, "Parameters", "Parameters", true)
		I_ASSIGN_MULTI_0(m_parametersIdAttrPtr, "ParametersId", "ID of each paremeter in 'Parameters'", true)
	I_END_COMPONENT

	// reimplemented (IParamsSet)
	virtual const iser::ISerializable* GetParameter(const std::string& id) const;
	virtual iser::ISerializable* GetEditableParameter(const std::string& id);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
	virtual I_DWORD GetMinimalVersion(int versionId = iser::IVersionInfo::UserVersionId) const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

private:
	I_MULTIREF(IParamsSet, m_slaveParamsCompPtr);
	I_MULTIREF(iser::ISerializable, m_parametersCompPtr);
	I_MULTIATTR(istd::CString, m_parametersIdAttrPtr);

	typedef std::map<std::string, iser::ISerializable*> ParamsMap;

	ParamsMap m_paramsMap;
};


} // namespace iprm


#endif // !iprm_CComposedParamsSetComp_included
