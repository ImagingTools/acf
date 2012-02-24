#ifndef iview_TFrameBase_included
#define iview_TFrameBase_included


// Qt includes
#include <QString>


// ACF includes
#include "istd/IPolymorphic.h"

#include "i2d/CRectangle.h"

#include "iview/IViewEventObserver.h"
#include "iview/CScreenTransform.h"


namespace iview
{


template <class ViewBase>
class TFramedViewBase;


template <class ViewBase>
class TFrameBase: public QWidget
{
public:
	typedef iview::TFramedViewBase<ViewBase> ViewImpl;

	/**
		Control fitting mode, to fit area of interest (AOI) to display.
	*/
	enum FitMode
	{
		/**
			No fitting, frame use always identity transformation.
		*/
		FM_RESET,
		
		/**
			Scale both axes proportional to display biggest, but full visible AOI.
		*/
		FM_BOTH,
		
		/**
			Only horizontal size of AOI will be consider.
		*/
		FM_HORIZONTAL,
		
		/**
			Only vertical size of AOI will be consider.
		*/
		FM_VERTICAL,
		
		/**
			Both axes will be scaled separately.
		*/
		FM_UNPROP,
		
		/**
			Scale both axes proportional to display smallest AOI, which fully covers display.
		*/
		FM_COVER
	};

	TFrameBase(QWidget* parent);

	// console visibility
	bool IsZoomToFit() const;
	void SetZoomToFit(bool state = true);
	FitMode GetFitMode() const;
	void SetFitMode(FitMode mode);
	bool IsGridVisible() const;
	void SetGridVisible(bool state = true);
	bool IsRulerVisible() const;
	void SetRulerVisible(bool state = true);
	bool IsGridInMm() const;
	void SetGridInMm(bool state = true);

	// gui visibility
	bool IsButtonsPanelVisible() const;
	void SetButtonsPanelVisible(bool state = true);
	bool IsButtonsPanelVertical() const;
	void SetButtonsPanelVertical(bool state = true);
	bool AreScrollbarsVisible() const;
	void SetScrollbarsVisible(bool state = true);
	bool IsStatusVisible() const;
	void SetStatusVisible(bool state = true);

	// buttons visibility
	bool AreZoomsVisible() const;
	void SetZoomsVisible(bool state = true);
	bool IsZoomToFitVisible() const;
	void SetZoomToFitVisible(bool state = true);
	bool ArePolylineButtonsVisible() const;
	void SetPolylineButtonsVisible(bool state = true);
	bool AreUserModeButtonsVisible() const;
	void SetUserModeButtonsVisible(bool state = true);
	bool IsStatusButtonVisible() const;
	void SetStatusButtonVisible(bool state = true);
	bool IsScrollbarsButtonVisible() const;
	void SetScrollbarsButtonVisible(bool state = true);
	bool IsGridButtonVisible() const;
	void SetGridButtonVisible(bool state = true);
	bool IsRulerButtonVisible() const;
	void SetRulerButtonVisible(bool state = true);
	bool IsMmButtonVisible() const;
	void SetMmButtonVisible(bool state = true);
	bool IsUndoButtonVisible() const;
	void SetUndoButtonVisible(bool state = true);
	bool IsPixelPositionVisible() const;
	void SetPixelPositionVisible(bool state = true);
	bool IsPixelValueVisible() const;
	void SetPixelValueVisible(bool state = true);
	bool IsMmPositionVisible() const;
	void SetMmPositionVisible(bool state = true);

	/**
		Check if background object is present and active.
	*/
	bool IsBackgroundActive() const;

	void UpdateView();

	// abstract methods

	/**
		Get access to internal view implementation.
	*/
	virtual const ViewImpl& GetView() const = 0;

	/**
		Get access to internal view implementation.
	*/
	virtual ViewImpl& GetViewRef() = 0;

	/**
		Update state of edit mode buttons.
		Edit mode buttons controls the mode of interaction with objects.
		For example for polyline nodes: select, move, insert, remove.
		\sa iview::ISelectable::EditMode
	*/
	virtual void UpdateEditModeButtons() = 0;
	
	/**
		Recalculates enabling flags for buttons.
	*/
	virtual void UpdateButtonsState() = 0;
	
	/**
		Recalculates position of all components.
	*/
	virtual void UpdateComponentsPosition() = 0;

