#include "iavt/CFireGrabAcquisitionComp.h"


#include <memory.h>

// Qt includes
#include <QFileInfo>
#include <QByteArray>
#include <QImageReader>
#include <QStringList>

#include "istd/TChangeNotifier.h"

#include "iprm/IParamsSet.h"


namespace iavt
{


CFireGrabAcquisitionComp::CFireGrabAcquisitionComp()
:	m_nodesCount(0)
{
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
					status = m_camera.StartDevice();

					if (status == FCE_NOERROR){
						m_isCameraValid = true;

						return;
					}

					m_camera.StopDevice();
				}
				m_camera.CloseCapture();
			}
			m_camera.Disconnect();
		}
		FGExitModule();
	}

	m_isCameraValid = false;
}


CFireGrabAcquisitionComp::~CFireGrabAcquisitionComp()
{
	if (m_isCameraValid){
		m_camera.StopDevice();
		m_camera.CloseCapture();
		m_camera.Disconnect();
		FGExitModule();
	}
}


// reimplemented (iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>)

int CFireGrabAcquisitionComp::DoSyncProcess(const iprm::IParamsSet* paramsPtr, const isys::ITimer* /*inputPtr*/, iimg::IBitmap* outputPtr)
{
	int retVal = TS_INVALID;

	FGFRAME frameInfo;
	UINT32 status = m_camera.GetFrame(&frameInfo, 5000);

	// ... Here you can processs image data in Frame.pData
	if (status == FCE_NOERROR){
		if (outputPtr != NULL){
			istd::CIndex2d size = GetBitmapSize(paramsPtr);
			if (!size.IsSizeEmpty() && (int(frameInfo.Length) >= size.GetProductVolume()) && outputPtr->CreateBitmap(size)){
				I_ASSERT(outputPtr->GetLineBytesCount() >= size.GetX());

				for (int y = 0; y < size.GetY(); ++y){
					memcpy(outputPtr->GetLinePtr(y), frameInfo.pData + y * size.GetX(), size.GetY());
				}

				retVal = TS_OK;
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


} // namespace iavt


