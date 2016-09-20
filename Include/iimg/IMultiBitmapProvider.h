#ifndef iimg_IMultiBitmapProvider_included
#define iimg_IMultiBitmapProvider_included


// ACF includes
#include <istd/IChangeable.h>
#include <i2d/ITransformation2d.h>
#include <iprm/IOptionsList.h>
#include <iimg/IBitmap.h>


namespace iimg
{


/**
	Supplier allowing to access of multiple produced bitmaps.
*/
class IMultiBitmapProvider: virtual public istd::IChangeable
{
public:
	/**
		Get optional information about bitmaps in form of section constraints.
		It allows to get name and description of each bitmap channel.
	*/
	virtual const iprm::IOptionsList* GetBitmapListInfo() const = 0;

	/**
		Get number of produced bitmaps.
	*/
	virtual int GetBitmapsCount() const = 0;
	/**
		Get access to produced bitmap object.
		\return	pointer to bitmap instance if this bitmap is accessible, or NULL.
	*/
	virtual const iimg::IBitmap* GetBitmap(int bitmapIndex) const = 0;
};


} // namespace iimg


#endif // !iimg_IMultiBitmapProvider_included


