#ifndef iview_TFramedViewBase_included
#define iview_TFramedViewBase_included


// STL includes
#include <sstream>


// ACF includes
#include "iimg/IBitmap.h"

#include "imath/CFixedPointManip.h"

#include "iview/IViewEventObserver.h"
#include "iview/TFrameBase.h"
#include "iview/CImageShape.h"


namespace iview
{


template <class ViewBase>
class TFramedViewBase:
		public ViewBase,
		virtual public IViewEventObserver
{
public:
	typedef ViewBase BaseClass;
	typedef iview::IViewEventObserver BaseClass2;

	typedef TFrameBase<ViewBase> Frame;

	TFramedViewBase(Frame* framePtr);
	template <typename ConstructorParam>
	TFramedViewBase(Frame* framePtr, ConstructorParam* param)
		:BaseClass(param),
		m_pixelPositionFormatter(0),
		m_logicalPositionFormatter(2)
	{
		m_blockBBoxEvent = false;
		m_framePtr = framePtr;
	
		AddViewEventObserver(this);

		m_lastBoundingBox.Reset();
	}

	virtual ~TFramedViewBase();

	void SetFramework(TFrameBase<ViewBase>* framePtr);

	Frame* GetFramePtr() const;

	void UpdateFitTransform();

	virtual void SetEditMode(int mode);
	virtual void SetFitArea(const i2d::CRectangle& area);

	/**
		Get number of digits after point used to display mouse position in pixel.
	*/
	int GetPixelPositionExact() const;
	
	/**
		Get number of digits after point used to display mouse position in logical units.
	*/
	int GetLogicalPositionExact() const;

	// reimplemented (IViewEventObserver)
	virtual bool OnSelectChange(const iview::IShapeView& view, const istd::CIndex2d& position, const iview::IInteractiveShape& shape, bool state);
	virtual bool OnMouseButton(const iview::IShapeView& view, const istd::CIndex2d& position, Qt::MouseButton buttonType, bool state, const iview::IInteractiveShape* shapePtr);

	// reimplemented (IMouseActionObserver)
	virtual bool OnMouseMove(istd::CIndex2d position);

protected:
	virtual void SetBackgroundBufferValid(bool state = true);
	virtual void OnBoundingBoxChanged();
	virtual void OnResize();
	virtual bool CanBeMoved() const;

	// reimplemented (CViewBase)
	virtual void CalcBoundingBox() const;

	// reimplemented (IDisplay)
	virtual void OnAreaInvalidated(const i2d::CRect& beforeBox, const i2d::CRect& afterBox);

private:
	mutable bool m_blockBBoxEvent;
	mutable i2d::CRect m_lastBoundingBox;

	Frame* m_framePtr;
	imath::CFixedPointManip m_pixelPositionFormatter;
	imath::CFixedPointManip m_logicalPositionFormatter;

	friend typename Frame;
};


// public methods

template <class ViewBase>
TFramedViewBase<ViewBase>::TFramedViewBase(Frame* framePtr)
	:	m_pixelPositionFormatter(0),
		m_logicalPositionFormatter(2)
{
	m_framePtr = framePtr;
	m_blockBBoxEvent = false;
	AddViewEventObserver(this);
	m_lastBoundingBox.Reset();
}


template <class ViewBase>
TFramedViewBase<ViewBase>::~TFramedViewBase()
{
	m_framePtr = NULL;
}


template <class ViewBase>
void TFramedViewBase<ViewBase>::SetFramework(Frame* framePtr)
{
	m_framePtr = framePtr;
}


template <class ViewBase>
typename TFramedViewBase<ViewBase>::Frame* TFramedViewBase<ViewBase>::GetFramePtr() const
{
	return m_framePtr;
}


template <class ViewBase>
void TFramedViewBase<ViewBase>::UpdateFitTransform()
{
	if (m_framePtr->m_isZoomToFit){
		m_framePtr->m_isZoomToFit = false;
		switch (m_framePtr->m_fitMode){
		case Frame::FM_RESET:
			SetZoom(iview::CViewBase::ZM_RESET);
			break;

		case Frame::FM_BOTH:
			SetZoom(iview::CViewBase::ZM_FIT);
			break;

		case Frame::FM_HORIZONTAL:
			SetZoom(iview::CViewBase::ZM_FIT_H);
			break;

		case Frame::FM_VERTICAL:
			SetZoom(iview::CViewBase::ZM_FIT_V);
			break;

		case Frame::FM_UNPROP:
			SetZoom(iview::CViewBase::ZM_FIT_UNPROP);
			break;

		case Frame::FM_COVER:
			SetZoom(iview::CViewBase::ZM_FIT_COVER);
			break;
		}
		m_framePtr->m_isZoomToFit = true;
	}

	CalcBoundingBox();

	i2d::CRect boundingBox = GetBoundingBox();
	if (boundingBox != GetBoundingBox()){
		OnBoundingBoxChanged();
	}
}


template <class ViewBase>
bool TFramedViewBase<ViewBase>::OnMouseMove(istd::CIndex2d position)
{
	bool retVal = BaseClass::OnMouseMove(position);

	Frame::CursorInfo info;

	// pixel position
	const iview::CScreenTransform& transform = GetTransform();
	info.pixelPos = transform.GetClientPosition(position);

	// color info
	int backgroundLayerIndex = GetBackgroundLayerIndex();
	if (backgroundLayerIndex >= 0){
		const iview::CSingleLayer* layerPtr = dynamic_cast<const iview::CSingleLayer*>(&GetLayer(backgroundLayerIndex));
		if (layerPtr != NULL){
			const iview::CImageShape* backgroundShapePtr = dynamic_cast<const iview::CImageShape*>(layerPtr->GetShapePtr());
			if (m_framePtr->IsPixelValueVisible() && backgroundShapePtr != NULL){
				const iimg::IBitmap* bitmapPtr = dynamic_cast<const iimg::IBitmap*>(backgroundShapePtr->GetModelPtr());
				if (bitmapPtr != NULL){
					istd::CIndex2d bitmapPos = info.pixelPos.ToIndex2d();
					ibase::CSize bitmapSize = bitmapPtr->GetImageSize();
					if (			(bitmapPos.GetX() >= 0) &&
									(bitmapPos.GetY() >= 0) &&
									(bitmapPos.GetX() < bitmapSize.GetX()) &&
									(bitmapPos.GetY() < bitmapSize.GetY())){
						int pixelMode = bitmapPtr->GetPixelFormat();

						I_BYTE* linePtr = (I_BYTE*)bitmapPtr->GetLinePtr(bitmapPos.GetY());

						int x = bitmapPos.GetX();
						
						switch (pixelMode){
							case iimg::IBitmap::PF_GRAY:
							{
								info.pixelBrightness = linePtr[x] * 100 / 255;
								break;
							}

							case iimg::IBitmap::PF_RGB:
							{
								info.red = linePtr[x] * 100 / 255;
								info.green = linePtr[x + 1] * 100 / 255;
								info.blue = linePtr[x + 2] * 100 / 255;
								break;
							}

							case iimg::IBitmap::PF_RGBA:
							{
								info.red = linePtr[x] * 100 / 255;
								info.green = linePtr[x + 1] * 100 / 255;
								info.blue = linePtr[x + 2] * 100 / 255;
								info.alpha = linePtr[x + 3] * 100 / 255;
								break;
							}
						}
					}
				}
			}
		}
	}

	i2d::CVector2d logPosition;
	if (m_framePtr->IsMmPositionVisible()){
		const iview::CScreenTransform& logToScreenTransform = GetLogToScreenTransform();

		const iview::ICalibration& calibration = GetCalibration();
		const iview::IIsomorphicCalibration* isomorphCalibPtr = dynamic_cast<const iview::IIsomorphicCalibration*>(&calibration);
		if (isomorphCalibPtr != NULL){
			isomorphCalibPtr->GetApplyToLog(info.pixelPos, info.logicalPos);
		}
		else{
			info.logicalPos = logToScreenTransform.GetClientPosition(position);
		}
	}

	m_framePtr->UpdateCursorInfo(info);

	return retVal;
}


template <class ViewBase>
void TFramedViewBase<ViewBase>::SetEditMode(int mode)
{
	if (mode != GetEditMode()){
		BaseClass::SetEditMode(mode);
		
		m_framePtr->UpdateEditModeButtons();
	}
}


template <class ViewBase>
void TFramedViewBase<ViewBase>::SetFitArea(const i2d::CRectangle& area)
{
	i2d::CRectangle prevFitArea = GetFitArea();

	BaseClass::SetFitArea(area);

	if ((prevFitArea != area) && m_framePtr->IsZoomToFit()){
		UpdateFitTransform();
	}
}


// reimplement (IViewEventObserver)

template <class ViewBase>
bool TFramedViewBase<ViewBase>::OnSelectChange(
			const iview::IShapeView& view,
			const istd::CIndex2d& position,
			const iview::IInteractiveShape& shape,
			bool state)
{
	return m_framePtr->OnSelectChange(view, position, shape, state);
}


template <class ViewBase>
bool TFramedViewBase<ViewBase>::OnMouseButton(
			const IShapeView& view,
			const istd::CIndex2d& position,
			Qt::MouseButton buttonType,
			bool state,
			const IInteractiveShape* shapePtr)
{
	if (!state){
		m_framePtr->UpdateButtonsState();
	}

	return m_framePtr->OnMouseButton(view, position, buttonType, state, shapePtr);
}


// protected methods

template <class ViewBase>
void TFramedViewBase<ViewBase>::SetBackgroundBufferValid(bool state)
{
	BaseClass::SetBackgroundBufferValid(state);
	if (!state){
		int backgroundLayerIndex = GetBackgroundLayerIndex();
		if (backgroundLayerIndex >= 0){
			const iview::CSingleLayer* layerPtr = dynamic_cast<const iview::CSingleLayer*>(&GetLayer(backgroundLayerIndex));
			if (layerPtr != NULL){
				const iview::CImageShape* backgroundPtr = dynamic_cast<const iview::CImageShape*>(layerPtr->GetShapePtr());
				bool isBkActive = (backgroundPtr != NULL) && !backgroundPtr->GetBoundingBox().IsEmpty();
				if ((m_framePtr != NULL) && (isBkActive != m_framePtr->m_isBkActive)){
					m_framePtr->m_isBkActive = isBkActive;
					if (m_framePtr->m_isZoomToFit){
						if (isBkActive){
							UpdateFitTransform();
						}
						else{
							SetZoom(iview::CViewBase::ZM_RESET);
						}
					}
					m_framePtr->UpdateButtonsState();
				}
			}
		}
	}
}


template <class ViewBase>
void TFramedViewBase<ViewBase>::OnBoundingBoxChanged()
{
	if (m_framePtr != NULL){
		m_framePtr->OnBoundingBoxChanged();
	}
}


template <class ViewBase>
void TFramedViewBase<ViewBase>::OnResize()
{
	BaseClass::OnResize();

	UpdateFitTransform();
	if (m_framePtr != NULL){
		m_framePtr->UpdateComponentsPosition();
	}
}


template <class ViewBase>
bool TFramedViewBase<ViewBase>::CanBeMoved() const
{
	if (m_framePtr != NULL){
		return !m_framePtr->m_isZoomToFit && BaseClass::CanBeMoved();
	}

	return false;
}


// reimplemented (iview::CViewBase)

template <class ViewBase>
void TFramedViewBase<ViewBase>::CalcBoundingBox() const
{
	if (!m_blockBBoxEvent){
		m_blockBBoxEvent = true;

		const i2d::CRect& lastBox = GetBoundingBox();
		BaseClass::CalcBoundingBox();
		if (lastBox != GetBoundingBox()){
			const_cast<TFramedViewBase<ViewBase>*>(this)->OnBoundingBoxChanged();
		}

		m_blockBBoxEvent = false;
	}
	else{
		BaseClass::CalcBoundingBox();
	}
}


// reimplemented (iview::IDisplay)

template <class ViewBase>
void TFramedViewBase<ViewBase>::OnAreaInvalidated(const i2d::CRect& beforeBox, const i2d::CRect& afterBox)
{
	BaseClass::OnAreaInvalidated(beforeBox, afterBox);

	if (!m_blockBBoxEvent){
		m_blockBBoxEvent = true;
		i2d::CRect boundingBox = GetBoundingBox();
		if (boundingBox != m_lastBoundingBox){
			OnBoundingBoxChanged();
			m_lastBoundingBox = boundingBox;
		}

		m_blockBBoxEvent = false;
	}
}


} // namespace iview


#endif // !iview_TFramedViewBase_included


