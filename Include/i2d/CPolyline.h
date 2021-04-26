#ifndef i2d_CPolyline_included
#define i2d_CPolyline_included


// ACF includes
#include <i2d/CLine2d.h>
#include <i2d/CPolygon.h>


namespace i2d
{


/**
	2D-object representing a polyline.
	A polyline is a connected series of line segments and is normally used to approximate curved paths.
*/
class CPolyline: public CPolygon
{
public:
	typedef CPolygon BaseClass;

	static QByteArray GetTypeName();

	CPolyline();

	virtual bool IsClosed() const;

	/**
		Set polyline to be opened or closed.
	*/
	virtual void SetClosed(bool state = true);
	
	/**
		Return number of line segments.
	*/
	virtual int GetSegmentsCount() const;
	
	/**
		Return number of line segments.
	*/
	virtual i2d::CLine2d GetSegmentLine(int segmentIndex) const;
	
	/**
		Get length of this polyline.
	*/
	virtual double GetLength() const;
	
	/**
		Return vector for knee, that has the same angle to both neighbor segments.
	*/
	virtual i2d::CVector2d GetKneeVector(int nodeIndex) const;

	/**
		Get adjacent nodes by position on line, and by position of arbitrary point.
	*/
	bool GetAdjacentNodes(double atPositionNormalized, i2d::CVector2d& previous, i2d::CVector2d& next, double& alpha) const;

	/**
		Get adjacent nodes by position on line, and by position of arbitrary point.
	*/
	bool GetAdjacentNodes(const i2d::CVector2d& position, i2d::CVector2d& previous, i2d::CVector2d& next) const;

	/**
		Get adjacent node indices by position on line, and position of arbitrary point.
	*/
	bool GetAdjacentNodeIndices(double position, int& previousIndex, int& nextIndex, double& alpha) const;

	/**
		Get adjacent node indices by position on line, and position of arbitrary point.
	*/
	bool GetAdjacentNodeIndices(const i2d::CVector2d& position, int& previousIndex, int& nextIndex) const;

	/**
		Get adjacent segment by position on line, and by position of arbitrary point.
	*/
	bool GetAdjacentLine(double position, i2d::CLine2d& line, double& alpha) const;

	/**
		Get adjacent segment by position on line, and by position of arbitrary point.
	*/
	bool GetAdjacentLine(const i2d::CVector2d& position, i2d::CLine2d& line) const;

	/**
		Get position on this polyline in order to normal position [0..1].
	*/
	bool GetInterpolatedPosition(double position, i2d::CVector2d& output) const;
	i2d::CVector2d GetInterpolatedPosition(double position) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (iser::IObject)
	virtual QByteArray GetFactoryId() const;

	// reimplemented istd::IChangeable
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;
	virtual bool IsEqual(const IChangeable& object) const;

private:
	bool m_isClosed;
};


// public inline methods

inline CPolyline::CPolyline()
{
	m_isClosed = false;
}


inline bool CPolyline::IsClosed() const
{
	return m_isClosed;
}


} // namespace i2d


#endif // !i2d_CPolyline_included


