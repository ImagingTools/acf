#ifndef iipr_CCaliperBasedPositionSupplierComp_included
#define iipr_CCaliperBasedPositionSupplierComp_included


#include "i2d/CVector2d.h"

#include "iprm/IParamsSet.h"

#include "iproc/IProcessor.h"
#include "iproc/TSupplierCompWrap.h"

#include "icam/IBitmapSupplier.h"

#include "iipr/IValueSupplier.h"
#include "iipr/IFeaturesMapper.h"


namespace iipr
{


class CCaliperBasedPositionSupplierComp: public iproc::TSupplierCompWrap<IValueSupplier, imath::CVarVector>
{
public:
	typedef iproc::TSupplierCompWrap<IValueSupplier, imath::CVarVector> BaseClass;

	I_BEGIN_COMPONENT(CCaliperBasedPositionSupplierComp);
		I_REGISTER_INTERFACE(CCaliperBasedPositionSupplierComp);
		I_ASSIGN(m_bitmapSupplierCompPtr, "BitmapSupplier", "Provide image to analyse", true, "BitmapSupplier");
		I_ASSIGN(m_featuresMapperCompPtr, "FeaturesMapper", "Calculate position from caliper extracted features", true, "FeaturesMapper");
		I_ASSIGN(m_caliperProcessorCompPtr, "CaliperProcessor", "Calculate position from projection", true, "CaliperProcessor");
	I_END_COMPONENT;

	// reimplemented (iipr::IValueSupplier)
	virtual imath::CVarVector GetValue(I_DWORD objectId) const;

	// reimplemented (iproc::ISupplier)
	virtual void BeginNextObject(I_DWORD objectId);

protected:
	// reimplemented (iproc::TSupplierCompWrap)
	virtual int ProduceObject(I_DWORD objectId, imath::CVarVector& result) const;

private:
	I_REF(icam::IBitmapSupplier, m_bitmapSupplierCompPtr);

	I_REF(iipr::IFeaturesMapper, m_featuresMapperCompPtr);
	I_REF(iproc::IProcessor, m_caliperProcessorCompPtr);
};


} // namespace iipr


#endif // !iipr_CCaliperBasedPositionSupplierComp_included


