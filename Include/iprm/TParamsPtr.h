#ifndef iprm_TParamsPtr_included
#define iprm_TParamsPtr_included


#include "iprm/IParamsSet.h"


namespace iprm
{


/**
	Help pointer wrapper for management of a parameter from the parameter set.
*/
template <class ParameterInterace>
class TParamsPtr: public istd::TPointerBase<ParameterInterace>
{
public:
	typedef istd::TPointerBase<ParameterInterace> BaseClass;

	TParamsPtr();

	TParamsPtr(const IParamsSet* parameterSetPtr, const QByteArray& parameterId);

	/**
		Initialize the pointer with the given parameter set and parameter ID.
	*/
	void Init(const IParamsSet* parameterSetPtr, const QByteArray& parameterId);
};


// public methods

template <class ParameterInterace>
TParamsPtr<ParameterInterace>::TParamsPtr()
	:BaseClass()
{
}


template <class ParameterInterace>
TParamsPtr<ParameterInterace>::TParamsPtr(const IParamsSet* parameterSetPtr, const QByteArray& parameterId)
{
	Init(parameterSetPtr, parameterId);
}


template <class ParameterInterace>
void TParamsPtr<ParameterInterace>::Init(const IParamsSet* parameterSetPtr, const QByteArray& parameterId)
{
	BaseClass::Reset();

	if (parameterSetPtr != NULL && !parameterId.isEmpty()){
		const ParameterInterace* parameterPtr = dynamic_cast<const ParameterInterace*>(parameterSetPtr->GetParameter(parameterId));
		if (parameterPtr != NULL){
			BaseClass::SetPtr(const_cast<ParameterInterace*>(parameterPtr));
		}
	}
}


} // namespace iprm


#endif // !iprm_TParamsPtr_included


