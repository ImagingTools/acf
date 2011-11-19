#ifndef iproc_IStateController_included
#define iproc_IStateController_included


#include "istd/IChangeable.h"


namespace iproc
{


/**
	Interface for a wizard page status info.
*/
class IStateController: virtual public istd::IChangeable
{
public:
	enum ChangeFlags
	{
		CF_STATE_ENTERED = 0x100000,
		CF_STATE_LEAVED = 0x200000,
		CF_GRAPH_CHANGED = 0x400000
	};

	/**
		Check if state is enabled.
		If state is disabled it will be not considered as a element of transition graph.
		State is disabled e.g. if some preselection turns it off.
	*/
	virtual bool IsStateEnabled() const = 0;

	/**
		Check if this state is active.
		State is active, if it was succesfully entered and becomes inactive if it was succesfully leaved.
		\sa	\c TryEnterState and \c TryLeaveState.
	*/
	virtual bool IsStateActive() const = 0;

	/**
		Check if entering of this page is allowed.
		\param	isActionAllowed	if set, action related to the state leaving should be done.
		\param	prevStatePtr	optional controller of the previous state.
	*/
	virtual bool IsEnterAllowed(bool isActionAllowed = true, const IStateController* prevStatePtr = NULL) const = 0;

	/**
		Check if this state can be leaved.
		\param	isActionAllowed	if set, action related to the state leaving should be done.
		\param	nextStatePtr	optional controller of the next state.
	*/
	virtual bool IsLeaveAllowed(bool isActionAllowed = true, const IStateController* nextStatePtr = NULL) const = 0;

	/**
		Try enter this state.
		\param	isActionAllowed	if set, action related to the state leaving should be done.
		\param	prevStatePtr	optional controller of the previous state.
		\return	true, if state could be enter.
	*/
	virtual bool TryEnterState(bool isActionAllowed = true, const IStateController* prevStatePtr = NULL) = 0;

	/**
		Try leave this state.
		\param	isActionAllowed	if set, action related to the state leaving should be done.
		\param	nextStatePtr	optional controller of the next state.
		\return	true, if state could be enter.
	*/
	virtual bool TryLeaveState(bool isActionAllowed = true, const IStateController* prevStatePtr = NULL) = 0;
};


} // namespace iproc


#endif // !iproc_IStateController_included


