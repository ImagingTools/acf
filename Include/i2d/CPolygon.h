#ifndef i2d_CPolygon_included
#define i2d_CPolygon_included


// ACF includes
#include <i2d/CPolypoint.h>


namespace i2d
{


/**
	Definition of the data model for a polygon.
*/
class CPolygon: public CPolypoint
{
public:
	typedef CPolypoint BaseClass;

	static QByteArray GetTypeName();

	/**
		Check if the polygon contains a given point.
	*/
	bool Contains(const i2d::CVector2d& point) const;

	/**
		Get outline length of this polygon.
	*/
	virtual double GetOutlineLength() const;

	/** 
		Flip object by X-axis.
	*/
	virtual void FlipByX();

	/** 
		Flip object by Y-axis.
	*/
	virtual void FlipByY();

	/** 
		Rotate object around its center.
	*/
	virtual void Rotate(double radians);

	/** 
		Reverses nodes order.
	*/
	virtual void ReverseNodes();

	/**
		Get area of the polygon.
	*/
	double GetArea(bool oriented = false) const;

	/**
		Get perimeter of the polygon.
	*/
	double GetPerimeter() const;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);

	// reimplemented (iser::IObject)
	virtual QByteArray GetFactoryId() const;
};


} // namespace i2d


#endif // !i2d_CPolygon_included


