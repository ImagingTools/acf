#ifndef imod_TModelWrap_included
#define imod_TModelWrap_included


#include "istd/TDelPtr.h"
#include "istd/IChangeable.h"

#include "iser/TCopySerializedWrap.h"

#include "imod/CModelBase.h"


namespace imod
{


/**
	This model wrapper provides a simple connection between a concrete
	\c istd::IChangeable implementation and a model.

	\ingroup ModelObserver
	\ingroup Helpers
*/
template <class Base>
class TModelWrap: public iser::TCopySerializedWrap<Base>, public imod::CModelBase
{
public:
	typedef iser::TCopySerializedWrap<Base> BaseClass;
	typedef imod::CModelBase BaseClass2;

	void SetBaseObject(const Base& baseObject);

	// pseudo-reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const;
	virtual istd::IChangeable* CloneMe() const;

protected:
	// pseudo-reimplemented (istd::IChangeable)
	virtual void OnBeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);
};


// public methods

template <class Base>
void TModelWrap<Base>::SetBaseObject(const Base& baseObject)
{
	Base::operator=(baseObject);
}


// pseudo-reimplemented (istd::IChangeable)

template <class Base>
int TModelWrap<Base>::GetSupportedOperations() const
{
	int baseOperations = Base::GetSupportedOperations();

	if ((baseOperations & istd::IChangeable::SO_COPY) != 0){
		return baseOperations |  istd::IChangeable::SO_CLONE |  istd::IChangeable::SO_OBSERVE;
	}
	else{
		return baseOperations |  istd::IChangeable::SO_OBSERVE;
	}
}


template <class Base>
istd::IChangeable* TModelWrap<Base>::CloneMe() const
{
	istd::TDelPtr< TModelWrap<Base> > clonePtr(new TModelWrap<Base>());

	if (clonePtr->CopyFrom(*this)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


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


