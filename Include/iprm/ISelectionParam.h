#ifndef iprm_ISelectionParam_included
#define iprm_ISelectionParam_included


#include "istd/CString.h"

#include "iser/ISerializable.h"


namespace iprm
{


/**
	Interface allowing to select single option from list of options.
*/
class ISelectionParam: virtual public iser::ISerializable
{
public:
	/**
		Get number of managed set.
	*/
	virtual int GetOptionsCount() const = 0;

	/**
		Get selected index.
	*/
	virtual int GetSelectedOptionIndex() const = 0;

	/**
		Set index of selected option.
		\return	true	if selection change was possible.
	*/
	virtual bool SetSelectedOptionIndex(int index) = 0;

	/**
		Get name of specified option.
	*/
	virtual const istd::CString& GetOptionName(int index) const = 0;

	/**
		Get subselection for specified option.
		Please note, that returned options tree belongs to selection data model.
	*/
	virtual ISelectionParam* GetActiveSubselection() const = 0;
};


} // namespace iprm


#endif // !iprm_ISelectionParam_included


