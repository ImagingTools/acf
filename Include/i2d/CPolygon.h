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
};


} // namespace i2d


#endif // !i2d_CPolygon_included


