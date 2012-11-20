#ifndef iprm_ISelectionContstraints_included
#define iprm_ISelectionContstraints_included


// Qt includes
#include <QtCore/QString>


// ACF includes
#include "istd/IChangeable.h"

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
		/**
			Option list updated.
		*/
		CF_OPTIONS_CHANGED = 1 << 18,

		/**
			One or more options were renamed.
		*/
		CF_OPTION_RENAMED = 1 << 19
	};

	/**
		Flags for control constraints behavior.
	*/
	enum SelectionConstraintsFlags
	{
		SCF_NONE = 0,

		/**
			If set, every option must have an unique ID.
		*/
		SCF_SUPPORT_UNIQUE_ID = 1 << 0
	};

	/**
		Get constraints flags. The flags describes how the constraints object mantainance
	*/
	virtual int GetConstraintsFlags() const = 0;

	/**
		Get number of managed options.
	*/
	virtual int GetOptionsCount() const = 0;

	/**
		Get name of specified option.
	*/
	virtual QString GetOptionName(int index) const = 0;

	/**
		Get human readable description for a option with the index \c index.
	*/
	virtual QString GetOptionDescription(int index) const = 0;

	/**
		Get option ID. The option ID must be unique, if flag SCF_SUPPORT_UNIQUE_ID is set.
		\sa SelectionConstraintsFlags
	*/
	virtual QByteArray GetOptionId(int index) const = 0;

	/**
		Return \c true if the option is enabled and can be selected.
		\sa SetOptionEnabled
	*/
	virtual bool IsOptionEnabled(int index) const = 0;
};


} // namespace iprm


#endif // !iprm_ISelectionContstraints_included


