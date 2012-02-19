#ifndef iview_TViewImpl_included
#define iview_TViewImpl_included


// Qt includes
#include <QPainter>


// ACF includes
#include "istd/TOptDelPtr.h"

#include "i2d/CRect.h"

#include "iview/CDrawBuffer.h"
#include "iview/CColorShema.h"


namespace iview
{


template <class Base>
class TViewImpl: public Base
{
public:
	TViewImpl();

	/**
		Set default color shema object.
		\param	colorShemaPtr	pointer to color shema object.
		\param	releaseFlag		if its true, object will be automatically removed.
	*/
	void SetDefaultColorShema(const IColorShema* colorShemaPtr, bool releaseFlag = false);

	bool IsBackgroundBufferActive() const;
	void SetBackgroundBufferActive(bool state = true);
	bool IsDoubleBufferActive() const;
	void SetDoubleBufferActive(bool state = true);

	const iview::CDrawBuffer& GetBackgroundBuffer() const;
	iview::CDrawBuffer& GetBackgroundBuffer();
	const iview::CDrawBuffer& GetDoubleBuffer() const;
	iview::CDrawBuffer& GetDoubleBuffer();

	// reimplemented (iview::CViewBase)
	virtual const iview::IColorShema& GetDefaultColorShema() const;

protected:
	virtual void DrawBuffers(QPaintDevice& nativeContext, const i2d::CRect& clipRect);

	virtual void DrawToContext(QPaintDevice& nativeContext, QPainter& context, const i2d::CRect& invalidatedBox);

	/**
		Called if found that display area was resized.
	*/
	virtual void OnResize();
	
	/**
		Check if size size was changed and invalidation and OnResize() should be done.
	*/
	void CheckResize();

	mutable istd::TOptDelPtr<const IColorShema> m_defaultColorShemaPtr;

private:
	iview::CDrawBuffer m_backgroundBuffer;
	iview::CDrawBuffer m_doubleBuffer;

	bool m_isBackgroundBufferActive;
	bool m_isDoubleBufferActive;

	i2d::CRect m_lastClientArea;
};


// inline public methods

template <class Base>
inline void TViewImpl<Base>::SetDefaultColorShema(const IColorShema* colorShemaPtr, bool releaseFlag)
{
	m_defaultColorShemaPtr.SetPtr(colorShemaPtr, releaseFlag);
}


template <class Base>
inline bool TViewImpl<Base>::IsBackgroundBufferActive() const
{
	return m_isBackgroundBufferActive;
}


template <class Base>
inline void TViewImpl<Base>::SetBackgroundBufferActive(bool state)
{
	m_isBackgroundBufferActive = state;
}


template <class Base>
inline bool TViewImpl<Base>::IsDoubleBufferActive() const
{
	return m_isDoubleBufferActive;
}


template <class Base>
inline void TViewImpl<Base>::SetDoubleBufferActive(bool state)
{
	m_isDoubleBufferActive = state;
}


template <class Base>
inline const iview::CDrawBuffer& TViewImpl<Base>::GetBackgroundBuffer() const
{
	return m_backgroundBuffer;
}


template <class Base>
inline iview::CDrawBuffer& TViewImpl<Base>::GetBackgroundBuffer()
{
	return m_backgroundBuffer;
}


template <class Base>
inline const iview::CDrawBuffer& TViewImpl<Base>::GetDoubleBuffer() const
{
	return m_doubleBuffer;
}


template <class Base>
inline iview::CDrawBuffer& TViewImpl<Base>::GetDoubleBuffer()
{
	return m_doubleBuffer;
}


// reimplemented (iview::IShapeView)

template <class Base>
inline const iview::IColorShema& TViewImpl<Base>::GetDefaultColorShema() const
{
	if (!m_defaultColorShemaPtr.IsValid()){
		m_defaultColorShemaPtr.SetPtr(new iview::CColorShema, true);
	}

	return *m_defaultColorShemaPtr;
}


// public methods

template <class Base>
TViewImpl<Base>::TViewImpl()
{
	m_lastClientArea.Reset();
	m_isBackgroundBufferActive = true;
	m_isDoubleBufferActive = true;
}


// protected methods

template <class Base>
void TViewImpl<Base>::DrawBuffers(QPaintDevice& nativeContext, const i2d::CRect& clipRect)
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


template <class Base>
void TViewImpl<Base>::DrawToContext(
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

template <class Base>
void TViewImpl<Base>::OnResize()
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


template <class Base>
void TViewImpl<Base>::CheckResize()
{
	i2d::CRect clientArea = GetClientRect();

	if (m_lastClientArea != clientArea){
		m_lastClientArea = clientArea;
		InvalidateBox(clientArea);

		OnResize();
	}
}


} // namespace iview


#endif // !iview_TViewImpl_included



