#ifndef i3d_Geometry_included
#define i3d_Geometry_included


// ACF includes
#include <i3d/CVector3d.h>
#include <i3d/CPlane3d.h>
#include <i3d/CLine3d.h>


namespace i3d
{


/**
	Geometric utility functions for 3D space.
*/
namespace Geometry
{


/**
	Calculate angle between two vectors in radians.
	\param v1	First vector.
	\param v2	Second vector.
	\return		Angle in radians [0, PI].
*/
inline double GetAngleBetweenVectors(const CVector3d& v1, const CVector3d& v2)
{
	double dot = v1.GetNormalized().GetDotProduct(v2.GetNormalized());
	// Clamp to avoid numerical issues with acos
	dot = qMax(-1.0, qMin(1.0, dot));
	return qAcos(dot);
}


/**
	Calculate signed angle between two vectors around an axis.
	\param v1		First vector.
	\param v2		Second vector.
	\param axis		Axis to measure rotation around.
	\return			Signed angle in radians [-PI, PI].
*/
inline double GetSignedAngleBetweenVectors(const CVector3d& v1, const CVector3d& v2, const CVector3d& axis)
{
	CVector3d n1 = v1.GetNormalized();
	CVector3d n2 = v2.GetNormalized();
	
	double angle = GetAngleBetweenVectors(n1, n2);
	
	// Determine sign using cross product
	CVector3d cross = n1.GetCrossProduct(n2);
	if (cross.GetDotProduct(axis) < 0.0){
		angle = -angle;
	}
	
	return angle;
}


/**
	Calculate distance between two points.
*/
inline double GetDistance(const CVector3d& p1, const CVector3d& p2)
{
	return (p2 - p1).GetLength();
}


/**
	Calculate squared distance between two points (faster).
*/
inline double GetDistanceSq(const CVector3d& p1, const CVector3d& p2)
{
	return (p2 - p1).GetLength2();
}


/**
	Calculate distance from point to plane.
*/
inline double GetDistance(const CVector3d& point, const CPlane3d& plane)
{
	return plane.GetDistance(point);
}


/**
	Calculate distance from point to line.
*/
inline double GetDistance(const CVector3d& point, const CLine3d& line)
{
	return line.GetDistance(point);
}


/**
	Check if two lines intersect in 3D.
	\param line1		First line.
	\param line2		Second line.
	\param tolerance	Tolerance for intersection test.
	\return				True if lines intersect (or nearly intersect).
*/
bool AreIntersecting(const CLine3d& line1, const CLine3d& line2, double tolerance = I_BIG_EPSILON);


/**
	Find intersection point between line and plane.
	\param line			Line to intersect.
	\param plane		Plane to intersect.
	\param result		Output intersection point.
	\param tolerance	Tolerance for parallel check.
	\return				True if intersection exists, false if line is parallel to plane.
*/
bool GetIntersection(const CLine3d& line, const CPlane3d& plane, CVector3d& result, double tolerance = I_BIG_EPSILON);


/**
	Find closest points between two lines.
	\param line1		First line.
	\param line2		Second line.
	\param point1		Output closest point on line1.
	\param point2		Output closest point on line2.
	\return				Distance between closest points.
*/
double GetClosestPoints(const CLine3d& line1, const CLine3d& line2, CVector3d& point1, CVector3d& point2);


/**
	Check if point is on the same side of plane as the plane's normal.
*/
inline bool IsOnPositiveSide(const CVector3d& point, const CPlane3d& plane)
{
	return plane.GetSignedDistance(point) >= 0.0;
}


/**
	Project vector onto another vector.
	\param vector		Vector to project.
	\param onto			Vector to project onto.
	\return				Projection of vector onto 'onto'.
*/
inline CVector3d ProjectVectorOntoVector(const CVector3d& vector, const CVector3d& onto)
{
	double ontoLengthSq = onto.GetLength2();
	if (ontoLengthSq < I_BIG_EPSILON){
		return CVector3d(0.0, 0.0, 0.0);
	}
	
	double projection = vector.GetDotProduct(onto) / ontoLengthSq;
	return onto * projection;
}


/**
	Calculate barycentric coordinates of point relative to triangle.
	\param point	Point to calculate barycentric coordinates for.
	\param p1		First triangle vertex.
	\param p2		Second triangle vertex.
	\param p3		Third triangle vertex.
	\param u		Output barycentric coordinate u.
	\param v		Output barycentric coordinate v.
	\param w		Output barycentric coordinate w (w = 1 - u - v).
	\return			True if calculation successful.
*/
bool GetBarycentricCoordinates(
	const CVector3d& point,
	const CVector3d& p1,
	const CVector3d& p2,
	const CVector3d& p3,
	double& u,
	double& v,
	double& w);


/**
	Check if point is inside triangle.
	\param point	Point to test.
	\param p1		First triangle vertex.
	\param p2		Second triangle vertex.
	\param p3		Third triangle vertex.
	\return			True if point is inside or on triangle boundary.
*/
bool IsPointInTriangle(
	const CVector3d& point,
	const CVector3d& p1,
	const CVector3d& p2,
	const CVector3d& p3);


/**
	Calculate normal vector of triangle.
*/
inline CVector3d GetTriangleNormal(const CVector3d& p1, const CVector3d& p2, const CVector3d& p3)
{
	CVector3d edge1 = p2 - p1;
	CVector3d edge2 = p3 - p1;
	return edge1.GetCrossProduct(edge2).GetNormalized();
}


/**
	Calculate area of triangle.
*/
inline double GetTriangleArea(const CVector3d& p1, const CVector3d& p2, const CVector3d& p3)
{
	CVector3d edge1 = p2 - p1;
	CVector3d edge2 = p3 - p1;
	return edge1.GetCrossProduct(edge2).GetLength() * 0.5;
}


} // namespace Geometry


} // namespace i3d


#endif // !i3d_Geometry_included