	/**
		Display specified text in status window.
	*/
	virtual void SetStatusText(const QString& message) = 0;

	struct CursorInfo
	{
		CursorInfo()
		:	pixelPos(), logicalPos(),
			pixelBrightness(-1), 
			red(-1), green(-1), blue(-1)
		{}

		i2d::CVector2d pixelPos;
		i2d::CVector2d logicalPos;
		int pixelBrightness;
		int red;
		int green;
		int blue;
		int alpha;
	};

	virtual void UpdateCursorInfo(const CursorInfo& info) = 0;

protected:
	// events
	virtual bool OnSelectChange(const iview::IShapeView& view, const istd::CIndex2d& position, const iview::IInteractiveShape& shape, bool state) = 0;
	virtual bool OnMouseButton(const iview::IShapeView& view, const istd::CIndex2d& position, Qt::MouseButton buttonType, bool state, const iview::IInteractiveShape* shapePtr) = 0;
	virtual void OnBoundingBoxChanged() = 0;

private:
	void Init();

	FitMode m_fitMode;
	bool m_isRulerVisible;

	bool m_areScollbarsVisible;
	bool m_isStatusVisible;
	bool m_isButtonsPanelVisible;
	bool m_isButtonsPanelVertical;

	bool m_areZoomsVisible;
	bool m_isZoomToFitVisible;
	bool m_arePolylineButtonsVisible;
	bool m_areUserModeButtonsVisible;
	bool m_isStatusButtonVisible;
	bool m_isScrollbarsButtonVisible;
	bool m_isGridButtonVisible;
	bool m_isRulerButtonVisible;
	bool m_isMmButtonVisible;
	bool m_isUndoButtonVisible;

	bool m_isPixelPositionVisible;
	bool m_isPixelValueVisible;
	bool m_isMmPositionVisible;

	iview::CScreenTransform m_storedTransform;

	bool m_isZoomToFit;

	bool m_isBkActive;

