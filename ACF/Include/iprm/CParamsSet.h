#ifndef iprm_CParamsSet_included
#define iprm_CParamsSet_included


#include <map>

#include "iprm/IParamsSet.h"

#include "icomp/CComponentBase.h"


namespace iprm
{


/**
	Implementation of interface IParamsSet as component.
	This implementation allows to register list of objects as editable parameters and list of slave parameter sets.
*/
class CParamsSet: virtual public IParamsSet
{
public:
	typedef icomp::CComponentBase BaseClass;

	CParamsSet(const IParamsSet* parentSetPtr = NULL);

	virtual bool SetEditableParameter(const std::string& id, iser::ISerializable* parameterPtr);

	// reimplemented (iprm::IParamsSet)
	virtual const iser::ISerializable* GetParameter(const std::string& id) const;
	virtual iser::ISerializable* GetEditableParameter(const std::string& id);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
	virtual I_DWORD GetMinimalVersion(int versionId = iser::IVersionInfo::UserVersionId) const;

protected:
	typedef std::map<std::string, iser::ISerializable*> ParamsMap;

	const ParamsMap& GetParamsMap() const;
	ParamsMap& GetParamsMapRef();

private:
	ParamsMap m_paramsMap;

	const IParamsSet* m_parentSetPtr;
};


// inline methods

// protected methods

inline const CParamsSet::ParamsMap& CParamsSet::GetParamsMap() const
{
	return m_paramsMap;
}


inline CParamsSet::ParamsMap& CParamsSet::GetParamsMapRef()
{
	return m_paramsMap;
}


} // namespace iprm


#endif // !iprm_CParamsSet_included
