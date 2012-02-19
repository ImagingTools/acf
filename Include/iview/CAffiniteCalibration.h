#ifndef iview_CAffiniteCalibration_included
#define iview_CAffiniteCalibration_included


#include "i2d/CAffine2d.h"

#include "iview/IIsomorphicCalibration.h"


namespace iview
{


// TODO: Switch to ACF i2d::ITransformation2d and i2d::CAffineTransformation2d.

/**
	Affinite calibration.
	This class describe a calibration where the coordination system is moved and rotated.
	You need this calibration when your logical coordination system is flat,
	but not orthogonal to camera axis.

	\note Please note, that logical coordination system represents only finite
	rectangle area, you can set using SetBounds(const i2d::CRectangle&) method.
*/
class CAffiniteCalibration: public IIsomorphicCalibration
{
public:
	CAffiniteCalibration();

	/**
		Get global transformation from logical into view units representing this calibration.
	*/
	const i2d::CAffine2d& GetLogToViewTransform() const;
	
	/**
		Get global transformation from logical into view units representing this calibration.
		\overload
	*/
	void GetLogToViewTransform(i2d::CAffine2d& result) const;
	
	/**
		Get global transformation from view into logical units representing this calibration.
	*/
	i2d::CAffine2d GetViewToLogTransform() const;
	
	/**
		Get global transformation from view into logical units representing this calibration.
		\overload
	*/
	void GetViewToLogTransform(i2d::CAffine2d& result) const;

	/**
		Reset this calibration, set to be identity transform.
	*/
	void Reset();

	/**
		Reset this calibration using position, rotation angle, and scale.
	*/
	void Reset(const i2d::CVector2d& position, double angle, double scale = 1.0);

	/**
		Set the transform object.
	*/
	void SetTransform(const i2d::CAffine2d& transform);

	i2d::CVector2d GetCenter() const;
	double GetAngle() const;
	double GetScaleFactor() const;

	virtual void Apply(const iview::IIsomorphicCalibration& calib);
	virtual void ApplyLeft(const iview::IIsomorphicCalibration& calib);
	virtual CAffiniteCalibration GetApply(const iview::IIsomorphicCalibration& calib);
	virtual CAffiniteCalibration GetApplyLeft(const iview::IIsomorphicCalibration& calib);

	// reimplemented (iview::ICalibration)
	virtual CalcStatus GetLogLength(const i2d::CLine2d& line, double& result) const;
	virtual CalcStatus GetLogDeform(const i2d::CVector2d& logPosition, i2d::CMatrix2d& result) const;
	virtual CalcStatus GetViewDeform(const i2d::CVector2d& viewPosition, i2d::CMatrix2d& result) const;

	// reimplemented (iview::IIsomorphicCalibration)
	virtual CalcStatus GetApplyToLog(const i2d::CVector2d& viewPosition, i2d::CVector2d& result) const;
	virtual CalcStatus GetApplyToView(const i2d::CVector2d& logPosition, i2d::CVector2d& result) const;

	// reimplemented (iser::ISerializable)
	bool Serialize(iser::IArchive& archive);

protected:
	static i2d::CAffine2d ExtractTransform(const i2d::CVector2d& position, const iview::IIsomorphicCalibration& calibration);

private:
	i2d::CAffine2d m_logToViewTransform;
};


} // namespace iview


#endif // !iview_CAffiniteCalibration_included