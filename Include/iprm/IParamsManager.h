#ifndef iprm_IParamsManager_included
#define iprm_IParamsManager_included


#include "istd/CString.h"

#include "iser/ISerializable.h"


namespace iprm
{


class IParamsSet;


/**
	Manager of parameters set.
*/
class IParamsManager: public iser::ISerializable
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
		Get control flags of this manager.
		\sa ManagerFlags.
	*/
	virtual int GetManagerFlags() const = 0;

	/**
		Get number of managed set.
	*/
	virtual int GetSetsCount() const = 0;

	/**
		Insert new parameters set at selected position or at the and of the list.
	*/
	virtual bool InsertSet(int index = -1) = 0;

	/**
		Remove parameters set at selected position.
	*/
	virtual bool RemoveSet(int index) = 0;

	/**
		Get selected parameter set.
	*/
	virtual IParamsSet* GetParamsSet(int index) const = 0;

	/**
		Get name of specified parameter set.
	*/
	virtual const istd::CString& GetSetName(int index) const = 0;

	/**
		Set name of specified parameter set.
	*/
	virtual bool SetSetName(int index, const istd::CString& name) = 0;
};


} // namespace iprm


#endif // !iprm_IParamsManager_included


