#ifndef i2d_CAffineCalibration2d_included
#define i2d_CAffineCalibration2d_included


// ACF includes
#include <i2d/ICalibration2d.h>
#include <i2d/CAffineTransformation2d.h>
#include <i2d/CAffine2d.h>


namespace i2d
{


/**
	Definition of an affine transformation for 2D-spaces.
*/
class CAffineCalibration2d:
			public CAffineTransformation2d,
			virtual public ICalibration2d
{
public:
	typedef CAffineTransformation2d BaseClass;

	CAffineCalibration2d();

	explicit CAffineCalibration2d(const i2d::CAffine2d& transformation);

	void SetArgumentArea(const i2d::CRectangle* areaPtr);
	void SetResultArea(const i2d::CRectangle* areaPtr);
	void SetArgumentUnitInfo(const imath::IUnitInfo* unitInfoPtr);
	void SetResultUnitInfo(const imath::IUnitInfo* unitInfoPtr);

	// reimplemented (i2d::ICalibration2d)
	virtual const CRectangle* GetArgumentArea() const;
	virtual const CRectangle* GetResultArea() const;
	virtual const imath::IUnitInfo* GetArgumentUnitInfo() const;
	virtual const imath::IUnitInfo* GetResultUnitInfo() const;
	virtual const ICalibration2d* CreateCombinedCalibration(const ITransformation2d& transformation) const;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

private:
	const i2d::CRectangle* m_argumentAreaPtr;
	const i2d::CRectangle* m_resultAreaPtr;
	const imath::IUnitInfo* m_argumentUnitInfoPtr;
	const imath::IUnitInfo* m_resultUnitInfoPtr;
};


} // namespace i2d


#endif // !i2d_CAffineTransformation2d_included


