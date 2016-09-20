#ifndef i2d_CCalibration2dProxyComp_included
#define i2d_CCalibration2dProxyComp_included


// ACF includes
#include <icomp/CComponentBase.h>
#include <imod/CMultiModelBridgeBase.h>
#include <i2d/ICalibration2d.h>
#include <i2d/ICalibrationProvider.h>


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

	I_BEGIN_COMPONENT(CCalibration2dProxyComp);
		I_REGISTER_INTERFACE(ICalibration2d);
		I_REGISTER_INTERFACE(ITransformation2d);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_ASSIGN(m_calibrationProviderCompPtr, "CalibrationProvider", "Provider of a 2D calibration", true, "CalibrationProvider");
		I_ASSIGN_TO(m_calibrationProviderModelCompPtr, m_calibrationProviderCompPtr, true);
	I_END_COMPONENT;

	CCalibration2dProxyComp();

	// reimplemented (i2d::ICalibration2d)
	virtual const i2d::CRectangle* GetArgumentArea() const;
	virtual const i2d::CRectangle* GetResultArea() const;
	virtual const imath::IUnitInfo* GetArgumentUnitInfo() const;
	virtual const imath::IUnitInfo* GetResultUnitInfo() const;
	virtual const ICalibration2d* CreateCombinedCalibration(const ITransformation2d& transformation) const;

	// reimplemented (i2d::ITransformation2d)
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

	// reimplemented (imath::TISurjectFunction<i2d::CVector2d, i2d::CVector2d>)
	virtual bool GetInvValueAt(const CVector2d& argument, CVector2d& result) const;
	virtual CVector2d GetInvValueAt(const CVector2d& argument) const;

	// reimplemented (imath::TIMathFunction<i2d::CVector2d, i2d::CVector2d>)
	virtual bool GetValueAt(const CVector2d& argument, CVector2d& result) const;
	virtual CVector2d GetValueAt(const CVector2d& argument) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (istd::IChangeable)
	virtual IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	const ICalibration2d* GetCalibration() const;

private:
	I_REF(ICalibrationProvider, m_calibrationProviderCompPtr);
	I_REF(imod::IModel, m_calibrationProviderModelCompPtr);
};


} // namespace i2d


#endif // !i2d_CCalibration2dProxyComp_included


