#ifndef iprm_CParamsSet_included
#define iprm_CParamsSet_included


#include <map>

#include "imod/CMultiModelObserverBase.h"

#include "iprm/IParamsSet.h"

#include "icomp/CComponentBase.h"


namespace iprm
{


/**
	Implementation of interface IParamsSet as component.
	This implementation allows to register list of objects as editable parameters and list of slave parameter sets.
*/
class CParamsSet: virtual public IParamsSet, private imod::CMultiModelObserverBase
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::CMultiModelObserverBase BaseClass2;

	CParamsSet(const IParamsSet* slaveSetPtr = NULL);

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
	// reimplemented (imod::IObserver)
	virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);

	ParamsMap m_paramsMap;

	const IParamsSet* m_slaveSetPtr;
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
