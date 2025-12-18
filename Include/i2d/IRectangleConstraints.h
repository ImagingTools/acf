#ifndef i2d_IRectangleConstraints_included
#define i2d_IRectangleConstraints_included


// ACF includes
#include <i2d/CRectangle.h>


namespace i2d
{


class IRectangleConstraints: virtual public istd::IPolymorphic
{
public:
	virtual istd::CIntRange GetWidthRange() const = 0;
	virtual istd::CIntRange GetHeightRange() const = 0;
};


} // namespace i2d


#endif // !i2d_IRectangleConstraints_included
