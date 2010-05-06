#ifndef iprm_CParamsSet_included
#define iprm_CParamsSet_included


// STL includes
#include <map>


// ACF includes
#include "istd/TChangeDelegator.h"
#include "istd/TOptDelPtr.h"
#include "istd/TPointerVector.h"

#include "imod/CMultiModelObserverBase.h"

#include "iprm/IParamsSet.h"


namespace iprm
{


/**
	Basic implementation of interface IParamsSet.
*/
class CParamsSet: virtual public IParamsSet
{
public:
	CParamsSet(const IParamsSet* slaveSetPtr = NULL);

	/**
		Get slave parameter set.
		Slave parameter set will be used for non editable parameter query.
		If no slave parameter set is defined, it returns NULL.
	*/
	const IParamsSet* GetSlaveSet() const;
	/**
		Set slave parameter set.
		Slave parameter set will be used for non editable parameter query.
		@param	slaveSetPtr		slave parameter set, or NULL, if no set is used.
	*/
	void SetSlaveSet(const IParamsSet* slaveSetPtr);

	virtual bool SetEditableParameter(const std::string& id, iser::ISerializable* parameterPtr, bool releaseFlag = false);

	// reimplemented (iprm::IParamsSet)
	virtual const iser::ISerializable* GetParameter(const std::string& id) const;
	virtual iser::ISerializable* GetEditableParameter(const std::string& id);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
	virtual I_DWORD GetMinimalVersion(int versionId = iser::IVersionInfo::UserVersionId) const;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object);

protected:
	struct ParameterInfo
	{
		ParameterInfo(const std::string& parameterId, iser::ISerializable* parameterPtr, bool releaseFlag = false)
		{
			this->parameterPtr.SetPtr(parameterPtr, releaseFlag);
			this->parameterId = parameterId;
		}

		std::string parameterId;
		istd::TOptDelPtr<iser::ISerializable> parameterPtr;
	};

	typedef istd::TPointerVector<ParameterInfo> Parameters;

	const ParameterInfo* FindParameterInfo(const std::string& parameterId) const;

private:
	/**
		Class to observe the changes of the single parameters.
	*/
	class ParamsObserver: public imod::CMultiModelObserverBase
	{
	public:
		ParamsObserver(CParamsSet& parent);

	private:
		// reimplemented (imod::IObserver)
		virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
		virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CParamsSet& m_parent;
	};

	ParamsObserver m_paramsObserver;

	Parameters m_params;

	const IParamsSet* m_slaveSetPtr;
};


// inline methods

inline const IParamsSet* CParamsSet::GetSlaveSet() const
{
	return m_slaveSetPtr;
}


inline void CParamsSet::SetSlaveSet(const IParamsSet* slaveSetPtr)
{
	m_slaveSetPtr = slaveSetPtr;
}


} // namespace iprm


#endif // !iprm_CParamsSet_included
