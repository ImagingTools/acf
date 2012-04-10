#ifndef iprm_ISelectionConstraintsController_included
#define iprm_ISelectionConstraintsController_included


// ACF includes
#include "iprm/ISelectionParam.h"


namespace iprm
{


/**
	Common interface for the management of the dynamic selection constraints.
*/
class IOptionsManager: virtual public istd::IChangeable
{
public:
	enum ChangeFlags
	{
		CF_OPTION_ADDED = 1 << 7,
		CF_OPTION_REMOVED = 1 << 8
	};


	/**
		Remove an option at the given index.
	*/
	virtual void RemoveOption(int index) = 0;

	/**
		Insert an option with the at the given option name and ID.
	*/
	virtual bool InsertOption(
				QString& optionName,
				const QByteArray& optionId,
				const QString& optionDescription = QString(),
				int index = -1) = 0;
};


} // namespace iprm


#endif // !iprm_ISelectionConstraintsController_included


