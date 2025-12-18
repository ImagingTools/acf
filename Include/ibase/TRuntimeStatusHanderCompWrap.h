#pragma once


// ACF includes
#include <imod/IModel.h>
#include <ibase/IRuntimeStatusProvider.h>
#include <imod/CMultiModelDispatcherBase.h>
#include <icomp/CComponentBase.h>


namespace ibase
{


/**
	Wrapper for providing additional component life-cycle states based on the application's runtime status.
*/
template <class ComponentClass>
class TRuntimeStatusHanderCompWrap: public ComponentClass
{
public:
	typedef ComponentClass BaseClass;

	enum ModelId
	{
		MI_RUNTIME_STATUS = 0,
		MI_LAST
	};

	I_BEGIN_BASE_COMPONENT(TRuntimeStatusHanderCompWrap);
		I_ASSIGN(m_runtimeStatusCompPtr, "RuntimeStatus", "Application's runtime status", false, "RuntimeStatus");
		I_ASSIGN_TO(m_runtimeStatusModelCompPtr, m_runtimeStatusCompPtr, false);
	I_END_COMPONENT;

	TRuntimeStatusHanderCompWrap();

protected:
	/**
		The method will be called directly after the creating the event loop of the application.
	*/
	virtual void OnSystemStarting();

	/**
		The method will be called after the application' initialization was completed.
	*/
	virtual void OnSystemStarted();

	/**
		The method will be called before the event loop of the application is quit.
	*/
	virtual void OnSystemShutdown();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

private:
	class RuntimeStatusObserver: public imod::CMultiModelDispatcherBase
	{
	public:
		RuntimeStatusObserver(TRuntimeStatusHanderCompWrap& parent);

	protected:
		// reimplemented (imod::CMultiModelDispatcherBase)
		virtual void OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet) override;

	private:
		TRuntimeStatusHanderCompWrap& m_parent;
	};

protected:
	I_REF(ibase::IRuntimeStatusProvider, m_runtimeStatusCompPtr);
	I_REF(imod::IModel, m_runtimeStatusModelCompPtr);

	RuntimeStatusObserver m_statusObserver;
};


// public methods

template <class ComponentClass>
TRuntimeStatusHanderCompWrap<ComponentClass>::TRuntimeStatusHanderCompWrap()
	:m_statusObserver(*this)
{
}


// protected methods

template <class ComponentClass>
void TRuntimeStatusHanderCompWrap<ComponentClass>::OnSystemStarting()
{
}


template <class ComponentClass>
void TRuntimeStatusHanderCompWrap<ComponentClass>::OnSystemStarted()
{
}


template <class ComponentClass>
void TRuntimeStatusHanderCompWrap<ComponentClass>::OnSystemShutdown()
{
}


// reimplemented (icomp::CComponentBase)

template <class ComponentClass>
void TRuntimeStatusHanderCompWrap<ComponentClass>::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_runtimeStatusCompPtr.IsValid() && m_runtimeStatusModelCompPtr.IsValid()){
		m_statusObserver.RegisterModel(m_runtimeStatusModelCompPtr.GetPtr(), MI_RUNTIME_STATUS);
	}
}


template <class ComponentClass>
void TRuntimeStatusHanderCompWrap<ComponentClass>::OnComponentDestroyed()
{
	m_statusObserver.UnregisterAllModels();

	BaseClass::OnComponentDestroyed();
}


// public methods of the embedded class RuntimeStatusObserver

template <class ComponentClass>
TRuntimeStatusHanderCompWrap<ComponentClass>::RuntimeStatusObserver::RuntimeStatusObserver(TRuntimeStatusHanderCompWrap<ComponentClass>& parent)
	:m_parent(parent)
{
}


// protected methods of the embedded class RuntimeStatusObserver

// reimplemented (imod::CMultiModelDispatcherBase)

template <class ComponentClass>
void TRuntimeStatusHanderCompWrap<ComponentClass>::RuntimeStatusObserver::OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet)
{
	Q_UNUSED(changeSet);

	if (modelId == MI_RUNTIME_STATUS){
		switch (m_parent.m_runtimeStatusCompPtr->GetRuntimeStatus()){
		case ibase::IRuntimeStatusProvider::RS_STARTING:
			m_parent.OnSystemStarting();
			break;

		case ibase::IRuntimeStatusProvider::RS_RUNNING:
			m_parent.OnSystemStarted();
			break;

		case ibase::IRuntimeStatusProvider::RS_SHUTDOWN:
			m_parent.OnSystemShutdown();
			break;

		default:
			break;
		}
	}
}


} // namespace ibase


