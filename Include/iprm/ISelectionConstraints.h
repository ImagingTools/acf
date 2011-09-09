#ifndef iprm_ISelectionContstraints_included
#define iprm_ISelectionContstraints_included


#include "istd/IChangeable.h"
#include "istd/CString.h"

#include "iprm/iprm.h"


namespace iprm
{


/**
	Constraints of selection from set of possibilities.
*/
class ISelectionConstraints: virtual public istd::IChangeable
{
public:
	/**
		Flags for changes related to this interface.
	*/
	enum ChangeFlags
	{
		CF_OPTIONS_COUNT_CHANGED = 1 << 18,
		CF_OPTION_RENAMED = 1 << 19
	};

	/**
		Get number of managed options.
	*/
	virtual int GetOptionsCount() const = 0;

	/**
		Get name of specified option.
	*/
	virtual istd::CString GetOptionName(int index) const = 0;
};


} // namespace iprm


#endif // !iprm_ISelectionContstraints_included


