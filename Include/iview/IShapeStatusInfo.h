#ifndef iview_IShapeStatusInfo_included
#define iview_IShapeStatusInfo_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include "istd/IChangeable.h"
#include "i2d/CVector2d.h"


namespace iview
{


/**
	Interface for access local bitmap information.
*/
class IShapeStatusInfo: virtual public istd::IChangeable
{
public:
	/**
		Get logical position on the bitmap.
	*/
//	virtual i2d::CVector2d GetLogicalPosition() const = 0;

	/**
		Set logical position on the bitmap.
	*/
//	virtual void SetLogicalPosition(const i2d::CVector2d& logicalPosition) = 0;

	/**
		Get position in pixel on the bitmap.
	*/
//	virtual i2d::CVector2d GetPixelPosition() const = 0;

	/**
		Set position in pixel on the bitmap.
	*/
//	virtual void SetPixelPosition(const i2d::CVector2d& pixelPosition) = 0;

	/**
		Get information text for the current position.
	*/
	virtual QString GetInfoText() const = 0;

	/**
		Set information text for the current position.
	*/
	virtual void SetInfoText(const QString& infoText) = 0;
};


} // namespace iview


#endif // !iview_IShapeStatusInfo_included


