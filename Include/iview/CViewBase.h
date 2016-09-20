#ifndef iview_CViewBase_included
#define iview_CViewBase_included


// ACF includes
#include <i2d/CRectangle.h>

#include <iview/IDraggable.h>
#include <iview/IMouseActionObserver.h>
#include <iview/CScreenTransform.h>
#include <iview/CViewLayer.h>
#include <iview/CInteractiveViewLayer.h>


class QPainter;


namespace iview
{


class IViewEventObserver;
class ISelectableLayer;
class IInteractiveShape;


class CViewBase:
			virtual public IShapeView,
			virtual public IDraggable,
			protected IMouseActionObserver
{
public:
	enum ZoomMode{
		ZM_RESET,
		ZM_ZOOM_IN,
		ZM_ZOOM_OUT,
		ZM_FIT,
		ZM_FIT_COVER,
		ZM_FIT_UNPROP,
		ZM_FIT_H,
		ZM_FIT_V
	};

	CViewBase();
	virtual ~CViewBase();

	/**
		Set zoom.
	*/
	virtual void SetZoom(ZoomMode zoom);
	
	/**	
		Set edit mode.
	*/
	virtual void SetEditMode(int mode);
	
	/**
		Inform all shapes about changes.
	*/
	virtual void UpdateAllShapes(const istd::IChangeable::ChangeSet& changeSet);
	
	/**
		It sets position to become transformation of client point to specified screen position.
	*/
	virtual void SetScreenPosition(const i2d::CVector2d& client, istd::CIndex2d screen);
	
	/**
		Get area will be used to automatic fit the zoom to view size.
		\sa	SetZoom(ZoomMode)
	*/
	virtual const i2d::CRectangle& GetFitArea() const;

	/**
		Set area will be used to fit in some zoom modes.
		\sa	SetZoom(ZoomMode)
	*/
	virtual void SetFitArea(const i2d::CRectangle& area);
	
	/**
		Turn on/off possibility to move whole view transform.
	*/
	virtual void SetViewDraggable(bool state = true);
	
	/**
		Turn on/off possibility to select more shapes using control keys.
	*/
	virtual void SetMultiselectable(bool state = true);
	
	/**
		Turn on/off possibility to move more selected shapes, when one is moved.
	*/
	virtual void SetDraggable(bool state = true);
	
	/**
		Adds handler to catch view events.
	*/
	virtual void AddViewEventObserver(iview::IViewEventObserver* listenerPtr);
	
	/**	
		Removes handler to catch view events.
	*/
	virtual void RemoveViewEventObserver(iview::IViewEventObserver* listenerPtr);
	
	/**
		Insert default layers, when there is no one user layer.
	*/
	virtual void InsertDefaultLayers();

	/**
		Get index of default background layer.
	*/
	int GetBackgroundLayerIndex() const;

	/**
		Get index of default inactive layer.
	*/
	int GetInactiveLayerIndex() const;

	/**
		Get index of default active layer.
	*/
	int GetActiveLayerIndex() const;

	/**
		Get shape under mouse pointer.
		\return	shape under mouse pointer, or NULL if there is no shape.
	*/
	IInteractiveShape* GetMouseShapePtr() const;
	
	IInteractiveShape* GetFirstActiveShape() const;

	// reimplemented (iview::IShapeView)
	virtual void Update();
	virtual bool IsViewDraggable() const;
	virtual bool IsMultiselectable() const;
	virtual int InsertLayer(IViewLayer* layerPtr, int index = -1, int layerType = IViewLayer::LT_NONE);
	virtual int GetLayerIndex(const IViewLayer& layer) const;
	virtual int GetLayerIndex(int layerType) const;
	virtual void RemoveLayer(int index);
	virtual int GetLayersCount() const;
	virtual IViewLayer& GetLayer(int index) const;
	virtual bool ConnectShape(IShape* shapePtr);
	virtual ISelectableLayer* GetFocusedLayerPtr() const;
	virtual void OnLayerShapeFocused(IInteractiveShape* shapePtr, ISelectableLayer* layerPtr);
	virtual void OnLayerShapeDefocused(IInteractiveShape* shapePtr, ISelectableLayer* layerPtr);
	virtual void OnLayerInvalidated(const IViewLayer& layer, const i2d::CRect& prevArea, const i2d::CRect& newArea);

	// reimplemented (iview::ISelectable)
	virtual int GetSelectedShapesCount() const;
	virtual void InsertSelectedShapes(SelectedShapes& result) const;
	virtual void DeselectAllShapes();
	virtual int GetKeysState() const;
	virtual int GetEditMode() const;
	virtual void OnShapeFocused(IInteractiveShape* /*shapePtr*/)	{}
	virtual void OnShapeDefocused(IInteractiveShape* /*shapePtr*/)	{}

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const;
	virtual QString GetShapeDescriptionAt(istd::CIndex2d position) const;

	// reimplemented (iview::IShapeView)
	virtual void SetTransform(const i2d::CAffine2d& transform);
	virtual void UpdateMousePointer();

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnMouseMove(istd::CIndex2d position);

	// reimplemented (iview::IDisplay)
	virtual IDisplay* GetParentDisplayPtr() const;
	virtual const iview::CScreenTransform& GetTransform() const;
	virtual i2d::CRect GetBoundingBox() const;
	virtual const IColorSchema& GetColorSchema() const;
	virtual void OnAreaInvalidated(const i2d::CRect& prevArea, const i2d::CRect& newArea);

	// reimplemented (iview::IShapeObserver)
	virtual void OnChangeShape(IShape* shapePtr);
	virtual bool DisconnectShape(IShape* shapePtr);
	virtual void OnShapeSelected(IInteractiveShape& shape, bool state = true);

	// reimplemented (iview::IDraggable)
	virtual bool IsDraggable() const;
	virtual void BeginDrag(const istd::CIndex2d& reference);
	virtual void SetDragPosition(const istd::CIndex2d& position);
	virtual void EndDrag();

	// abstract methods
	virtual const IColorSchema& GetDefaultColorSchema() const = 0;

protected:
	typedef QVector<IViewLayer*> Layers;

	/**
		Draw Background layer.
		Standard implementation draws background shape,
		but it is also possible to use this method to display additional
		visual elements like a grid or rulers.
	*/
	virtual void DrawLayers(QPainter& drawContext, int firstLayer, int lastLayer);
	
	/**
		Get last background layer index.
	*/
	int GetLastBackgroundLayerIndex();
	void SetLastBackgroundLayerIndex(int index);
	
	/**
		This method is internal used to check, if drag mode is allowed.
		Normally it return IsViewDraggable(), but in derived class you
		can change its implementation to avoid dragging.
	*/
	virtual bool CanBeMoved() const;

	/**
		Disconnect all shapes from this view.
	*/
	void DisconnectAllShapes();
	
	/**
		Invalidate whole background buffer.
		By next call Update() background buffer will be full repainted.
	*/
	virtual void InvalidateBackground();

	/**
		Invalidate bounding box.
	*/
	virtual void InvalidateBoundingBox();
	/**
		Calculate bounding box if it was invalid.
		\return	true, if the new bounding box was calculated.
	*/
	bool EnsureBoundingBoxValid();
	
	/**
		Calculate bounding box of all shapes.
		It doesn't update current bounding box.
	*/
	virtual i2d::CRect CalcBoundingBox() const;
	/**
		Called when bounding box has been changed.
	*/
	virtual void OnBoundingBoxChanged();

	virtual MousePointerMode CalcMousePointer(istd::CIndex2d position) const;

	/**
		Invalidate stored shape under mouse pointer.
	*/
	virtual void InvalidateMouseShape();

	/**
		Calc shape under mouse pointer.
	*/
	virtual void CalcMouseShape() const;

	/**
		Check if background buffer is valid.
	*/
	virtual bool IsBackgroundBufferValid() const;

	/**
		Set validate flag of background buffer.
	*/
	virtual void SetBackgroundBufferValid(bool state = true);

	/**
		Get bounding box of all invalidated rectangles.
	*/
	const i2d::CRect& GetInvalidatedBox() const;

	/**
		Invalidate rectangle screen area.
		It added this area to other invalidated areas.
	*/
	void InvalidateBox(const i2d::CRect& rect);

	/**
		Reset invalidated box.
	*/
	void ResetInvalidatedBox();

	/**
		Set state of keys.
	*/
	void SetKeysState(int state);

	const Layers& GetLayers() const;
	Layers& GetLayers();

	// abstract methods

	/**
		Set mouse pointer.
		You have to implement it in your iview::CViewBase implementations.
	*/
	virtual void SetMousePointer(MousePointerMode mode) = 0;
	/**	Start updating specified rectangle area.
	*/
	virtual void UpdateRectArea(const i2d::CRect& rect) = 0;

private:
	enum ViewMode
	{
		VM_NONE,
		VM_DRAG,
		VM_MOVE
	};

	bool m_isBackgroundBufferValid;

	iview::CScreenTransform m_transform;

	ViewMode m_viewMode;
	int m_editMode;
	i2d::CVector2d m_moveReference;

	bool m_isMultiselectable;
	bool m_isViewDraggable;
	bool m_isDraggable;

	QVector<iview::IViewEventObserver*> m_viewListeners;

	ISelectableLayer* m_focusedLayerPtr;
	int m_backgroundLayerIndex;
	int m_activeLayerIndex;
	int m_inactiveLayerIndex;

	i2d::CRect m_invalidatedBox;
	i2d::CRectangle m_fitArea;

	Layers m_layers;

	int m_lastBackgroundLayerIndex;

	// default layers
	iview::CViewLayer m_backgroundLayer;
	iview::CViewLayer m_inactiveLayer;
	iview::CInteractiveViewLayer m_activeLayer;

	// help objects
	bool m_isLastMouseButtonDown;
	mutable int m_keysState;
	mutable istd::CIndex2d m_lastMousePosition;
	mutable bool m_isSelectEventActive;

	i2d::CRect m_boundingBox;
	bool m_isBoundingBoxValid;

	mutable IInteractiveShape* m_mouseShapePtr;
	mutable bool m_isMouseShapeValid;

	mutable bool m_blockBBoxEvent;
};


// inline methods

inline const i2d::CRectangle& CViewBase::GetFitArea() const
{
	return m_fitArea;
}


inline void CViewBase::SetFitArea(const i2d::CRectangle& area)
{
	m_fitArea = area;
}


inline void CViewBase::SetViewDraggable(bool state)
{
	m_isViewDraggable = state;
}


inline void CViewBase::SetMultiselectable(bool state)
{
	m_isMultiselectable = state;
}


inline void CViewBase::SetDraggable(bool state)
{
	m_isDraggable = state;
}


inline IInteractiveShape* CViewBase::GetMouseShapePtr() const
{
	if (!m_isMouseShapeValid){
		CalcMouseShape();
		m_isMouseShapeValid = true;
	}
	
	return m_mouseShapePtr;
}


// reimplemented (iview::IShapeView)

inline bool CViewBase::IsViewDraggable() const
{
	return m_isViewDraggable;
}


inline bool CViewBase::IsMultiselectable() const
{
	return m_isMultiselectable;
}


inline int CViewBase::GetLayersCount() const
{
	return int(m_layers.size());
}


inline IViewLayer& CViewBase::GetLayer(int index) const
{
	if (GetLayersCount() <= 0){
		CViewBase* view = const_cast<CViewBase*> (this);
		view->InsertDefaultLayers();
	}

	Q_ASSERT((index >= 0) && (index < int(m_layers.size())));

	IViewLayer* layerPtr = m_layers[index];
	Q_ASSERT(layerPtr != NULL);

	return *layerPtr;
}


// reimplemented (iview::IDisplay)

inline IDisplay* CViewBase::GetParentDisplayPtr() const
{
	return NULL;
}


inline const iview::CScreenTransform& CViewBase::GetTransform() const
{
	return m_transform;
}


inline i2d::CRect CViewBase::GetBoundingBox() const
{
	const_cast<CViewBase*>(this)->EnsureBoundingBoxValid();

	return m_boundingBox;
}


inline const IColorSchema& CViewBase::GetColorSchema() const
{
	return GetDefaultColorSchema();
}


// reimplemented (iview::ISelectable)

inline int CViewBase::GetKeysState() const
{
	return m_keysState;
}


// reimplemented (iview::IDraggable)

inline bool CViewBase::IsDraggable() const
{
	return m_isDraggable;
}


// protected methods

inline int CViewBase::GetLastBackgroundLayerIndex()
{
	return m_lastBackgroundLayerIndex;
}


inline void CViewBase::SetLastBackgroundLayerIndex(int index)
{
	m_lastBackgroundLayerIndex = index;
}


inline int CViewBase::GetBackgroundLayerIndex() const
{
	return m_backgroundLayerIndex;
}


inline int CViewBase::GetInactiveLayerIndex() const
{
	return m_inactiveLayerIndex;
}


inline int CViewBase::GetActiveLayerIndex() const
{
	return m_activeLayerIndex;
}


inline bool CViewBase::IsBackgroundBufferValid() const
{
	return m_isBackgroundBufferValid;
}


inline void CViewBase::SetBackgroundBufferValid(bool state)
{
	if (state != m_isBackgroundBufferValid){
		if (!state){
			m_invalidatedBox = GetClientRect();
		}
		m_isBackgroundBufferValid = state;
	}
}


inline const i2d::CRect& CViewBase::GetInvalidatedBox() const
{
	return m_invalidatedBox;
}


inline void CViewBase::InvalidateBox(const i2d::CRect& rect)
{
	m_invalidatedBox.Union(rect);
}


inline void CViewBase::ResetInvalidatedBox()
{
	m_invalidatedBox.Reset();
}


inline void CViewBase::SetKeysState(int state)
{
	m_keysState = state;
}


inline const CViewBase::Layers& CViewBase::GetLayers() const
{
	return m_layers;
}


inline CViewBase::Layers& CViewBase::GetLayers()
{
	return m_layers;
}


} // namespace iview


#endif // !iview_CViewBase_included


