#ifndef icmpstr_IComponentNoteController_included
#define icmpstr_IComponentNoteController_included


#include "istd/IChangeable.h"

#include "icmpstr/icmpstr.h"


namespace icmpstr
{


/**
	Interface for management of notes in the component registry.
	A note is a user defined description of the component in the diagram context.
	Using of notes make it easier for non-authors to understand, what is the meaning of the given component in the concrete context.
*/
class IComponentNoteController: virtual public istd::IChangeable
{
public:
	enum ChangeFlags
	{
		CF_NOTE_CHANGED = 1 << 29
	};

	/**
		Get the note text to the component with the given name \c componentName.
	*/
	virtual istd::CString GetComponentNote(const std::string& componentName) = 0;

	/**
		Add a note \c componentNote to the component with the given name \c componentName.
	*/
	virtual void SetComponentNote(const std::string& componentName, const istd::CString& componentNote) = 0;
};


} // namespace icmpstr


#endif // !icmpstr_IComponentNoteController_included

