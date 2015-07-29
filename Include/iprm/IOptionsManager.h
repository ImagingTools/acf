#ifndef iprm_IOptionsManager_included
#define iprm_IOptionsManager_included


// ACF includes
#include "iprm/IOptionsList.h"
#include "iprm/ISelectionParam.h"


namespace iprm
{


/**
	Common interface for the management of the dynamic selection constraints.
*/
class IOptionsManager: virtual public IOptionsList, virtual public ISelectionParam
{
public:
	/**
		Data model change notification flags.
	*/
	enum ChangeFlags
	{
		CF_OPTION_ADDED = 0x26be73a,
		CF_OPTION_REMOVED
	};
	
	/**
		Bitwise coded flags used to find out supported features.
	*/
	enum OptionOperationFlags
	{
		/**
			Active if number of parameters is fixed.
		*/
		OOF_COUNT_FIXED = 1,

		/**
			Active if rename of parameters is supported.
		*/
		OOF_SUPPORT_RENAME = 2,

		/**
			Active if insert of parameters is possible.
		*/
		OOF_SUPPORT_INSERT = 4,

		/**
			Active if delete of parameters is possible.
		*/
		OOF_SUPPORT_DELETE = 8,

		/**
			Active if swap of parameters with the other one is possible.
		*/
		OOF_SUPPORT_SWAP = 16,

		/**
			Active if it is possible to get the disabled option.
		*/
		OOF_DISABLE_ALLOWED = 32,

		/**
			Active if enabling and disabling of the parameters is possible.
		*/
		OOF_SUPPORT_ENABLING = 64
	};


	/**
		Get operation control flags of some option or whole manager.
		\param	index	index addressing position in the option list.
						For negative value general flags are returned.
						For example if you want to check if there is any re-nameable element
						you should call it with negative index value.
						To check if first element can be renamed call it with index value 0.
						To check if you can insert at the last position please use index value equal to current options count.
	*/
	virtual int GetOptionOperationFlags(int index = -1) const = 0;	// TODO: Merge this with iprm::IOptionsList::GetOptionsFlags()

	/**
		Enables a given option. Only enabled options can be selected.
		\sa GetOptionEnabled
	*/
	virtual bool SetOptionEnabled(int index, bool isEnabled = true) = 0;

	/**
		Remove an option at the given index.
		Returns \c true if the operation was successful.
	*/
	virtual bool RemoveOption(int index) = 0;

	/**
		Insert an option at some position.
		\param optionName	Name of the option
		\param optionId		ID of the option, according to data model constraints needs to be unique
		\param optionDescription	Description of the option
		\param index	Position in the list, where the new option will be inserted. Negative value means that the option will be added to the end of the list.	
		Returns \c true if the operation was successful.
	*/
	virtual bool InsertOption(
				const QString& optionName,
				const QByteArray& optionId,
				const QString& optionDescription = QString(),
				int index = -1) = 0;

	/**
		Swap two options.
	*/
	virtual bool SwapOptions(int index1, int index2) = 0;

	/**
		Set a new name for the option at the given index \c optionIndex
	*/
	virtual bool SetOptionName(int optionIndex, const QString& optionName) = 0;

	/**
		Set a new description for the option at the given index \c optionIndex
	*/
	virtual bool SetOptionDescription(int optionIndex, const QString& optionDescription) = 0;
};


} // namespace iprm


#endif // !iprm_IOptionsManager_included


