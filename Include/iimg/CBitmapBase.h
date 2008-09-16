#ifndef iimg_CBitmapBase_included
#define iimg_CBitmapBase_included


#include "iimg/IBitmap.h"


namespace iimg
{


/**
	Base implementation of some \c iimg::IBitmap methods.
*/
class CBitmapBase: virtual public IBitmap
{
public:
	// reimplemented (iimg::IBitmap)
	virtual int GetLineBytesCount() const;
	virtual int GetComponentBitsCount(int componentIndex = 0) const;

	// reimplemented (iimg::IRasterImage)
	virtual bool IsEmpty() const;
	virtual icmm::CVarColor GetColorAt(const istd::CIndex2d& position) const;
	virtual bool SetColorAt(const istd::CIndex2d& position, const icmm::CVarColor& color);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
};


} // namespace iimg


#endif // !iimg_CBitmapBase_included


