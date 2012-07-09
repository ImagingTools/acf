#ifndef iprm_IParamsSet_included
#define iprm_IParamsSet_included


// Qt includes
#include <QtCore/QSet>
#include <QtCore/QByteArray>

// ACF includes
#include "iser/ISerializable.h"

#include "iprm/iprm.h"


namespace iprm
{


/**
	Set of general parameters.
	These parameters can be editable or not (e.g. automatic calculated parameters).
*/
class IParamsSet: virtual public iser::ISerializable
{
public:
	typedef QSet<QByteArray> Ids;

	/**
		Get list of used parameter IDs in the parameter set.
		\return Set of the parameter IDs.
	*/
	virtual Ids GetParamIds(bool editableOnly = false) const = 0;

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


