#ifndef i2d_CPerspectiveCalibration2d_included
#define i2d_CPerspectiveCalibration2d_included


// ACF includes
#include <i2d/ICalibration2d.h>
#include <i2d/CAffine2d.h>
#include <i2d/CRectangle.h>


namespace i2d
{


/**	
	Perspective calibration.
	This class describe a perspective calibration.
	You need this calibration when your logical coordination system is flat,
	but not orthogonal to camera axis.
*/
class CPerspectiveCalibration2d: virtual public i2d::ICalibration2d
{
public:
	CPerspectiveCalibration2d();
	CPerspectiveCalibration2d(const i2d::CAffine2d& affinePart, const i2d::CVector2d& perspectiveAxis);

	/**
		Reset this calibration, set to be identity transform.
	*/
	void Reset();

	const i2d::CAffine2d& GetAffinePart() const;
	i2d::CAffine2d& GetAffinePartRef();
	virtual void SetAffinePart(const i2d::CAffine2d& value);

	const i2d::CVector2d& GetPerspAxis() const;
	virtual void SetPerspAxis(const i2d::CVector2d& value);

	void SetArgumentArea(const i2d::CRectangle* areaPtr);
	void SetResultArea(const i2d::CRectangle* areaPtr);
	void SetArgumentUnitInfo(const imath::IUnitInfo* unitInfoPtr);
	void SetResultUnitInfo(const imath::IUnitInfo* unitInfoPtr);

	bool operator==(const CPerspectiveCalibration2d& calib) const;
	bool operator!=(const CPerspectiveCalibration2d& calib) const;

	// reimplemented (i2d::ICalibration2d)
	virtual const i2d::CRectangle* GetArgumentArea() const;
	virtual const i2d::CRectangle* GetResultArea() const;
	virtual const imath::IUnitInfo* GetArgumentUnitInfo() const;
	virtual const imath::IUnitInfo* GetResultUnitInfo() const;
	virtual const ICalibration2d* CreateCombinedCalibration(const ITransformation2d& transformation) const;

	// reimplemented (i2d::ITransformation2d)
	virtual int GetTransformationFlags() const;
	virtual bool GetDistance(
				const i2d::CVector2d& origPos1,
				const i2d::CVector2d& origPos2,
				double& result,
				ExactnessMode mode = EM_NONE) const;
	virtual bool GetPositionAt(
				const i2d::CVector2d& origPosition,
				i2d::CVector2d& result,
				ExactnessMode mode = EM_NONE) const;
	virtual bool GetInvPositionAt(
				const i2d::CVector2d& transfPosition,
				i2d::CVector2d& result,
				ExactnessMode mode = EM_NONE) const;
	virtual bool GetLocalTransform(
				const i2d::CVector2d& origPosition,
				i2d::CAffine2d& result,
				ExactnessMode mode = EM_NONE) const;
	virtual bool GetLocalInvTransform(
				const i2d::CVector2d& transfPosition,
				i2d::CAffine2d& result,
				ExactnessMode mode = EM_NONE) const;

	// reimplemented (imath::TISurjectFunction)
	virtual bool GetInvValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const;
	virtual i2d::CVector2d GetInvValueAt(const i2d::CVector2d& argument) const;

	// reimplemented (imath::TIMathFunction)
	virtual bool GetValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const;
	virtual i2d::CVector2d GetValueAt(const i2d::CVector2d& argument) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual bool IsEqual(const istd::IChangeable& object) const;
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS);

protected:
	void CalcTransformationFlags();

	// reimplemented (istd::IChangeable)
	virtual void OnEndChanges(const ChangeSet& changeSet);

private:
	i2d::CAffine2d m_affinePart;
	i2d::CVector2d m_perspAxis;

	const i2d::CRectangle* m_argumentAreaPtr;
	const i2d::CRectangle* m_resultAreaPtr;
	const imath::IUnitInfo* m_argumentUnitInfoPtr;
	const imath::IUnitInfo* m_resultUnitInfoPtr;

	int m_transformationFlags;
};


// inline methods

inline const i2d::CAffine2d& CPerspectiveCalibration2d::GetAffinePart() const
{
	return m_affinePart;
}


inline i2d::CAffine2d& CPerspectiveCalibration2d::GetAffinePartRef()
{
	return m_affinePart;
}


inline const i2d::CVector2d& CPerspectiveCalibration2d::GetPerspAxis() const
{
	return m_perspAxis;
}


inline bool CPerspectiveCalibration2d::operator!=(const CPerspectiveCalibration2d& calib) const
{
	return !operator==(calib);
}


} // namespace i2d


#endif // !i2d_CPerspectiveCalibration2d_included


