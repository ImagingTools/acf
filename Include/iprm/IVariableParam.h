#ifndef iprm_IVariableParam_included
#define iprm_IVariableParam_included


// STL includes
#include <string>
#include <set>


// ACF includes
#include "iser/ISerializable.h"

#include "iprm/iprm.h"


namespace iprm
{


/**
	Represents parameter, which can change its type.
*/
class IVariableParam: virtual public iser::ISerializable
{
public:
	typedef std::set<std::string> TypeIds;

	/**
		Get lilst of all known type ID's.
	*/
	virtual TypeIds GetKnownTypeIds() const = 0;

	/**
		Get type ID of current stored parameter.
		\return	type ID of current stored parameter or empty string if no type is assigned.
	*/
	virtual std::string GetParameterTypeId() const = 0;
	/**
		Get current parameter.
		\return	current stored parameter or NULL if no type is assigned.
	*/
	virtual iser::ISerializable* GetParameter() const = 0;

	/**
		Assign type ID to this object.
		Assigning of type ID creates internal instance of parameter object.
	*/
	virtual bool AssignTypeId(const std::string& typeId) = 0;
};


} // namespace iprm


#endif // !iprm_IVariableParam_included


