#ifndef iqt_CFireGrabAcquisitionComp_included
#define iqt_CFireGrabAcquisitionComp_included


// FireGrab includes
#include <basetype.h>
#include <fgcamera.h>


#include "iproc/TSyncProcessorWrap.h"

#include "icam/IBitmapAcquisition.h"

#include "icomp/CComponentBase.h"

#include "iavt/iavt.h"


namespace iavt
{


/**
	Bitmap loader component implementing interfaces \c iser::IFileLoader and \c icam::IBitmapAcquisition.
*/
class CFireGrabAcquisitionComp:
			public icomp::CComponentBase,
			virtual public iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CFireGrabAcquisitionComp)
		I_REGISTER_INTERFACE(icam::IBitmapAcquisition)
	I_END_COMPONENT

	CFireGrabAcquisitionComp();
	virtual ~CFireGrabAcquisitionComp();

	// reimplemented (iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>)
	virtual int DoSyncProcess(const iprm::IParamsSet* paramsPtr, const isys::ITimer* inputPtr, iimg::IBitmap* outputPtr);

	// reimplemented (icam::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

private:
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


