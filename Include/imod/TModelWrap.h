#ifndef imod_TModelWrap_included
#define imod_TModelWrap_included


#include "istd/TUpdateManagerWrap.h"

#include "imod/CModelBase.h"


namespace imod
{


/**
	\ingroup imod

	This model wrapper provides a simple connection between a concrete
	\c istd::IChangeable implementation and a model.
*/
template <class Base>
class TModelWrap: public istd::TUpdateManagerWrap<Base>, public imod::CModelBase
{
public:
	typedef istd::TUpdateManagerWrap<Base> BaseClass;
	typedef imod::CModelBase BaseClass2;

protected:
	// pseudo-reimplemented (istd::IChangeable)
	virtual void OnBeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);
};


// protected methods

// pseudo-reimplemented (istd::IChangeable)

template <class Base>
void TModelWrap<Base>::OnBeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass::OnBeginChanges(changeFlags, changeParamsPtr);

	NotifyBeforeUpdate(changeFlags, changeParamsPtr);
}


template <class Base>
void TModelWrap<Base>::OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass::OnEndChanges(changeFlags, changeParamsPtr);

	NotifyAfterUpdate(changeFlags, changeParamsPtr);
}


} // namespace imod


#endif // !imod_TModelWrap_included


