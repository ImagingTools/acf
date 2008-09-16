#ifndef imod_IUndoManager_included
#define imod_IUndoManager_included


#include "istd/IChangeable.h"


namespace imod
{


/**
	Interface providing UNDO/REDO functionality.
*/
class IUndoManager: virtual public istd::IChangeable
{
public:
	enum ChangeFlags{
		/**
			Used to avoid adding of changes to undo list.
		*/
		CF_NO_UNDO = 0x400
	};

	/**
		Check, if UNDO operation is available.
	*/
	virtual bool IsUndoAvailable() const = 0;
	/**
		Check, if REDO operation is available.
	*/
	virtual bool IsRedoAvailable() const = 0;
	/**
		Reset all UNDO and REDO steps.
	*/
	virtual void ResetUndo() = 0;
	/**
		Process single UNDO step.
	*/
	virtual bool DoUndo() = 0;
	/**
		Process single REDO step.
	*/
	virtual bool DoRedo() = 0;
};


} // namespace imod


#endif // !imod_IUndoManager_included


