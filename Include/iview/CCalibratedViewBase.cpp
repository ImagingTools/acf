#include "iview/CCalibratedViewBase.h"


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include "i2d/CRectangle.h"

#include "iview/IViewRulersAccessor.h"
#include "iview/IColorShema.h"
#include "iview/IVisualizable.h"


namespace iview
{


CCalibratedViewBase::CCalibratedViewBase(QWidget* parentWidgetPtr)
	:BaseClass2(parentWidgetPtr),
	m_calibrationPtr(&CNoneCalibration::GetInstance()),
	m_isGridVisible(false),
	m_isGridInMm(true),
	m_minGridDistance(10)
{
	m_calibrationLayerIndex = -1;
	m_lastClientArea.Reset();
	m_isBackgroundBufferActive = true;
	m_isDoubleBufferActive = true;
}


void CCalibratedViewBase::ConnectCalibrationShape(iview::IShape* shapePtr)
{
	I_ASSERT(shapePtr != NULL);
	if (GetLayersCount() <= 0){
		InsertDefaultLayers();
	}
	I_ASSERT(m_calibrationLayerIndex >= 0);

	iview::ILayer& layer = GetLayer(m_calibrationLayerIndex);
	layer.ConnectShape(shapePtr);
}


// reimplemented (iview::CViewBase)

void CCalibratedViewBase::UpdateAllShapes(int changeFlag)
{
	if ((changeFlag & iview::CF_TRANSFORM) != 0){
		SetBackgroundBufferValid(false);
	}

	BaseClass::UpdateAllShapes(changeFlag);
}


void CCalibratedViewBase::InsertDefaultLayers()
{
	BaseClass::InsertDefaultLayers();

	int layerIndex = InsertLayer(&m_calibrationLayer, 1, LT_CALIBRATION);
	SetLastBackgroundLayerIndex(layerIndex);
}


// reimplemented (iview::IShapeView)

int CCalibratedViewBase::InsertLayer(iview::ILayer* layerPtr, int index, int layerType)
{
	int result = BaseClass::InsertLayer(layerPtr, index, layerType);

	if ((layerType & LT_CALIBRATION) != 0){
		m_calibrationLayerIndex = result;
	}
	else if (m_calibrationLayerIndex >= result){
		m_calibrationLayerIndex++;
	}

	return result;
}


void CCalibratedViewBase::RemoveLayer(int index)
{
	BaseClass::RemoveLayer(index);

	if (m_calibrationLayerIndex >= index){
		if (m_calibrationLayerIndex == index){
			m_calibrationLayerIndex = -1;
		}
		else{
			m_calibrationLayerIndex--;
		}
	}
}


void CCalibratedViewBase::SetDefaultColorShema(const IColorShema* colorShemaPtr, bool releaseFlag)
{
	m_defaultColorShemaPtr.SetPtr(colorShemaPtr, releaseFlag);
}


bool CCalibratedViewBase::IsBackgroundBufferActive() const
{
	return m_isBackgroundBufferActive;
}


void CCalibratedViewBase::SetBackgroundBufferActive(bool state)
{
	m_isBackgroundBufferActive = state;
}


bool CCalibratedViewBase::IsDoubleBufferActive() const
{
	return m_isDoubleBufferActive;
}


void CCalibratedViewBase::SetDoubleBufferActive(bool state)
{
	m_isDoubleBufferActive = state;
}


const iview::CDrawBuffer& CCalibratedViewBase::GetBackgroundBuffer() const
{
	return m_backgroundBuffer;
}


iview::CDrawBuffer& CCalibratedViewBase::GetBackgroundBuffer()
{
	return m_backgroundBuffer;
}


const iview::CDrawBuffer& CCalibratedViewBase::GetDoubleBuffer() const
{
	return m_doubleBuffer;
}


iview::CDrawBuffer& CCalibratedViewBase::GetDoubleBuffer()
{
	return m_doubleBuffer;
}


// reimplemented (iview::IShapeView)


const iview::IColorShema& CCalibratedViewBase::GetDefaultColorShema() const
{
	if (!m_defaultColorShemaPtr.IsValid()){
		m_defaultColorShemaPtr.SetPtr(new iview::CColorShema, true);
	}

	return *m_defaultColorShemaPtr;
}


// protected methods


void CCalibratedViewBase::DrawBuffers(QPaintDevice& nativeContext, const i2d::CRect& clipRect)
{
	CheckResize();

	if (!m_lastClientArea.IsEmpty()){
		i2d::CRect invalidatedBox = GetInvalidatedBox();
		invalidatedBox.Intersection(m_lastClientArea);

		if (IsDoubleBufferActive()){
			iview::CDrawBuffer& doubleBuffer = GetDoubleBuffer();
			if (!invalidatedBox.IsEmpty()){
				if (!doubleBuffer.IsCreated()){
					doubleBuffer.Create(nativeContext, m_lastClientArea.GetSize());
				}

				QPainter& doubleBufferDC = doubleBuffer.GetDrawContext();

				DrawToContext(nativeContext, doubleBufferDC, invalidatedBox);
			}
			doubleBuffer.CopyRectTo(clipRect, nativeContext, clipRect.GetLeftTop());
		}
		else{
			QPainter canvasDC(&nativeContext);
			invalidatedBox.Union(clipRect);

			DrawToContext(nativeContext, canvasDC, invalidatedBox);
		}

		ResetInvalidatedBox();
	}
}



void CCalibratedViewBase::DrawToContext(
			QPaintDevice& nativeContext,
			QPainter& context,
			const i2d::CRect& invalidatedBox)
{
	I_ASSERT(!invalidatedBox.IsEmpty());
	// TODO: Implement drawing without background buffer

	iview::CDrawBuffer& backgroundBuffer = GetBackgroundBuffer();
	if (!backgroundBuffer.IsCreated()){
		backgroundBuffer.Create(nativeContext, m_lastClientArea.GetSize());
	}

	QPainter& backgroundDC = backgroundBuffer.GetDrawContext();
	const iview::IColorShema& colorShema =  GetDefaultColorShema();

	context.setPen(colorShema.GetPen(iview::IColorShema::SP_NORMAL));
	context.setBrush(colorShema.GetBrush(iview::IColorShema::SB_NORMAL));
	context.setFont(colorShema.GetFont(iview::IColorShema::SF_NORMAL));

	int lastBackgroundLayerIndex = GetLastBackgroundLayerIndex();
	int layersCount = GetLayersCount();

	bool isBackgroundBufferValid = IsBackgroundBufferValid();
	if (!isBackgroundBufferValid){
		backgroundDC.setBrush(colorShema.GetBrush(iview::IColorShema::SB_BACKGROUND));
		backgroundDC.setClipRegion(iqt::GetQRect(m_lastClientArea));
		backgroundDC.fillRect(iqt::GetQRect(m_lastClientArea), colorShema.GetBrush(iview::IColorShema::SB_BACKGROUND));

		if ((lastBackgroundLayerIndex >= 0) && (lastBackgroundLayerIndex < layersCount)){
			backgroundDC.setPen(colorShema.GetPen(iview::IColorShema::SP_NORMAL));
			backgroundDC.setBrush(colorShema.GetBrush(iview::IColorShema::SB_BACKGROUND));
			backgroundDC.setFont(colorShema.GetFont(iview::IColorShema::SF_NORMAL));

			DrawLayers(backgroundDC, 0, lastBackgroundLayerIndex);
		}

		SetBackgroundBufferValid();
	}

	context.setClipRegion(iqt::GetQRect(invalidatedBox));
	backgroundBuffer.CopyRectTo(invalidatedBox, context, invalidatedBox.GetLeftTop());

	I_ASSERT(lastBackgroundLayerIndex >= -1);
	if (lastBackgroundLayerIndex + 1 < layersCount){
		DrawLayers(context, lastBackgroundLayerIndex + 1, layersCount - 1);
	}
}


// events


void CCalibratedViewBase::OnResize()
{
	SetBackgroundBufferValid(false);
	InvalidateBox(m_lastClientArea);

	iview::CDrawBuffer& backgroundBuffer = GetBackgroundBuffer();
	if (backgroundBuffer.IsCreated()){
		backgroundBuffer.Reset();
	}
	iview::CDrawBuffer& doubleBuffer = GetDoubleBuffer();
	if (doubleBuffer.IsCreated()){
		doubleBuffer.Reset();
	}
}



void CCalibratedViewBase::CheckResize()
{
	i2d::CRect clientArea = GetClientRect();

	if (m_lastClientArea != clientArea){
		m_lastClientArea = clientArea;
		InvalidateBox(clientArea);

		OnResize();
	}
}




} // namespace iview


