#ifndef iimg_CBitmapBase_included
#define iimg_CBitmapBase_included


// ACF includes
#include <i2d/CObject2dBase.h>
#include <iimg/IBitmap.h>


namespace iimg
{


/**
	Base implementation of some \c iimg::IBitmap methods.
*/
class CBitmapBase:
			public i2d::CObject2dBase,
			virtual public IBitmap
{
public:
	typedef i2d::CObject2dBase BaseClass;

	// reimplemented (i2d::IObject2d)
	virtual i2d::CVector2d GetCenter() const;
	virtual void MoveCenterTo(const i2d::CVector2d& position);
	virtual i2d::CRectangle GetBoundingBox() const;

	// reimplemented (iimg::IBitmap)
	virtual bool CreateImageFromRegion(const iimg::IBitmap& sourceBitmap, const i2d::CRect& region);
	virtual int GetLineBytesCount() const;
	virtual int GetComponentBitsCount(int componentIndex = 0) const;

	// reimplemented (iimg::IRasterImage)
	virtual bool IsEmpty() const;
	virtual icmm::CVarColor GetColorAt(const istd::CIndex2d& position) const;
	virtual bool SetColorAt(const istd::CIndex2d& position, const icmm::CVarColor& color);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (istd::IChangeable)
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS);
};


} // namespace iimg


#endif // !iimg_CBitmapBase_included


