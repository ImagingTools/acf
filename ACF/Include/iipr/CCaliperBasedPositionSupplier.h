#ifndef iipr_CCaliperBasedPositionSupplier_included
#define iipr_CCaliperBasedPositionSupplier_included


#include "i2d/CVector2d.h"

#include "iprm/IParamsSet.h"

#include "iproc/TSupplierCompWrap.h"

#include "icam/IBitmapSupplier.h"

#include "iipr/IVector2dSupplier.h"
#include "iipr/ILineProjectionProcessor.h"
#include "iipr/ICaliperProcessor.h"


namespace iipr
{


class CCaliperBasedPositionSupplier: public iproc::TSupplierCompWrap<IVector2dSupplier, i2d::CVector2d>
{
public:
	typedef iproc::TSupplierCompWrap<IVector2dSupplier, i2d::CVector2d> BaseClass;

	I_BEGIN_COMPONENT(CCaliperBasedPositionSupplier);
		I_ASSIGN(m_bitmapSupplierCompPtr, "BitmapSupplier", "Provide image to analyse", true, "BitmapSupplier");
		I_ASSIGN(m_lineProjectionProcessorCompPtr, "ProjectionProcessor", "Calculate projection from image", true, "ProjectionProcessor");
		I_ASSIGN(m_caliperProcessorCompPtr, "CaliperProcessor", "Calculate position from projection", true, "CaliperProcessor");
		I_ASSIGN(m_paramsSetCompPtr, "ParamsSet", "Parameters set for processors", false, "ParamsSet");
	I_END_COMPONENT;

	// reimplemented (iipr::IVector2dSupplier)
	virtual const i2d::CVector2d* GetVector2d(I_DWORD objectId) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
	virtual I_DWORD GetMinimalVersion(int versionId = iser::IVersionInfo::UserVersionId) const;

protected:
	// reimplemented (iproc::TSupplierCompWrap)
	virtual int ProduceObject(I_DWORD objectId, i2d::CVector2d& result) const;

private:
	I_REF(icam::IBitmapSupplier, m_bitmapSupplierCompPtr);

	I_REF(iipr::ILineProjectionProcessor, m_lineProjectionProcessorCompPtr);
	I_REF(iipr::ICaliperProcessor, m_caliperProcessorCompPtr);

	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
};


} // namespace iipr


#endif // !iipr_CCaliperBasedPositionSupplier_included


