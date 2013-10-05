#ifndef i2d_CCalibration2dProxyComp_included
#define i2d_CCalibration2dProxyComp_included


// ACF includes
#include "istd/TOptDelPtr.h"
#include "icomp/CComponentBase.h"
#include "imod/CMultiModelBridgeBase.h"
#include "i2d/ICalibration2d.h"
#include "i2d/ICalibrationProvider.h"
#include "i2d/CAffineTransformation2d.h"


namespace i2d
{


/**
	Implementation of a proxy of ICalibration2d interface realized over ICalibrationProvider access.
	Behind this proxy a ICalibrationProvider component reference is used for interface implementation
*/
class CCalibration2dProxyComp:
			public icomp::CComponentBase,
			public ICalibration2d,
			protected imod::CMultiModelBridgeBase
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::CMultiModelBridgeBase BaseClass2;

	I_BEGIN_COMPONENT(CCalibration2dProxyComp);
		I_REGISTER_INTERFACE(ICalibration2d);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_ASSIGN(m_calibrationProviderCompPtr, "CalibrationProvider", "Provider of a 2D calibration", true, "CalibrationProvider");
		I_ASSIGN_TO(m_calibrationProviderModelCompPtr, m_calibrationProviderCompPtr, false);
	I_END_COMPONENT;

public:
	CCalibration2dProxyComp();

	// reimplemented (ICalibration2d)
	virtual const imath::IUnitInfo* GetArgumentUnitInfo() const;
	virtual const imath::IUnitInfo* GetResultUnitInfo() const;
	virtual const ICalibration2d* CreateCombinedCalibration(const ICalibration2d& calibration) const;

	// reimplemented (ITransformation2d)
	virtual int GetTransformationFlags() const;
	virtual bool GetDistance(
				const CVector2d& origPos1,
				const CVector2d& origPos2,
				double& result,
				ExactnessMode mode = EM_NONE) const;
	virtual bool GetPositionAt(
				const CVector2d& origPosition,
				CVector2d& result,
				ExactnessMode mode = EM_NONE) const;
	virtual bool GetInvPositionAt(
				const CVector2d& transfPosition,
				CVector2d& result,
				ExactnessMode mode = EM_NONE) const;
	virtual bool GetLocalTransform(
				const CVector2d& origPosition,
				CAffine2d& result,
				ExactnessMode mode = EM_NONE) const;
	virtual bool GetLocalInvTransform(
				const CVector2d& transfPosition,
				CAffine2d& result,
				ExactnessMode mode = EM_NONE) const;

	// reimplemented (imath::TISurjectFunction)
	virtual bool GetInvValueAt(const CVector2d& argument, CVector2d& result) const;
	virtual CVector2d GetInvValueAt(const CVector2d& argument) const;

	// reimplemented (imath::TIMathFunction)
	virtual bool GetValueAt(const CVector2d& argument, CVector2d& result) const;
	virtual CVector2d GetValueAt(const CVector2d& argument) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	const ICalibration2d* GetCalibrationPtr() const;
	
	void EnsureWorkingCalibrationUpdated() const;

private:
	I_REF(ICalibrationProvider, m_calibrationProviderCompPtr);
	I_REF(imod::IModel, m_calibrationProviderModelCompPtr);

	mutable istd::TOptDelPtr<i2d::ICalibration2d> m_workingCalibrationPtr;
	mutable bool m_isCalibrationCalculated;

	static i2d::CAffineTransformation2d s_defaultTransform;
};


} // namespace i2d


#endif // !i2d_CCalibration2dProxyComp_included


