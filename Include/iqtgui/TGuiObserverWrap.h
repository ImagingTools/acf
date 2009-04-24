#ifndef iqtgui_TGuiObserverWrap_included
#define iqtgui_TGuiObserverWrap_included


#include "imod/IModelEditor.h"
#include "imod/IModel.h"


namespace iqtgui
{


/**
	Join functionality of \c iqtgui::IGuiObject interface and \c imod::IObserver.
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
	class UpdateBlocker
	{
	public:
		UpdateBlocker(TGuiObserverWrap<Gui, Observer>* parentPtr);
		~UpdateBlocker();

	private:
		TGuiObserverWrap<Gui, Observer>& m_parent;
	};

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

	/**
		Check if GUI updating is blocked.
	*/
	bool IsUpdateBlocked() const;

	// pseudo-reimplemented (iqtgui::CGuiComponentBase)
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
	int m_ignoreUpdatesCounter;
	bool m_isReadOnly;
	int m_updateFilter;
};


// public methods

template <class Gui, class Observer>
TGuiObserverWrap<Gui, Observer>::TGuiObserverWrap()
:	m_ignoreUpdatesCounter(0), m_isReadOnly(false), m_updateFilter(0)
{
}


// pseudo-reimplemented (imod::IObserver)

template <class Gui, class Observer>
bool TGuiObserverWrap<Gui, Observer>::OnAttached(imod::IModel* modelPtr)
{
	bool retVal = Observer::OnAttached(modelPtr);

	if (retVal && Gui::IsGuiCreated() && Observer::IsModelAttached(NULL)){
		OnGuiModelAttached();
	}

	return retVal;
}


template <class Gui, class Observer>
bool TGuiObserverWrap<Gui, Observer>::OnDetached(imod::IModel* modelPtr)
{
	if (Observer::IsModelAttached(modelPtr)){
		if (!m_isReadOnly){
			UpdateModel();
		}

		if (Gui::IsGuiCreated()){
			OnGuiModelDetached();
		}
	}

	return Observer::OnDetached(modelPtr);
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
	I_ASSERT(Gui::IsGuiCreated());
	I_ASSERT(Observer::IsModelAttached(NULL));

	if (!IsUpdateBlocked()){
		UpdateBlocker blocker(this);

		UpdateEditor();
	}
}


template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::OnGuiModelDetached()
{
	if (!m_isReadOnly && Observer::IsModelAttached(NULL)){
		UpdateModel();
	}
}


template <class Gui, class Observer>
bool TGuiObserverWrap<Gui, Observer>::IsUpdateBlocked() const
{
	return (m_ignoreUpdatesCounter > 0);
}


// pseudo-reimplemented (iqtgui::CGuiComponentBase)

template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::OnGuiShown()
{
	Gui::OnGuiShown();

	if (Observer::IsModelAttached(NULL)){
		OnGuiModelShown();
	}
}


template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::OnGuiHidden()
{
	Gui::OnGuiHidden();

	if (Observer::IsModelAttached(NULL)){
		OnGuiModelHidden();
	}
}


template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::OnGuiCreated()
{
	Gui::OnGuiCreated();

	if (Observer::IsModelAttached(NULL)){
		OnGuiModelAttached();
	}
}


template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::OnGuiDestroyed()
{
	if (Observer::IsModelAttached(NULL)){
		OnGuiModelDetached();
	}

	Gui::OnGuiDestroyed();
}


// pseudo-reimplemented (imod::IObserver)

template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	I_ASSERT(modelPtr != NULL);
	I_ASSERT(Observer::IsModelAttached(modelPtr));

	Observer::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);

	bool skipUpdate = false;
	if ((m_updateFilter != 0) && (updateFlags != 0)){
		skipUpdate = ((m_updateFilter & updateFlags) == 0);
	}

	if (!IsUpdateBlocked() && Gui::IsGuiCreated() && !skipUpdate){
		UpdateBlocker blocker(this);

		UpdateEditor(updateFlags);
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


// public methods of embedded class UpdateBlocker

template <class Gui, class Observer>
TGuiObserverWrap<Gui, Observer>::UpdateBlocker::UpdateBlocker(TGuiObserverWrap<Gui, Observer>* parentPtr)
:	m_parent(*parentPtr)
{
	++m_parent.m_ignoreUpdatesCounter;
}


template <class Gui, class Observer>
TGuiObserverWrap<Gui, Observer>::UpdateBlocker::~UpdateBlocker()
{
	--m_parent.m_ignoreUpdatesCounter;
}


} // namespace iqtgui


#endif // !iqtgui_TGuiObserverWrap_included


