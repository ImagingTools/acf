#ifndef imod_TModelWrap_included
#define imod_TModelWrap_included


#include "imod/imod.h"


#include "istd/TUpdateManagerWrap.h"

#include "imod/CModelBase.h"


namespace imod
{


/**
	\ingroup imod

	This model wrapper provides a simple connection between a concrete IChangeable implementation
	and a model.
*/
template <class BaseClass>
class TModelWrap: public istd::TUpdateManagerWrap<BaseClass>, public imod::CModelBase
{
public:
	typedef istd::TUpdateManagerWrap<BaseClass> BaseClass;
	typedef imod::CModelBase BaseClass2;

protected:
	// pseudo-reimplemented (istd::IChangeable)
	virtual void OnBeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);
};


// protected methods

// pseudo-reimplemented (istd::IChangeable)

template <class BaseClass>
void TModelWrap<BaseClass>::OnBeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass::OnBeginChanges(changeFlags, changeParamsPtr);

	NotifyBeforeUpdate(changeFlags, changeParamsPtr);
}


template <class BaseClass>
void TModelWrap<BaseClass>::OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass::OnEndChanges(changeFlags, changeParamsPtr);

	NotifyAfterUpdate(changeFlags, changeParamsPtr);
}


} // namespace imod


#endif // !imod_TModelWrap_included


