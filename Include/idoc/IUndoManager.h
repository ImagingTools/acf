#ifndef idoc_IUndoManager_included
#define idoc_IUndoManager_included


// ACF includes
#include "idoc/IDocumentStateComparator.h"


namespace idoc
{


/**
	Interface providing UNDO/REDO functionality.
*/
class IUndoManager: virtual public IDocumentStateComparator
{
public:
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


} // namespace idoc


#endif // !idoc_IUndoManager_included