	friend typename ViewImpl;
};


// public methods

template <class ViewBase>
TFrameBase<ViewBase>::TFrameBase(QWidget* parent)
	:QWidget(parent)
{
	Init();
}


template <class ViewBase>
void TFrameBase<ViewBase>::SetZoomToFit(bool state)
{
	if (state != m_isZoomToFit){
		m_isZoomToFit = state;
		ViewImpl& view = GetViewRef();
		if (m_isZoomToFit){
			m_storedTransform = view.GetTransform();
			UpdateComponentsPosition();
			view.UpdateFitTransform();
		}
		else{
			UpdateComponentsPosition();
			view.SetTransform(m_storedTransform);
		}
		UpdateButtonsState();
	}
}


template <class ViewBase>
void TFrameBase<ViewBase>::SetFitMode(FitMode mode)
{
	if (m_fitMode != mode){
		m_fitMode = mode;
		ViewImpl& view = GetViewRef();
		view.UpdateFitTransform();
		if (m_isZoomToFit){
			UpdateComponentsPosition();
		}
	}
}


template <class ViewBase>
void TFrameBase<ViewBase>::SetGridVisible(bool state)
{
	ViewImpl& view = GetViewRef();
	view.SetGridVisible(state);
}


template <class ViewBase>
void TFrameBase<ViewBase>::SetRulerVisible(bool state)
{
	if (state != m_isRulerVisible){
		m_isRulerVisible = state;
		UpdateComponentsPosition();
		UpdateButtonsState();
	}
}


template <class ViewBase>
void TFrameBase<ViewBase>::SetGridInMm(bool state)
{
	ViewImpl& view = GetViewRef();

	if (state != view.IsGridInMm()){
		view.SetGridInMm(state);
		UpdateButtonsState();
	}
}


// UI visibility

template <class ViewBase>
void TFrameBase<ViewBase>::SetButtonsPanelVisible(bool state)
{
	if (state != m_isButtonsPanelVisible){
		m_isButtonsPanelVisible = state;
		UpdateComponentsPosition();
	}
}


template <class ViewBase>
void TFrameBase<ViewBase>::SetButtonsPanelVertical(bool state)
{
	if (state != m_isButtonsPanelVertical){
		m_isButtonsPanelVertical = state;
		UpdateComponentsPosition();
	}
}


template <class ViewBase>
void TFrameBase<ViewBase>::SetScrollbarsVisible(bool state)
{
	if (state != m_areScollbarsVisible){
		m_areScollbarsVisible = state;
		UpdateComponentsPosition();
		UpdateButtonsState();
	}
}


template <class ViewBase>
void TFrameBase<ViewBase>::SetStatusVisible(bool state)
{
	if (state != m_isStatusVisible){
		m_isStatusVisible = state;
		UpdateComponentsPosition();
		UpdateButtonsState();
	}
}


// Buttons visibility

template <class ViewBase>
void TFrameBase<ViewBase>::SetZoomsVisible(bool state)
{
	if (state != m_areZoomsVisible){
		m_areZoomsVisible = state;
		UpdateComponentsPosition();
	}
}


template <class ViewBase>
void TFrameBase<ViewBase>::SetZoomToFitVisible(bool state)
{
	if (state != m_isZoomToFitVisible){
		m_isZoomToFitVisible = state;
		UpdateComponentsPosition();
	}
}


template <class ViewBase>
void TFrameBase<ViewBase>::SetPolylineButtonsVisible(bool state)
{
	if (state != m_arePolylineButtonsVisible){
		m_arePolylineButtonsVisible = state;
		UpdateComponentsPosition();
	}
}


template <class ViewBase>
void TFrameBase<ViewBase>::SetUserModeButtonsVisible(bool state)
{
	if (state != m_areUserModeButtonsVisible){
		m_areUserModeButtonsVisible = state;
		UpdateComponentsPosition();
	}
}


template <class ViewBase>
void TFrameBase<ViewBase>::SetStatusButtonVisible(bool state)
{
	if (state != m_isStatusButtonVisible){
		m_isStatusButtonVisible = state;
		UpdateComponentsPosition();
	}
}


template <class ViewBase>
void TFrameBase<ViewBase>::SetScrollbarsButtonVisible(bool state)
{
	if (state != m_isScrollbarsButtonVisible){
		m_isScrollbarsButtonVisible = state;
		UpdateComponentsPosition();
	}
}


template <class ViewBase>
void TFrameBase<ViewBase>::SetGridButtonVisible(bool state)
{
	if (state != m_isGridButtonVisible){
		m_isGridButtonVisible = state;
		UpdateComponentsPosition();
	}
}


template <class ViewBase>
void TFrameBase<ViewBase>::SetRulerButtonVisible(bool state)
{
	if (state != m_isRulerButtonVisible){
		m_isRulerButtonVisible = state;
		UpdateComponentsPosition();
	}
}


template <class ViewBase>
void TFrameBase<ViewBase>::SetMmButtonVisible(bool state)
{
	if (state != m_isMmButtonVisible){
		m_isMmButtonVisible = state;
		UpdateComponentsPosition();
	}
}


template <class ViewBase>
void TFrameBase<ViewBase>::SetUndoButtonVisible(bool state)
{
	if (state != m_isUndoButtonVisible){
		m_isUndoButtonVisible = state;
		UpdateComponentsPosition();
	}
}


template <class ViewBase>
void TFrameBase<ViewBase>::SetPixelPositionVisible(bool state)
{
	if (state != m_isPixelPositionVisible){
		m_isPixelPositionVisible = state;
	}
}


template <class ViewBase>
void TFrameBase<ViewBase>::SetPixelValueVisible(bool state)
{
	if (state != m_isPixelValueVisible){
		m_isPixelValueVisible = state;
	}
}


template <class ViewBase>
void TFrameBase<ViewBase>::SetMmPositionVisible(bool state)
{
	if (state != m_isMmPositionVisible){
		m_isMmPositionVisible = state;
	}
}


template <class ViewBase>
void TFrameBase<ViewBase>::UpdateView()
{
	ViewImpl& view = GetViewRef();
	if (m_isZoomToFit){
		m_storedTransform = view.GetTransform();
		UpdateComponentsPosition();
		view.UpdateFitTransform();
	}

	view.Update();
}


// private methods

template <class ViewBase>
void TFrameBase<ViewBase>::Init()
{
	m_isButtonsPanelVisible = true;
	m_isButtonsPanelVertical = false;
	m_areScollbarsVisible = true;
	m_areZoomsVisible = true;
	m_isZoomToFitVisible = true;
	m_arePolylineButtonsVisible = true;
	m_areUserModeButtonsVisible = false;
	m_isStatusVisible = true;
	m_isStatusButtonVisible = true;
	m_isScrollbarsButtonVisible = true;
	m_isGridButtonVisible = true;
	m_isRulerButtonVisible = true;
	m_isRulerVisible = false;
	m_isMmButtonVisible = true;
	m_isUndoButtonVisible = true;

	m_isPixelPositionVisible = true;
	m_isPixelValueVisible = true;
	m_isMmPositionVisible = false;

	m_isBkActive = false;
	m_isZoomToFit = false;
	m_fitMode = FM_BOTH;
}


// inline methods

template <class ViewBase>
inline bool TFrameBase<ViewBase>::IsZoomToFit() const
{
	return m_isZoomToFit;
}


template <class ViewBase>
inline typename TFrameBase<ViewBase>::FitMode TFrameBase<ViewBase>::GetFitMode() const
{
	return m_fitMode;
}


template <class ViewBase>
inline bool TFrameBase<ViewBase>::IsGridVisible() const
{
	const ViewImpl& view = GetView();
	return view.IsGridVisible();
}


template <class ViewBase>
inline bool TFrameBase<ViewBase>::IsRulerVisible() const
{
	return m_isRulerVisible;
}


template <class ViewBase>
inline bool TFrameBase<ViewBase>::IsGridInMm() const
{
	const ViewImpl& view = GetView();
	return view.IsGridInMm();
}


// gui visibility

template <class ViewBase>
inline bool TFrameBase<ViewBase>::IsButtonsPanelVisible() const
{
	return m_isButtonsPanelVisible;
}


template <class ViewBase>
inline bool TFrameBase<ViewBase>::IsButtonsPanelVertical() const
{
	return m_isButtonsPanelVertical;
}


template <class ViewBase>
inline bool TFrameBase<ViewBase>::AreScrollbarsVisible() const
{
	return m_areScollbarsVisible;
}


template <class ViewBase>
inline bool TFrameBase<ViewBase>::IsStatusVisible() const
{
	return m_isStatusVisible;
}


// buttons visibility

template <class ViewBase>
inline bool TFrameBase<ViewBase>::AreZoomsVisible() const
{
	return m_areZoomsVisible;
}


template <class ViewBase>
inline bool TFrameBase<ViewBase>::IsZoomToFitVisible() const
{
	return m_isZoomToFitVisible;
}


template <class ViewBase>
inline bool TFrameBase<ViewBase>::ArePolylineButtonsVisible() const
{
	return m_arePolylineButtonsVisible;
}


template <class ViewBase>
inline bool TFrameBase<ViewBase>::AreUserModeButtonsVisible() const
{
	return m_areUserModeButtonsVisible;
}


template <class ViewBase>
inline bool TFrameBase<ViewBase>::IsStatusButtonVisible() const
{
	return m_isStatusButtonVisible;
}


template <class ViewBase>
inline bool TFrameBase<ViewBase>::IsScrollbarsButtonVisible() const
{
	return m_isScrollbarsButtonVisible;
}


template <class ViewBase>
inline bool TFrameBase<ViewBase>::IsGridButtonVisible() const
{
	return m_isGridButtonVisible;
}


template <class ViewBase>
inline bool TFrameBase<ViewBase>::IsRulerButtonVisible() const
{
	return m_isRulerButtonVisible;
}


template <class ViewBase>
inline bool TFrameBase<ViewBase>::IsMmButtonVisible() const
{
	return m_isMmButtonVisible;
}


template <class ViewBase>
inline bool TFrameBase<ViewBase>::IsUndoButtonVisible() const
{
	return m_isUndoButtonVisible;
}


template <class ViewBase>
inline bool TFrameBase<ViewBase>::IsPixelPositionVisible() const
{
	return m_isPixelPositionVisible;
}


template <class ViewBase>
inline bool TFrameBase<ViewBase>::IsPixelValueVisible() const
{
	return m_isPixelValueVisible;
}


template <class ViewBase>
inline bool TFrameBase<ViewBase>::IsMmPositionVisible() const
{
	return m_isMmPositionVisible;
}


template <class ViewBase>
inline bool TFrameBase<ViewBase>::IsBackgroundActive() const
{
	return m_isBkActive;
}


} // namespace iview


#endif // !iview_TFrameBase_included

