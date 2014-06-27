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
	virtual istd::IChangeable* CloneMe(istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) const;
	virtual void BeginChanges(const istd::IChangeable::ChangeSet& changeSet);
	virtual void EndChanges(const istd::IChangeable::ChangeSet& changeSet);
	virtual void BeginChangeGroup(const istd::IChangeable::ChangeSet& changeSet);
	virtual void EndChangeGroup(const istd::IChangeable::ChangeSet& changeSet);

protected:
	// reimplemented (imod::CModelBase)
	virtual void OnBeginGlobalChanges();
	virtual void OnEndGlobalChanges(const istd::IChangeable::ChangeSet& changeSet);
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

	return baseOperations | istd::IChangeable::SO_CLONE | istd::IChangeable::SO_OBSERVE;
}


template <class Base>
istd::IChangeable* TModelWrap<Base>::CloneMe(istd::IChangeable::CompatibilityMode mode) const
{
	istd::TDelPtr< TModelWrap<Base> > clonePtr(new TModelWrap<Base>());

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


template <class Base>
void TModelWrap<Base>::BeginChanges(const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass2::NotifyBeforeChange(changeSet, false);
}


template <class Base>
void TModelWrap<Base>::EndChanges(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	BaseClass2::NotifyAfterChange();
}


template <class Base>
void TModelWrap<Base>::BeginChangeGroup(const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass2::NotifyBeforeChange(changeSet, true);
}


template <class Base>
void TModelWrap<Base>::EndChangeGroup(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	BaseClass2::NotifyAfterChange();
}


// protected methods

// reimplemented (imod::CModelBase)

template <class Base>
void TModelWrap<Base>::OnBeginGlobalChanges()
{
	BaseClass::OnBeginChanges();
}


template <class Base>
void TModelWrap<Base>::OnEndGlobalChanges(const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass::OnEndChanges(changeSet);
}


} // namespace imod


#endif // !imod_TModelWrap_included


