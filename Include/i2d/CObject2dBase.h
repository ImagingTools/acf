#pragma once


// ACF includes
#include <istd/TOptInterfacePtr.h>
#include <i2d/IObject2d.h>


namespace i2d
{


/**
	Base class for 2D-objects implementing interface i2d::IObject2d.
	It contains simple or empty implementations.
*/
class CObject2dBase: virtual public IObject2d
{
public:
	CObject2dBase();
	CObject2dBase(const CObject2dBase& object2d);
	CObject2dBase(CObject2dBase&& object2d);

	CObject2dBase& operator=(const CObject2dBase& object2d);
	CObject2dBase& operator=(CObject2dBase&& object2d);

	// reimplemented (i2d::ICalibrationProvider)
	virtual const ICalibration2d* GetCalibration() const override;

	// reimplemented (i2d::IObject2d)
	virtual void SetCalibration(const ICalibration2d* calibrationPtr, bool releaseFlag = false) override;
	virtual bool Transform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) override;
	virtual bool InvTransform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) override;
	virtual bool GetTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const override;
	virtual bool GetInvTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const override;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS) override;

protected:
	static const ChangeSet s_objectMovedChange;
	static const ChangeSet s_objectModifiedChange;

private:
	istd::TOptInterfacePtr<ICalibration2d> m_calibrationPtr;
};


} // namespace i2d


