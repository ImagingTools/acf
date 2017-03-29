#ifndef i2d_CDirection2d_included
#define i2d_CDirection2d_included


namespace i2d
{


class CVector2d;


/**
	Represents a direction in 2D space
*/
class CDirection2d
{
public:
	CDirection2d();

	/**
		Construct a new object by copy.
	*/
	CDirection2d(const CDirection2d& direction);

	/**
		Construct a new object from radian coordinates.
		\param	radian	angle in radians.
	*/
	explicit CDirection2d(double radian);

	/**
		Construct direction using angle of vector.
	*/
	CDirection2d(const CVector2d& direction);

	/**
		Construct a new CDirection2d object and initialize from radian.
	*/
	static CDirection2d FromRadian(double radian);

	/**
		Construct a new CDirection2d object and initialize from degree.
	 */
	static CDirection2d FromDegree(double degree);

	/**
		Get direction in degrees.
		\return		Normalized direction in range [0, 360).
	*/
	double ToDegree() const;

	/**
		Get direction in radians.
		\return		Normalized direction in range [-PI, +PI].
	 */
	double ToRadian() const;

	/**
		Get angle difference in radians.
		\return		Minimal angle distance between the this and dir2 directions in radians [0, +PI].
	 */
	double DistInRadian(const CDirection2d& dir2) const;

	/**
		Get angle difference in degrees.
		\return		Minimal angle distance between the this and dir2 directions in degree [0, +180].
	 */
	double DistInDegree(const CDirection2d& dir2) const;

	/**
		Get minimal distance between two directions.
		\return		Direction that minimizes the distance to this and dir2.
	 */
	CDirection2d Avg(const CDirection2d& dir2) const;

	/**	
		Get weighted minimal distance between two directions.
		\return		Direction that minimizes the weighted distances between both directions.
	 */
	CDirection2d Avg(double weight, const CDirection2d& dir2) const;

	CDirection2d TurnByRadian(double turn) const;

	/**
		Compare the objects direction with the one of dir2.
		\param	dir2	direction to compare.
		\return			true if both are equal.
	 */
	bool operator==(const CDirection2d& dir2) const;

	/**
		Check if the direction is in the direction range between leftDirectionLimit and rightDirectionLimit
	*/
	bool IsInRange(const CDirection2d& leftDirectionLimit, const CDirection2d& rightDirectionLimit) const;

private:
	/**
		Checks the objects invariant. This is for assertion annotation
		\return		true if all object invariants are valid
	*/
	bool Invariant() const;

	/**
		Normalize internal angle to the range [-PI, +PI]
	*/
	static double Normalize(double radian);

	/**
		The direction in radian.
	*/
	double m_direction;
};


} // namespace i2d


#endif // !i2d_CDirection2d_included


