#ifndef i2d_COrientedCircle_included
#define i2d_COrientedCircle_included


#include <i2d/CCircle.h>


namespace i2d
{


/**
	Definition of an oriented graphical circle object.
	The circle can be oriented outside or inside.
	Interpretation what it means 'outside oriented' is user dependent.
*/
class COrientedCircle: public CCircle
{
public:
	typedef CCircle BaseClass;

	COrientedCircle();
	COrientedCircle(double radius, const CVector2d& center, bool orientedOutside);
	
	/**
		Check if this circle is oriented outside.
		Interpretation what it means 'outside oriented' is user dependent.
	*/
	bool IsOrientedOutside() const;
	/**
		Set this circle oriented outside or inside.
		Interpretation what it means 'outside oriented' is user dependent.
	*/
	void SetOrientedOutside(bool orientedOutside);

	bool operator==(const COrientedCircle& circle) const;
	bool operator!=(const COrientedCircle& circle) const;

	// reimplemented (i2d::CCircle)
	virtual bool ConvertToPolygon(i2d::CPolygon& result, int segmentsCount = 0) const;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	bool m_orientedOutside;
};


// inline methods

inline bool COrientedCircle::IsOrientedOutside() const
{
	return m_orientedOutside;
}


} // namespace i2d


#endif // !i2d_COrientedCircle_included


