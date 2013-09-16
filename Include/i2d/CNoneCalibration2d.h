#ifndef i2d_CNoneCalibration2d
#define i2d_CNoneCalibration2d


// ACF includes
#include "i2d/ICalibration2d.h"


namespace i2d
{


/**
	Identity calibration, no transformation will be done.
*/
class CNoneCalibration2d: virtual public i2d::ICalibration2d
{
public:
	// reimplemented (i2d::ICalibration2d)
	virtual const imath::IUnitInfo* GetArgumentUnitInfo() const;
	virtual const imath::IUnitInfo* GetResultUnitInfo() const;
	virtual const ICalibration2d* CreateCombinedCalibration(const ICalibration2d& calibration) const;

	// reimplemented (i2d::ITransformation2d)
	virtual int GetTransformationFlags() const;
	virtual bool GetDistance(
				const i2d::CVector2d& origPos1,
				const i2d::CVector2d& origPos2,
				double& result,
				i2d::ITransformation2d::ExactnessMode mode = EM_NONE) const;
	virtual bool GetLocalTransform(const i2d::CVector2d& logPosition, i2d::CAffine2d& result, ExactnessMode mode = EM_NONE) const;
	virtual bool GetLocalInvTransform(const i2d::CVector2d& viewPosition, i2d::CAffine2d& result, ExactnessMode mode = EM_NONE) const;
	virtual bool GetPositionAt(const i2d::CVector2d& viewPosition, i2d::CVector2d& result, ExactnessMode mode = EM_NONE) const;
	virtual bool GetInvPositionAt(const i2d::CVector2d& logPosition, i2d::CVector2d& result, ExactnessMode mode = EM_NONE) const;

	// reimplemented (imath::TISurjectFunction)
	virtual bool GetInvValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const;
	virtual i2d::CVector2d GetInvValueAt(const i2d::CVector2d& argument) const;

	// reimplemented (imath::TIMathFunction)
	virtual bool GetValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const;
	virtual i2d::CVector2d GetValueAt(const i2d::CVector2d& argument) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// static methods
	static const CNoneCalibration2d& GetInstance();

private:
	// static attributes
	static CNoneCalibration2d s_defaultInstance;
};


} // namespace i2d


#endif // !i2d_CNoneCalibration2d


