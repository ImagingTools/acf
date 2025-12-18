#pragma once


#include <istd/TDelPtr.h>
#include <istd/IChangeable.h>
#include <iser/TCopySerializedWrap.h>
#include <imod/CModelBase.h>


namespace imod
{


/**
	This model wrapper provides a simple connection between a concrete
	\c istd::IChangeable implementation and a model.

	\ingroup DataModel
	\ingroup ModelObserver
*/
template <class Base>
class TModelWrap: public iser::TCopySerializedWrap<Base>, public imod::CModelBase
{
public:
	typedef iser::TCopySerializedWrap<Base> BaseClass;
	typedef imod::CModelBase BaseClass2;

	TModelWrap();

	void SetBaseObject(const Base& baseObject);

	// pseudo-reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) const override;
	virtual void BeginChanges(const istd::IChangeable::ChangeSet& changeSet) override;
	virtual void EndChanges(const istd::IChangeable::ChangeSet& changeSet) override;
	virtual void BeginChangeGroup(const istd::IChangeable::ChangeSet& changeSet) override;
	virtual void EndChangeGroup(const istd::IChangeable::ChangeSet& changeSet) override;

protected:
	// reimplemented (imod::CModelBase)
	virtual void OnBeginGlobalChanges() override;
	virtual void OnEndGlobalChanges(const istd::IChangeable::ChangeSet& changeSet) override;

private:
	Q_DISABLE_COPY(TModelWrap);
};


// public methods

template <class Base>
TModelWrap<Base>::TModelWrap()
{
}


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
istd::IChangeableUniquePtr TModelWrap<Base>::CloneMe(istd::IChangeable::CompatibilityMode mode) const
{
	istd::IChangeableUniquePtr clonePtr(new TModelWrap<Base>());

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr;
	}

	return istd::IChangeableUniquePtr();
}


template <class Base>
void TModelWrap<Base>::BeginChanges(const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass2::NotifyBeforeChange(changeSet, false);
}


template <class Base>
void TModelWrap<Base>::EndChanges(const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass2::NotifyAfterChange(changeSet);
}


template <class Base>
void TModelWrap<Base>::BeginChangeGroup(const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass2::NotifyBeforeChange(changeSet, true);
}


template <class Base>
void TModelWrap<Base>::EndChangeGroup(const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass2::NotifyAfterChange(changeSet);
}


// protected methods

// reimplemented (imod::CModelBase)

template <class Base>
void TModelWrap<Base>::OnBeginGlobalChanges()
{
	this->OnBeginChanges();
}


template <class Base>
void TModelWrap<Base>::OnEndGlobalChanges(const istd::IChangeable::ChangeSet& changeSet)
{
	this->OnEndChanges(changeSet);
}


} // namespace imod


