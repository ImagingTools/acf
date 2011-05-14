#ifndef iprm_IParamsManager_included
#define iprm_IParamsManager_included


#include "istd/CString.h"

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
	enum ManagerFlags{
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
		Get control flags of this manager.
		\sa ManagerFlags.
	*/
	virtual int GetManagerFlags() const = 0;

	/**
		Get number of managed set.
	*/
	virtual int GetParamsSetsCount() const = 0;

	/**
		Insert new parameters set at selected position or at the and of the list.
	*/
	virtual bool InsertParamsSet(int index = -1) = 0;

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
	virtual const istd::CString& GetParamsSetName(int index) const = 0;

	/**
		Set name of specified parameter set.
	*/
	virtual bool SetParamsSetName(int index, const istd::CString& name) = 0;
};


} // namespace iprm


#endif // !iprm_IParamsManager_included


