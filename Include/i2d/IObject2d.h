#ifndef i2d_IObject2d_included
#define i2d_IObject2d_included


#include "iser/ISerializable.h"

#include "imath/imath.h"


namespace i2d
{


class CVector2d;
class CRectangle;


/**
	Common interface for describing the 2D-objects.
*/
class IObject2d: virtual public iser::ISerializable
{
public:
	enum ChangeFlags
	{
		CF_OBJECT_POSITION = 1 << 20
	};

	/**
		Returns center of this 2D-object.
	*/
	virtual CVector2d GetCenter() const = 0;

	/**
		Move object to position \c position.
	*/
	virtual void MoveCenterTo(const CVector2d& position) = 0;
};


} // namespace i2d


#endif // !i2d_IObject2d_included

