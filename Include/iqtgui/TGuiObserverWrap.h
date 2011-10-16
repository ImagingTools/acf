#ifndef iqtgui_TGuiObserverWrap_included
#define iqtgui_TGuiObserverWrap_included


// ACF includes
#include "imod/IModelEditor.h"
#include "imod/IModel.h"


namespace iqtgui
{


/**
	Join functionality of \c iqtgui::IGuiObject interface and \c imod::IObserver.

	\ingroup Helpers
*/
template <class Gui, class Observer>
class TGuiObserverWrap:
			public Gui,
			public Observer,
			virtual public imod::IModelEditor
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

	/**
		Do update of the model GUI.
	*/
	virtual void UpdateGui(int updateFlags);

	// reimplemented (imod::IModelEditor)
	virtual void UpdateEditor(int updateFlags = 0);
	virtual void UpdateModel() const;

	// pseudo-reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiShown();
	virtual void OnGuiHidden();
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// pseudo-reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// pseudo-reimplemented (imod::IModelEditor)
	virtual bool IsReadOnly() const;
	virtual void SetReadOnly(bool state);

private:
	void DoPostponedUpdate(int updateFlags);
	void DoUpdate(int updateFlags);

private:
	int m_ignoreUpdatesCounter;
	bool m_isReadOnly;
	int m_updateFilter;

	/**
		Do editor update, if the gui change its state to visible.
	*/
	bool m_updateOnShow;

	/**
		Cumulated flags for UI-update after it becomes visible state.
	*/
	int m_updateOnShowFlags;
};


// public methods

template <class Gui, class Observer>
TGuiObserverWrap<Gui, Observer>::TGuiObserverWrap()
:	m_ignoreUpdatesCounter(0),
	m_isReadOnly(false),
	m_updateFilter(0),
	m_updateOnShow(false),
	m_updateOnShowFlags(0)
{
}


// pseudo-reimplemented (imod::IObserver)

template <class Gui, class Observer>
bool TGuiObserverWrap<Gui, Observer>::OnAttached(imod::IModel* modelPtr)
{
	bool retVal = true;
	{
		UpdateBlocker block(this);
		
		retVal = Observer::OnAttached(modelPtr);
	}

	if (retVal && Gui::IsGuiCreated() && Observer::IsModelAttached(NULL)){
		OnGuiModelAttached();
	}

	return retVal;
}


template <class Gui, class Observer>
bool TGuiObserverWrap<Gui, Observer>::OnDetached(imod::IModel* modelPtr)
{
	if (Observer::IsModelAttached(modelPtr)){
		if (Gui::IsGuiCreated()){
			if (!m_isReadOnly && !IsUpdateBlocked() && !m_updateOnShow){
				UpdateBlocker blocker(this);

				UpdateModel();
			}

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

	DoUpdate(CF_INIT_EDITOR);
}


template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::OnGuiModelDetached()
{
	if (!m_isReadOnly && Observer::IsModelAttached(NULL) && !IsUpdateBlocked() && !m_updateOnShow){
		UpdateBlocker blocker(this);

		UpdateModel();
	}
}


template <class Gui, class Observer>
bool TGuiObserverWrap<Gui, Observer>::IsUpdateBlocked() const
{
	return (m_ignoreUpdatesCounter > 0);
}


template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::UpdateGui(int /*updateFlags*/)
{
}


// reimplemented (imod::IModelEditor)

template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::UpdateEditor(int updateFlags)
{
	if (!IsUpdateBlocked() && Gui::IsGuiCreated()){
		UpdateBlocker updateBlocker(this);

		UpdateGui(updateFlags);
	}
}


template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::UpdateModel() const
{
}


// pseudo-reimplemented (iqtgui::CGuiComponentBase)

template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::OnGuiShown()
{
	Gui::OnGuiShown();

	if (Observer::IsModelAttached(NULL)){
		if (m_updateOnShow){
			DoUpdate(m_updateOnShowFlags);
		}

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

	if (!Gui::IsGuiCreated()){
		return;
	}

	DoPostponedUpdate(updateFlags);
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


// private methods
template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::DoPostponedUpdate(int updateFlags)
{
	// skip update if the UI is not visible:
	if (!Gui::IsGuiShown()){
		m_updateOnShow = true;
		m_updateOnShowFlags = m_updateOnShowFlags | updateFlags;

		return;
	}

	DoUpdate(updateFlags);
}


template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::DoUpdate(int updateFlags)
{
	bool skipUpdate = false;
	if ((m_updateFilter != 0) && (updateFlags != 0)){
		skipUpdate = ((m_updateFilter & updateFlags) == 0);
	}

	if (!skipUpdate){
		UpdateEditor(updateFlags);
	}

	m_updateOnShow = false;
	m_updateOnShowFlags = 0;
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


