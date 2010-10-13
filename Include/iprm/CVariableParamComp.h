#ifndef iprm_CVariableParamComp_included
#define iprm_CVariableParamComp_included


#include "icomp/CComponentBase.h"

#include "iprm/IVariableParam.h"


namespace iprm
{


class CVariableParamComp:
			public icomp::CComponentBase,
			virtual public IVariableParam
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CVariableParamComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(IVariableParam);
		I_ASSIGN_MULTI_0(m_factoriesFact, "Factories", "List of factories for each type", true);
		I_ASSIGN_MULTI_0(m_factoryIdsAttrPtr, "TypeIds", "List of factory ID for each factory object", true);
	I_END_COMPONENT;

	// reimplemented (iprm::IParamsSet)
	virtual TypeIds GetKnownTypeIds() const;
	virtual std::string GetParameterTypeId() const;
	virtual iser::ISerializable* GetParameter() const;
	virtual bool AssignTypeId(const std::string& typeId);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
	virtual I_DWORD GetMinimalVersion(int versionId = iser::IVersionInfo::UserVersionId) const;

private:
	I_MULTIFACT(iser::ISerializable, m_factoriesFact);
	I_MULTIATTR(istd::CString, m_factoryIdsAttrPtr);

	std::string m_paramTypeId;
	istd::TDelPtr<iser::ISerializable> m_paramPtr;
};


} // namespace iprm


#endif // !iprm_CVariableParamComp_included


