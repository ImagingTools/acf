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
		MF_NO_DELETE = 4
		MF_NO_INSERT = 8
	};

	/**
		Get number of managed set.
	*/
	virtual int GetSetsCount() const = 0;

	/**
		Get number of managed set.
	*/
	virtual bool SetSetsCount(int count) = 0;

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


