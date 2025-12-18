#ifndef i2d_CObject2dProxyComp_included
#define i2d_CObject2dProxyComp_included


// ACF includes
#include <icomp/CComponentBase.h>
#include <i2d/IObject2dProvider.h>


namespace i2d
{


/**
	Implementation of a proxy of IObject2d interface realized over IObject2dProvider access.
	Behind this proxy a IObject2dProvider component reference is used for interface implementation
*/
class CObject2dProxyComp:
			public icomp::CComponentBase,
			public IObject2d
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CObject2dProxyComp);
		I_REGISTER_INTERFACE(IObject2d);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_ASSIGN(m_object2dProviderCompPtr, "Object2dProvider", "Provider of a 2D object", true, "Object2dProvider");
	I_END_COMPONENT;

	// reimplemented (i2d::IObject2d)
	virtual CVector2d GetCenter() const override;
	virtual void MoveCenterTo(const CVector2d& position) override;
	virtual CRectangle GetBoundingBox() const override;
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

	// reimplemented (i2d::IClibrationProvider)
	virtual const ICalibration2d* GetCalibration() const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;

protected:
	const i2d::IObject2d* GetObject2d() const;

private:
	I_REF(i2d::IObject2dProvider, m_object2dProviderCompPtr);
};


} // namespace i2d


#endif // !i2d_CObject2dProxyComp_included


