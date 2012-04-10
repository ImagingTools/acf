#ifndef iprm_IParamsSet_included
#define iprm_IParamsSet_included


#include "iser/ISerializable.h"

#include "iprm/iprm.h"


namespace iprm
{


/**
	Set of processing parameter.
	Processing parameters can be editable or not (e.g. automatic calculated parameters).
*/
class IParamsSet: virtual public iser::ISerializable
{
public:
	/**
		Get any parameter.
		\param	id	ID of parameter. It is application specified.
		\return		pointer to parameter instance or NULL if there is no parameter with this ID.
	*/
	virtual const iser::ISerializable* GetParameter(const QByteArray& id) const = 0;
	/**
		Get access to editable parameter.
		\param	id	ID of parameter. It is application specified.
		\return		pointer to parameter instance or NULL if there is no editable parameter with this ID.
	*/
	virtual iser::ISerializable* GetEditableParameter(const QByteArray& id) = 0;
};


} // namespace iprm


#endif // !iprm_IParamsSet_included


