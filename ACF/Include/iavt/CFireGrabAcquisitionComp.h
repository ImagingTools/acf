#ifndef iqt_CFireGrabAcquisitionComp_included
#define iqt_CFireGrabAcquisitionComp_included


// FireGrab includes
#include <basetype.h>
#include <fgcamera.h>


#include "ibase/TMessageProducerWrap.h"

#include "iproc/TSyncProcessorWrap.h"

#include "icam/IBitmapAcquisition.h"
#include "icam/IExposureConstraints.h"
#include "icam/IExposureParams.h"


#include "icomp/CComponentBase.h"

#include "iavt/IAvtTriggerParams.h"


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

	I_BEGIN_COMPONENT(CFireGrabAcquisitionComp);
		I_REGISTER_INTERFACE(icam::IBitmapAcquisition);
		I_REGISTER_INTERFACE(icam::IExposureConstraints);
		I_ASSIGN(m_defaultExposureParamsCompPtr, "DefaultExposureParams", "Default exposure parameters will be used if no parameters are found", false, "DefaultExposureParams");
		I_ASSIGN(m_exposureParamsIdAttrPtr, "ExposureParamsId", "Id used to get exposure parameters from the parameter set", false, "ExposureParams");
		I_ASSIGN(m_triggerParamsCompPtr, "TriggerParams", "Trigger parameters that will be used", false, "TriggerParams");
		I_ASSIGN(m_imageRegionParamsCompPtr, "ImageRegionParams", "Image region parameters that will be used", false, "ImageRegionParams");
		I_ASSIGN(m_singleShootAttrPtr, "UseSingleShoot", "If it is true, only one shot will be done", true, true);
	I_END_COMPONENT;

	enum MessageId
	{
		MI_CANNOT_INIT = 0x2ef3210,
		MI_NO_NODES,
		MI_CANNOT_CONNECT,
		MI_CANNOT_OPEN,
		MI_CANNOT_START,
		MI_CANNOT_SET_SINGLE_SHOT,
		MI_CANNOT_SET_CONTINUOUS,
		MI_CANNOT_SET_TRIGGER,
		MI_CANNOT_SET_IMAGE_AOI
	};

	CFireGrabAcquisitionComp();

	bool IsCameraValid() const;

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

protected:
	/**
		Check whether parameter was set correctly.
	*/
	bool CheckParameter(UINT16 parameterId, UINT32 setValue);

	/**
		Initialize device with the trigger params.
	*/
	void InitializeTriggerParams(const iavt::IAvtTriggerParams& triggerParams);

	/**
		Initialize image region.
	*/
	void InitializeImageRegion(const i2d::CRectangle& imageRegion);
	
private:
	I_REF(icam::IExposureParams, m_defaultExposureParamsCompPtr);
	I_ATTR(istd::CString, m_exposureParamsIdAttrPtr);
	I_REF(iavt::IAvtTriggerParams, m_triggerParamsCompPtr);
	I_REF(i2d::IObject2d, m_imageRegionParamsCompPtr);

	I_ATTR(bool, m_singleShootAttrPtr);

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


