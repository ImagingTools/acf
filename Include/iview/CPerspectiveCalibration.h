#ifndef iview_CPerspectiveCalibration_included
#define iview_CPerspectiveCalibration_included


// ACF includes
#include "i3d/CVector3d.h"

#include "i2d/CNoneCalibration2d.h"
#include "i2d/CAffine2d.h"
#include "i2d/CRectangle.h"


namespace iview
{


/**	
	Perspective calibration.
	This class describe a perspective calibration.
	You need this calibration when your logical coordination system is flat,
	but not orthogonal to camera axis.
	Please note, that logical coordination system represents only finite
	rectangle area, you can set using SetBounds(const i2d::CRectangle&) method.
*/
class CPerspectiveCalibration: virtual public i2d::CNoneCalibration2d
{
public:
	CPerspectiveCalibration();

	/**
		Reset this calibration, set to be identity transform.
	*/
	void Reset();

	void SetCameraParams(const i2d::CVector2d& viewOpticalCenter, const i2d::CVector2d& pixelAngleTangents);

	/**
		Get logical coordinates bounds.
	*/
	const i2d::CRectangle& GetBounds() const;
	
	/**
		Set locical coordinates bounds.
	*/
	virtual void SetBounds(const i2d::CRectangle& bounds);

	const i3d::CVector3d& GetLogAxisGetX() const;
	virtual void SetLogAxisX(const i3d::CVector3d& value);
	const i3d::CVector3d& GetLogAxisGetY() const;
	virtual void SetLogAxisY(const i3d::CVector3d& value);
	const i3d::CVector3d& GetLogCenter() const;
	virtual void SetLogCenter(const i3d::CVector3d& value);

	const i2d::CVector2d& GetViewCenter() const;
	void SetViewCenter(const i2d::CVector2d& center);

	void ApplyLeft(const i2d::CAffine2d& transform);

	void ResetAxes();

	/**
		Calculte calibration using set of two set of points.
	*/
	bool DoCalibration(const i2d::CVector2d* logicalPoints, const i2d::CVector2d* viewPoints, int pointsCount);

	virtual bool GetLogDerivative(const i2d::CVector2d& viewPosition, const i2d::CVector2d& viewDirection, double& result) const;

	bool operator==(const CPerspectiveCalibration& calib) const;
	bool operator!=(const CPerspectiveCalibration& calib) const;

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

protected:
	i2d::CVector2d GetNormPosition(const i2d::CVector2d& viewPosition) const;	
	i2d::CVector2d GetViewPosition(const i2d::CVector2d& normPosition) const;

	bool CalcAllAxes(
					const i2d::CVector2d* logicalPoints,
					const i2d::CVector2d* viewPoints,
					int pointsCount,
					i3d::CVector3d& newAxisX,
					i3d::CVector3d& newAxisY,
					i3d::CVector3d& newCenter);
	bool CalcAxes2d(const i2d::CVector2d* logicalPoints,
					const i2d::CVector2d* viewPoints,
					int pointsCount,
					i3d::CVector3d& newAxisX,
					i3d::CVector3d& newAxisY,
					i3d::CVector3d& newCenter);
	bool CalcRotationOY(
					const i2d::CVector2d* logicalPoints,
					const i2d::CVector2d* viewPoints,
					int pointsCount,
					i3d::CVector3d& newAxisX,
					i3d::CVector3d& newAxisY,
					i3d::CVector3d& newCenter);
	bool CalcRotationOX(
					const i2d::CVector2d* logicalPoints,
					const i2d::CVector2d* viewPoints,
					int pointsCount,
					i3d::CVector3d& newAxisX,
					i3d::CVector3d& newAxisY,
					i3d::CVector3d& newCenter);

private:
	i3d::CVector3d m_logAxisX;
	i3d::CVector3d m_logAxisY;
	i3d::CVector3d m_logCenter;

	i2d::CRectangle m_bounds;

	//	Camera attributes.
	i2d::CVector2d m_viewCenter;
	i2d::CVector2d m_pixelAngleTangents;
};


// inline methods

inline const i2d::CRectangle& CPerspectiveCalibration::GetBounds() const
{
	return m_bounds;
}


inline void CPerspectiveCalibration::SetBounds(const i2d::CRectangle& bounds)
{
	m_bounds = bounds;
}


inline const i3d::CVector3d& CPerspectiveCalibration::GetLogAxisGetX() const
{
	return m_logAxisX;
}


inline void CPerspectiveCalibration::SetLogAxisX(const i3d::CVector3d& value)
{
	m_logAxisX = value;
}


inline const i3d::CVector3d& CPerspectiveCalibration::GetLogAxisGetY() const
{
	return m_logAxisY;
}


inline void CPerspectiveCalibration::SetLogAxisY(const i3d::CVector3d& value)
{
	m_logAxisY = value;
}


inline const i3d::CVector3d& CPerspectiveCalibration::GetLogCenter() const
{
	return m_logCenter;
}


inline void CPerspectiveCalibration::SetLogCenter(const i3d::CVector3d& value)
{
	m_logCenter = value;
}


inline const i2d::CVector2d& CPerspectiveCalibration::GetViewCenter() const
{
	return m_viewCenter;
}


inline void CPerspectiveCalibration::SetViewCenter(const i2d::CVector2d& center)
{
	m_viewCenter = center;
}


inline bool CPerspectiveCalibration::operator!=(const CPerspectiveCalibration& calib) const
{
	return !operator==(calib);
}


// protected methods

inline i2d::CVector2d CPerspectiveCalibration::GetNormPosition(const i2d::CVector2d& viewPosition) const
{
	return i2d::CVector2d(
					(viewPosition.GetX() - m_viewCenter.GetX()) * m_pixelAngleTangents.GetX(),
					(viewPosition.GetY() - m_viewCenter.GetY()) * m_pixelAngleTangents.GetY());
}


inline i2d::CVector2d CPerspectiveCalibration::GetViewPosition(const i2d::CVector2d& normPosition) const
{
	return i2d::CVector2d(
					normPosition.GetX() / m_pixelAngleTangents.GetX() + m_viewCenter.GetX(),
					normPosition.GetY() / m_pixelAngleTangents.GetY() + m_viewCenter.GetY());
}


} // namespace iview


#endif // !iview_CPerspectiveCalibration_included


