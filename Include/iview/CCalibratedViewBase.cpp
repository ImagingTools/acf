#include <iview/CCalibratedViewBase.h>


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include <i2d/CRectangle.h>
#include <iview/IViewRulersAccessor.h>
#include <iview/IColorSchema.h>
#include <iview/IVisualizable.h>


namespace iview
{


// public methods

CCalibratedViewBase::CCalibratedViewBase()
:	m_calibrationPtr(NULL),
	m_isGridVisible(false),
	m_isGridInMm(true),
	m_minGridDistance(10)
{
	m_calibrationLayerIndex = -1;
	m_lastClientArea.Reset();
	m_isBackgroundBufferActive = true;
	m_isDoubleBufferActive = true;
}


void CCalibratedViewBase::SetDisplayCalibration(const i2d::ICalibration2d* calibrationPtr)
{
	if (calibrationPtr != m_calibrationPtr){
		m_calibrationPtr = calibrationPtr;

		InvalidateBackground();

		static const istd::IChangeable::ChangeSet changeSet(IShape::CF_CALIB);
		UpdateAllShapes(changeSet);
	}
}


void CCalibratedViewBase::ConnectCalibrationShape(iview::IShape* shapePtr)
{
	Q_ASSERT(shapePtr != NULL);
	if (GetLayersCount() <= 0){
		InsertDefaultLayers();
	}
	Q_ASSERT(m_calibrationLayerIndex >= 0);

	iview::IViewLayer& layer = GetLayer(m_calibrationLayerIndex);
	layer.ConnectShape(shapePtr);
}


// reimplemented (iview::CViewBase)

void CCalibratedViewBase::UpdateAllShapes(const istd::IChangeable::ChangeSet& changeSet)
{
	if (changeSet.Contains(CF_TRANSFORM)){
		SetBackgroundBufferValid(false);
	}

	BaseClass::UpdateAllShapes(changeSet);
}


void CCalibratedViewBase::InsertDefaultLayers()
{
	BaseClass::InsertDefaultLayers();

	int layerIndex = InsertLayer(&m_calibrationLayer, 1, IViewLayer::LT_CALIBRATION);

	SetLastBackgroundLayerIndex(layerIndex);
}


// reimplemented (iview::IShapeView)

int CCalibratedViewBase::InsertLayer(iview::IViewLayer* layerPtr, int index, int layerType)
{
	int result = BaseClass::InsertLayer(layerPtr, index, layerType);

	if (layerType == IViewLayer::LT_CALIBRATION){
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


void CCalibratedViewBase::SetDefaultColorSchema(const IColorSchema* colorSchemaPtr, bool releaseFlag)
{
	m_defaultColorSchemaPtr.SetPtr(colorSchemaPtr, releaseFlag);
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

const iview::IColorSchema& CCalibratedViewBase::GetDefaultColorSchema() const
{
	if (!m_defaultColorSchemaPtr.IsValid()){
		m_defaultColorSchemaPtr.SetPtr(new iview::CColorSchema, true);
	}

	return *m_defaultColorSchemaPtr;
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
	Q_ASSERT(!invalidatedBox.IsEmpty());
	// TODO: Implement drawing without background buffer

	iview::CDrawBuffer& backgroundBuffer = GetBackgroundBuffer();
	if (!backgroundBuffer.IsCreated()){
		backgroundBuffer.Create(nativeContext, m_lastClientArea.GetSize());
	}

	QPainter& backgroundDC = backgroundBuffer.GetDrawContext();
	const iview::IColorSchema& colorSchema = GetDefaultColorSchema();

	context.setPen(colorSchema.GetPen(iview::IColorSchema::SP_NORMAL));
	context.setBrush(colorSchema.GetBrush(iview::IColorSchema::SB_NORMAL));
	context.setFont(colorSchema.GetFont(iview::IColorSchema::SF_NORMAL));

	int lastBackgroundLayerIndex = GetLastBackgroundLayerIndex();
	int layersCount = GetLayersCount();

	bool isBackgroundBufferValid = IsBackgroundBufferValid();
	if (!isBackgroundBufferValid){
		backgroundDC.setBrush(colorSchema.GetBrush(iview::IColorSchema::SB_BACKGROUND));
		backgroundDC.setClipRegion(iqt::GetQRect(m_lastClientArea));
		backgroundDC.fillRect(iqt::GetQRect(m_lastClientArea), colorSchema.GetBrush(iview::IColorSchema::SB_BACKGROUND));

		if ((lastBackgroundLayerIndex >= 0) && (lastBackgroundLayerIndex < layersCount)){
			backgroundDC.setPen(colorSchema.GetPen(iview::IColorSchema::SP_NORMAL));
			backgroundDC.setBrush(colorSchema.GetBrush(iview::IColorSchema::SB_BACKGROUND));
			backgroundDC.setFont(colorSchema.GetFont(iview::IColorSchema::SF_NORMAL));

			DrawLayers(backgroundDC, 0, lastBackgroundLayerIndex);
		}

		SetBackgroundBufferValid();
	}

	context.setClipRegion(iqt::GetQRect(invalidatedBox));
	backgroundBuffer.CopyRectTo(invalidatedBox, context, invalidatedBox.GetLeftTop());

	Q_ASSERT(lastBackgroundLayerIndex >= -1);
	if (lastBackgroundLayerIndex + 1 < layersCount){
		DrawLayers(context, lastBackgroundLayerIndex + 1, layersCount - 1);
	}
}


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


