#ifndef iprm_IParamsManager_included
#define iprm_IParamsManager_included


// Qt includes
#include <QString>


// ACF includes
#include "iprm/ISelectionParam.h"


namespace iprm
{


class IParamsSet;


/**
	Manager of parameters set.
*/
class IParamsManager: virtual public iprm::ISelectionParam
{
public:
	/**
		Bitwise coded flags used to find out supported features.
	*/
	enum ParamsOperationFlags
	{
		MF_COUNT_FIXED = 1,
		MF_NAME_FIXED = 2,
		MF_NO_INSERT = 4,
		MF_NO_DELETE = 8
	};

	/**
		Possible changes of the manager's data model.
	*/
	enum ChangeFlags
	{
		CF_SET_INSERTED = 1 << 14,
		CF_SET_REMOVED = 1 << 15,
		CF_SET_NAME_CHANGED = 1 << 16
	};

	/**
		Get operation control flags of some paramter set or whole manager.
		\sa ManagerFlags.
		\param	index	index addressing position in parameter set list.
						For negative value general flags are returned.
						For example if you want to check if there is any renameable element
						you should call it with negative index value.
						To check if first element can be renamed call it with index value 0.
						To check if you can insert at the last position please use index value equal to current number of parameter set.
	*/
	virtual int GetIndexOperationFlags(int index = -1) const = 0;

	/**
		Get number of managed set.
	*/
	virtual int GetParamsSetsCount() const = 0;

	/**
		Insert new parameters set at selected position.
		\param	index	position in list of parameters or negative value to use default position.
		\return			position of inserted new entry or negative value if no insert was possible.
						For specified index positions (positive \c index value) it will return the same index or negative value.
	*/
	virtual int InsertParamsSet(int index = -1) = 0;

	/**
		Remove parameters set at selected position.
	*/
	virtual bool RemoveParamsSet(int index) = 0;

	/**
		Get selected parameter set.
	*/
	virtual IParamsSet* GetParamsSet(int index) const = 0;

	/**
		Get name of specified parameter set.
	*/
	virtual const QString& GetParamsSetName(int index) const = 0;

	/**
		Set name of specified parameter set.
	*/
	virtual bool SetParamsSetName(int index, const QString& name) = 0;
};


} // namespace iprm


#endif // !iprm_IParamsManager_included


