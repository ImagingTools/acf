#ifndef iprm_ISelectionParam_included
#define iprm_ISelectionParam_included


// Qt includes
#include <QtCore/QString>


// ACF includes
#include "iser/ISerializable.h"


namespace iprm
{


class IOptionsList;


/**
	Interface allowing to select single option from list of options.
*/
class ISelectionParam: virtual public iser::ISerializable
{
public:
	enum ChangeFlags
	{
		CF_SELECTION_CHANGED = 1 << 17
	};

	enum 
	{
		NO_SELECTION = -1,
	};

	/**
		Get constraints of this parameter.
		Constraints describes some additional information about allowed parameter ranges and related informations.
	*/
	virtual const IOptionsList* GetSelectionConstraints() const = 0;

	/**
		Get selected index.
		\return	index of selected option or negative value if no option is selected.
	*/
	virtual int GetSelectedOptionIndex() const = 0;

	/**
		Set index of selected option.
		\return	true	if selection change was possible.
	*/
	virtual bool SetSelectedOptionIndex(int index) = 0;

	/**
		Get subselection for specified option.
		Please note, that returned options tree belongs to selection data model.
	*/
	virtual ISelectionParam* GetSubselection(int index) const = 0;
};


} // namespace iprm


#endif // !iprm_ISelectionParam_included


