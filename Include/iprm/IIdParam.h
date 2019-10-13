#ifndef iprm_IIdParam_included
#define iprm_IIdParam_included


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <iser/ISerializable.h>


namespace iprm
{


/**
	Interface for an ID-parameter.
*/
class IIdParam: virtual public iser::ISerializable
{
public:
	/**
		Get the object-ID.
	*/
	virtual QByteArray GetId() const = 0;

	/**
		Set the object-ID.
	*/
	virtual void SetId(const QByteArray& id) = 0;
};


} // namespace iprm


#endif // !iprm_IIdParam_included


