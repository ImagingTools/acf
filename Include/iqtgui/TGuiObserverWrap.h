#ifndef iqtgui_TGuiObserverWrap_included
#define iqtgui_TGuiObserverWrap_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

// ACF includes
#include "imod/IModelEditor.h"
#include "imod/IModel.h"


namespace iqtgui
{


/**
	Join functionality of \c iqtgui::IGuiObject interface and \c imod::IObserver.

	\ingroup ModelObserver
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
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask);
	virtual bool OnModelDetached(imod::IModel* modelPtr);

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
		Secure update model.
		It use GUI block mechanism and it is designed to be called directly from UI slot.
	*/
	bool DoUpdateModel();

	/**
		Do update of the model GUI.
	*/
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (imod::IModelEditor)
	virtual void UpdateEditor(const istd::IChangeable::ChangeSet& changeSet);
	virtual void UpdateModel() const;

	// pseudo-reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiShown();
	virtual void OnGuiHidden();
	virtual void OnGuiRetranslate();
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// pseudo-reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet);

	// pseudo-reimplemented (imod::IModelEditor)
	virtual bool IsReadOnly() const;
	virtual void SetReadOnly(bool state);

protected:
	/**
		Control if GUI should be disaled for read-only editors.
		By default it is set to \c true.
	*/
	void SetDisableUiIfReadOnly(bool state);

	bool m_isReadOnly;

private:
	void DoUpdate(const istd::IChangeable::ChangeSet& changeSet);

private:
	bool m_disableUiIfReadOnly;
	int m_ignoreUpdatesCounter;

	/**
		Do editor update, if the gui change its state to visible.
	*/
	bool m_updateOnShow;

	/**
		Cumulated flags for UI-update after it becomes visible state.
	*/
	istd::IChangeable::ChangeSet m_onShowChangeIds;
};


// public methods

template <class Gui, class Observer>
TGuiObserverWrap<Gui, Observer>::TGuiObserverWrap()
:	m_isReadOnly(false),
	m_disableUiIfReadOnly(true),
	m_ignoreUpdatesCounter(0),
	m_updateOnShow(false)
{
}


// pseudo-reimplemented (imod::IObserver)

template <class Gui, class Observer>
bool TGuiObserverWrap<Gui, Observer>::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	bool retVal = false;

	{
		UpdateBlocker block(this);

		retVal = Observer::OnModelAttached(modelPtr, changeMask);
	}

	if (retVal && Gui::IsGuiCreated()){
		Q_ASSERT(Observer::IsModelAttached(NULL));

		OnGuiModelAttached();
	}

	return retVal;
}


template <class Gui, class Observer>
bool TGuiObserverWrap<Gui, Observer>::OnModelDetached(imod::IModel* modelPtr)
{
	if (Observer::IsModelAttached(modelPtr)){
		if (Gui::IsGuiCreated()){
			if (!m_isReadOnly && !IsUpdateBlocked() && !m_updateOnShow){
				UpdateBlocker updateBlocker(this);
				Q_UNUSED(updateBlocker);

				UpdateModel();
			}

			OnGuiModelDetached();
		}
	}

	return Observer::OnModelDetached(modelPtr);
}


// protected methods

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
	Q_ASSERT(Gui::IsGuiCreated());
	Q_ASSERT(Observer::IsModelAttached(NULL));

	static istd::IChangeable::ChangeSet initChangeSet(CF_INIT_EDITOR);
	UpdateEditor(initChangeSet);
}


template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::OnGuiModelDetached()
{
	if (!m_isReadOnly && Observer::IsModelAttached(NULL) && !IsUpdateBlocked() && !m_updateOnShow){
		UpdateBlocker updateBlocker(this);
		Q_UNUSED(updateBlocker);

		UpdateModel();
	}
}


template <class Gui, class Observer>
bool TGuiObserverWrap<Gui, Observer>::IsUpdateBlocked() const
{
	return (m_ignoreUpdatesCounter > 0);
}


template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::SetDisableUiIfReadOnly(bool state)
{
	m_disableUiIfReadOnly = state;
}


// provate methods

template <class Gui, class Observer>
bool TGuiObserverWrap<Gui, Observer>::DoUpdateModel()
{
	if (!m_isReadOnly && !IsUpdateBlocked() && Observer::IsModelAttached()){
		UpdateBlocker updateBlocker(this);
		Q_UNUSED(updateBlocker);

		UpdateModel();

		return true;
	}

	return false;
}


template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
}


// reimplemented (imod::IModelEditor)

template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::UpdateEditor(const istd::IChangeable::ChangeSet& changeSet)
{
	if (Gui::IsGuiShown()){
		DoUpdate(changeSet);
	}
	else{
		// prepare postponed update
		m_updateOnShow = true;
		m_onShowChangeIds += changeSet;

		return;
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
			// skip update if the UI is not visible:
			DoUpdate(m_onShowChangeIds);

			m_onShowChangeIds.Reset();
			m_updateOnShow = false;
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
void TGuiObserverWrap<Gui, Observer>::OnGuiRetranslate()
{
	UpdateBlocker updateBlocker(this);
	Q_UNUSED(updateBlocker);

	Gui::OnGuiRetranslate();
}


template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::OnGuiCreated()
{
	Gui::OnGuiCreated();

	if (m_disableUiIfReadOnly){
		QWidget* widgetPtr = Gui::GetWidget();
		Q_ASSERT(widgetPtr != NULL);

		widgetPtr->setEnabled(!m_isReadOnly);
	}

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
void TGuiObserverWrap<Gui, Observer>::AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	Q_ASSERT(modelPtr != NULL);
	Q_ASSERT(Observer::IsModelAttached(modelPtr));

	Observer::AfterUpdate(modelPtr, changeSet);

	if (!Gui::IsGuiCreated() || changeSet.ContainsExplicit(istd::IChangeable::CF_DESTROYING)){
		return;
	}

	UpdateEditor(changeSet);
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

	if (m_disableUiIfReadOnly && Gui::IsGuiCreated()){
		QWidget* widgetPtr = Gui::GetWidget();
		Q_ASSERT(widgetPtr != NULL);

		widgetPtr->setEnabled(!m_isReadOnly);
	}
}


// private methods

template <class Gui, class Observer>
void TGuiObserverWrap<Gui, Observer>::DoUpdate(const istd::IChangeable::ChangeSet& changeSet)
{
	if (changeSet.IsEmpty() || IsUpdateBlocked() || !Gui::IsGuiCreated()){
		return;
	}

	UpdateBlocker updateBlocker(this);
	Q_UNUSED(updateBlocker);

	UpdateGui(changeSet);
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


