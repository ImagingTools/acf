#ifndef i2d_IObject2dProvider_included
#define i2d_IObject2dProvider_included


// ACF includes
#include <istd/IChangeable.h>
#include <i2d/IObject2d.h>


namespace i2d
{


/**
	Common interface for a provider, which delivers a 2D-object.
*/
class IObject2dProvider: virtual public istd::IChangeable
{
public:
	/**
		Get access to 2D-object.
	*/
	virtual const IObject2d* GetObject2d() const = 0;
};


} // namespace i2d


#endif // !i2d_IObject2dProvider_included


