#ifndef iview_CScaleCalibration_included
#define iview_CScaleCalibration_included


#include "istd/TChangeNotifier.h"

#include "i2d/ITransformation2d.h"
#include "i2d/CAffine2d.h"


namespace iview
{


class CScaleCalibration: virtual public i2d::ITransformation2d
{
public:
	CScaleCalibration();

	/**
		Reset this calibration, set to be identity transform.
	*/
	void Reset();

	/**
		Get scale factor.
		This scale factor is multiplier used to calculate
		view position from logical position.
	*/
	double GetScaleFactor() const;

	/**
		Set scale factor.
		This scale factor is multiplier used to calculate
		view position from logical position.
	*/
	void SetScaleFactor(double value);

	/**
		Get position of logical center in view units.
	*/
	const i2d::CVector2d& GetViewCenter() const;

	/**
		Set position of logical center in view units.
	*/
	void SetViewCenter(const i2d::CVector2d& center);

	/**
		Check, if this calibration is identity.
	*/
	bool IsIdentity() const;
	
	/**
		Get global transformation from logical into view units representing this calibration.
	*/
	i2d::CAffine2d GetLogToViewTransform() const;
	
	/**
		\overload
	*/
	void GetLogToViewTransform(i2d::CAffine2d& result) const;

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
	virtual const i2d::ITransformation2d* CreateCombinedTransformation(const ITransformation2d& transform) const;

	// reimplemented (imath::TISurjectFunction)
	virtual bool GetInvValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const;
	virtual i2d::CVector2d GetInvValueAt(const i2d::CVector2d& argument) const;

	// reimplemented (imath::TIMathFunction)
	virtual bool GetValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const;
	virtual i2d::CVector2d GetValueAt(const i2d::CVector2d& argument) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	i2d::CVector2d m_viewCenter;
	double m_scaleFactor;
};


// inline methods

inline double CScaleCalibration::GetScaleFactor() const
{
	return m_scaleFactor;
}


inline void CScaleCalibration::SetScaleFactor(double value)
{
	if (m_scaleFactor != value){
		istd::CChangeNotifier changePtr(this);
	
		m_scaleFactor = value;
	}
}


inline const i2d::CVector2d& CScaleCalibration::GetViewCenter() const
{
	return m_viewCenter;
}


inline void CScaleCalibration::SetViewCenter(const i2d::CVector2d& center)
{
	if (m_viewCenter != center){
		istd::CChangeNotifier changePtr(this);

		m_viewCenter = center;
	}
}


inline bool CScaleCalibration::IsIdentity() const
{
	return	(m_viewCenter.GetLength2() < I_EPSILON) &&
			(qAbs(m_scaleFactor) < I_EPSILON);
}


} // namespace iview


#endif // !iview_CScaleCalibration_included




