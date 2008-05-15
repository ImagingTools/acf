#ifndef iqt_TGuiObserverWrap_included
#define iqt_TGuiObserverWrap_included


#include "imod/IModelEditor.h"
#include "imod/IModel.h"


namespace iqt
{


/**
	Join functionality of \c iqt::IGuiObject interface and \c imod::IObserver.
*/
template <class Gui, class Observer>
class TGuiObserverWrap: public Gui, public Observer, virtual public imod::IModelEditor
{
public:
	TGuiObserverWrap();

	// pseudo-reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

protected:
	/**
		Sets update flags for the editor. 
		GUI editor will ignore all update events, which not matches the previously set filter.
	*/
	virtual void SetUpdateFilter(int updateFlags);

	/**
		Called when model is attached and GUI is shown.
	*/
	virtual void OnGuiModelShown();

	/**
		Called when model is detached or GUI is hidden.
	*/
	virtual void OnGuiModelHidden();

	/**
		Called when model is attached and GUI is created.
	*/
	virtual void OnGuiModelAttached();

	/**
		Called when model is detached or GUI is destroyed.
	*/
	virtual void OnGuiModelDetached();

	// pseudo-reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiShown();
	virtual void OnGuiHidden();
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// pseudo-reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);

	// pseudo-reimplemented (imod::IModelEditor)
	virtual bool IsReadOnly() const;
	virtual void SetReadOnly(bool state);

private:
	bool m_ignoreUpdates;
	bool m_isReadOnly;
	int m_updateFilter;
};


// public methods

template <class Gui, class Observer>
TGuiObserverWrap<Gui, Observer>::TGuiObserverWrap()
:	m_ignoreUpdates(false), m_isReadOnly(false), m_updateFilter(0)
{
}


// pseudo-reimplemented (imod::IObserver)

template <class Gui, class Observer>
bool TGuiObserverWrap<Gui, Observer>::OnAttached(imod::IModel* modelPtr)
{
	bool retVal = Observer::OnAttached(modelPtr);

	if (retVal && IsGuiCreated() && IsModelAttached(NULL)){
		OnGuiModelAttached();
	}

	return retVal;
}


template <class Gui, class Observer>
bool TGuiObserverWrap<Gui, Observer>::OnDetached(imod::IModel* modelPtr)
{
	if (!m_isReadOnly && IsModelAttached(modelPtr)){
		UpdateModel();
	}

	bool retVal = Observer::OnDetached(modelPtr);

	if (retVal && IsGuiCreated() && !IsModelAttached(NULL)){
		OnGuiModelDetached();
	}

	return retVal;
}


// protected methods

template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::SetUpdateFilter(int updateFlags)
{
	m_updateFilter = updateFlags;
}


template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::OnGuiModelShown()
{
}


template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::OnGuiModelHidden()
{
}


template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::OnGuiModelAttached()
{
	I_ASSERT(IsGuiCreated());
	I_ASSERT(IsModelAttached(NULL));

	if (!m_ignoreUpdates){
		m_ignoreUpdates = true;

		UpdateEditor();

		m_ignoreUpdates = false;
	}
}


template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::OnGuiModelDetached()
{
	if (!m_isReadOnly && IsModelAttached(NULL)){
		UpdateModel();
	}
}


// pseudo-reimplemented (iqt::CGuiComponentBase)

template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::OnGuiShown()
{
	Gui::OnGuiShown();

	if (IsModelAttached(NULL)){
		OnGuiModelShown();
	}
}


template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::OnGuiHidden()
{
	Gui::OnGuiHidden();

	if (IsModelAttached(NULL)){
		OnGuiModelHidden();
	}
}


template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::OnGuiCreated()
{
	Gui::OnGuiCreated();

	if (IsModelAttached(NULL)){
		OnGuiModelAttached();
	}
}


template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::OnGuiDestroyed()
{
	if (IsModelAttached(NULL)){
		OnGuiModelDetached();
	}

	Gui::OnGuiDestroyed();
}


// pseudo-reimplemented (imod::IObserver)

template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	I_ASSERT(modelPtr != NULL);
	I_ASSERT(IsModelAttached(modelPtr));

	Observer::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);

	bool skipUpdate = false;
	if (m_updateFilter){
		skipUpdate = ((m_updateFilter & updateFlags) == 0);
	}

	if (!m_ignoreUpdates && IsGuiCreated() && !skipUpdate){
		m_ignoreUpdates = true;

		UpdateEditor();

		m_ignoreUpdates = false;
	}
}


// pseudo-reimplemented (imod::IModelEditor)

template <class Gui, class Observer>
bool TGuiObserverWrap<Gui, Observer>::IsReadOnly() const
{
	return m_isReadOnly;
}


template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::SetReadOnly(bool state)
{
	m_isReadOnly = state;
}


} // namespace iqt


#endif //!iqt_TGuiObserverWrap_included


