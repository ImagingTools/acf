#ifndef iqt_CFireGrabAcquisitionComp_included
#define iqt_CFireGrabAcquisitionComp_included


// FireGrab includes
#include <basetype.h>
#include <fgcamera.h>


#include "ibase/TMessageProducerWrap.h"

#include "iproc/TSyncProcessorWrap.h"

#include "icam/IBitmapAcquisition.h"
#include "icam/IExposureConstraints.h"

#include "icomp/CComponentBase.h"

#include "iavt/iavt.h"


namespace iavt
{


/**
	AVT based camera component implementing interfaces \c icam::IBitmapAcquisition and \c icam::IExposureConstraints for exposure control support.
*/
class CFireGrabAcquisitionComp:
			public ibase::TMessageProducerWrap<icomp::CComponentBase>,
			virtual public icam::IExposureConstraints,
			virtual public iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>
{
public:
	typedef ibase::TMessageProducerWrap<icomp::CComponentBase> BaseClass;

	I_BEGIN_COMPONENT(CFireGrabAcquisitionComp)
		I_REGISTER_INTERFACE(icam::IBitmapAcquisition)
		I_REGISTER_INTERFACE(icam::IExposureConstraints)
		I_ASSIGN(m_exposureParamsIdAttrPtr, "ExposureParamsId", "Id used to get exposure parameters from the parameter set", false, "ExposureParams")
	I_END_COMPONENT

	enum MessageId
	{
		MI_CANNOT_INIT = 0x2ef3210,
		MI_NO_NODES,
		MI_CANNOT_CONNECT,
		MI_CANNOT_OPEN,
		MI_CANNOT_START
	};

	CFireGrabAcquisitionComp();

	// reimplemented (iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>)
	virtual int DoProcessing(const iprm::IParamsSet* paramsPtr, const isys::ITimer* inputPtr, iimg::IBitmap* outputPtr);

	// reimplemented (icam::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

	// reimplemented (icam::IExposureConstraints)
	virtual istd::CRange GetShutterTimeRange() const;
	virtual istd::CRange GetDelayTimeRange() const;
	virtual istd::CRange GetEenDelayRange() const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_ATTR(istd::CString, m_exposureParamsIdAttrPtr);

	enum
	{
		MAX_NODES_COUNT = 16
	};

	mutable CFGCamera  m_camera;
	FGNODEINFO m_nodeInfos[MAX_NODES_COUNT];
	UINT32 m_nodesCount;

	bool m_isCameraValid;
};


} // namespace iavt


#endif // !iqt_CFireGrabAcquisitionComp_included


