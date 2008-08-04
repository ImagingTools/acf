#include "iavt/CFireGrabAcquisitionComp.h"


#include <memory.h>

#include "istd/TChangeNotifier.h"

#include "iprm/IParamsSet.h"


namespace iavt
{


CFireGrabAcquisitionComp::CFireGrabAcquisitionComp()
:	m_nodesCount(0), m_isCameraValid(false)
{
}


bool CFireGrabAcquisitionComp::IsCameraValid() const
{
	return m_isCameraValid;
}


// reimplemented (iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>)

int CFireGrabAcquisitionComp::DoProcessing(const iprm::IParamsSet* paramsPtr, const isys::ITimer* /*inputPtr*/, iimg::IBitmap* outputPtr)
{
	I_ASSERT(m_singleShootAttrPtr.IsValid());	// isObligatory is true

	const icam::IExposureParams* exposureParamsPtr = NULL;

	if (paramsPtr != NULL && m_exposureParamsIdAttrPtr.IsValid()){
		exposureParamsPtr = dynamic_cast<const icam::IExposureParams*>(
					paramsPtr->GetParameter((*m_exposureParamsIdAttrPtr).ToString()));
	}

	if (m_defaultExposureParamsCompPtr.IsValid()){
		exposureParamsPtr = m_defaultExposureParamsCompPtr.GetPtr();
	}

	if (exposureParamsPtr != NULL){
		double shutterTime = exposureParamsPtr->GetShutterTime();
		if (shutterTime >= 0){
			m_camera.SetParameter(FGP_SHUTTER, UINT32(shutterTime * 1000000));
		}
	}

	if (*m_singleShootAttrPtr && (m_camera.StartDevice() != FCE_NOERROR)){
		return TS_INVALID;
	}

	int retVal = TS_INVALID;

	FGFRAME frameInfo;
	UINT32 status = m_camera.GetFrame(&frameInfo, 5000);

	// ... Here you can processs image data in Frame.pData
	if (status == FCE_NOERROR){
		if (outputPtr != NULL){
			istd::CIndex2d size = GetBitmapSize(paramsPtr);
			if (!size.IsSizeEmpty() && (int(frameInfo.Length) >= size.GetProductVolume())){
				istd::TChangeNotifier<iimg::IBitmap> bitmapPtr(outputPtr);
				if (bitmapPtr->CreateBitmap(size)){
					I_ASSERT(outputPtr->GetLineBytesCount() >= size.GetX());

					for (int y = 0; y < size.GetY(); ++y){
						memcpy(outputPtr->GetLinePtr(y), frameInfo.pData + y * size.GetX(), size.GetX());
					}

					retVal = TS_OK;
				}
			}
		}
		else{
			retVal = TS_OK;
		}

		m_camera.PutFrame(&frameInfo);
	}

	return retVal;
}


// reimplemented (icam::IBitmapAcquisition)

istd::CIndex2d CFireGrabAcquisitionComp::GetBitmapSize(const iprm::IParamsSet* /*paramsPtr*/) const
{
	if (m_isCameraValid){
		FGPINFO xSize;
		FGPINFO ySize;
		if (		(m_camera.GetParameterInfo(FGP_XSIZE, &xSize) == FCE_NOERROR) &&
					(m_camera.GetParameterInfo(FGP_YSIZE, &ySize) == FCE_NOERROR)){
			return istd::CIndex2d(int(xSize.IsValue), int(ySize.IsValue));
		}
	}

	return istd::CIndex2d(-1, -1);	// unknown size
}


// reimplemented (icam::IExposureConstraints)

istd::CRange CFireGrabAcquisitionComp::GetShutterTimeRange() const
{
	return istd::CRange(0.000001, 0.004095);
}


istd::CRange CFireGrabAcquisitionComp::GetDelayTimeRange() const
{
	return istd::CRange(-1, -1);
}


istd::CRange CFireGrabAcquisitionComp::GetEenDelayRange() const
{
	return istd::CRange(-1, -1);
}


// reimplemented (icomp::IComponent)

void CFireGrabAcquisitionComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	I_ASSERT(m_singleShootAttrPtr.IsValid());	// isObligatory is true

	// Init module
	UINT32 status = FGInitModule(NULL);

	// Get list of connected nodes
	if (status == FCE_NOERROR){
		status = FGGetNodeList(m_nodeInfos, MAX_NODES_COUNT, &m_nodesCount);

		// Connect with node
		if ((status == FCE_NOERROR) && (m_nodesCount > 0)){
			status = m_camera.Connect(&m_nodeInfos[0].Guid);

			// Start DMA logic
			if (status == FCE_NOERROR){
				status = m_camera.OpenCapture();

				// Start image device
				if (status == FCE_NOERROR){
					if (m_externTriggerAttrPtr.IsValid()){
						UINT32 triggerValue = MAKETRIGGER((*m_externTriggerAttrPtr)? 1: 0, 1, 0, 0, 0);

						if (m_camera.SetParameter(FGP_TRIGGER, triggerValue) != FCE_NOERROR){
							SendErrorMessage(MI_CANNOT_SET_TRIGGER, "Cannot set trigger mode");
						}
					}

					if (*m_singleShootAttrPtr){
						if (m_camera.SetParameter(FGP_BURSTCOUNT, 1) == FCE_NOERROR){
							m_isCameraValid = true;

							return;
						}
						else{
							SendErrorMessage(MI_CANNOT_SET_SINGLE_SHOT, "Cannot set single shot mode");
						}
					}
					else{
						if (m_camera.SetParameter(FGP_BURSTCOUNT, BC_INFINITE) != FCE_NOERROR){
							SendErrorMessage(MI_CANNOT_SET_CONTINUOUS, "Cannot set continuous mode");
						}
					}

					status = m_camera.StartDevice();

					if (status == FCE_NOERROR){
						m_isCameraValid = true;

						return;
					}
					else{
						SendErrorMessage(MI_CANNOT_START, "Cannot start grab");
					}

					m_camera.StopDevice();
				}
				else{
					SendErrorMessage(MI_CANNOT_OPEN, "Cannot open capture device");
				}

				m_camera.CloseCapture();
			}
			else{
				SendErrorMessage(MI_CANNOT_CONNECT, "Cannot connect to camera node");
			}

			m_camera.Disconnect();
		}
		else{
			SendErrorMessage(MI_NO_NODES, "No camera nodes");
		}

		FGExitModule();
	}
	else{
		SendErrorMessage(MI_CANNOT_INIT, "Cannot init Fire Grab module");
	}

	m_isCameraValid = false;
}


void CFireGrabAcquisitionComp::OnComponentDestroyed()
{
	if (m_isCameraValid){
		m_camera.StopDevice();
		m_camera.CloseCapture();
		m_camera.Disconnect();
		FGExitModule();

		m_isCameraValid = false;
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace iavt


