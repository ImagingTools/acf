#ifndef iprm_IOptionsManager_included
#define iprm_IOptionsManager_included


// ACF includes
#include "iprm/ISelectionConstraints.h"


namespace iprm
{


/**
	Common interface for the management of the dynamic selection constraints.
*/
class IOptionsManager: virtual public ISelectionConstraints
{
public:
	enum ChangeFlags
	{
		CF_OPTION_ADDED = 1 << 7,
		CF_OPTION_REMOVED = 1 << 8
	};

	/**
		Enables a given option. Only enabled options can be selected.
		\sa GetOptionEnabled
	*/
	virtual void SetOptionEnabled(int index, bool isEnabled = true) = 0;

	/**
		Remove an option at the given index.
		Returns \c true if the operation was successull.
	*/
	virtual bool RemoveOption(int index) = 0;

	/**
		Insert an option with the at the given option name and ID.
		Returns \c true if the operation was successull.
	*/
	virtual bool InsertOption(
				QString& optionName,
				const QByteArray& optionId,
				const QString& optionDescription = QString(),
				int index = -1) = 0;
};


} // namespace iprm


#endif // !iprm_IOptionsManager_included


