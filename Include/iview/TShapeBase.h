#ifndef iview_TShapeBase_included
#define iview_TShapeBase_included


// ACF includes
#include "imod/CSingleModelObserverBase.h"
#include "iview/IDisplay.h"
#include "iview/IColorShema.h"
#include "iview/ILogicalCoords.h"
#include "iview/IViewLayer.h"
#include "iview/CShapeControl.h"


namespace iview
{


template <class Base>
class TShapeBase:
			virtual public Base,
			public CShapeControl,
			public imod::CSingleModelObserverBase
{
public:
	typedef Base BaseClass;
	typedef CShapeControl BaseClass2;
	typedef imod::CSingleModelObserverBase BaseClass3;

	using BaseClass3::AfterUpdate;
	using BaseClass3::BeforeUpdate;
	using BaseClass3::IsModelAttached;
	using BaseClass3::OnAttached;
	using BaseClass3::OnDetached;

	TShapeBase();
	TShapeBase(const TShapeBase<Base>& shape);
	virtual ~TShapeBase();

	virtual void DisconnectFromModel();
	virtual bool AssignToLayer(int layerType);

	// reimplemented (iview::CShapeControl)
	virtual void Invalidate(int changeFlags = 0);
	virtual iview::IDisplay* GetDisplayPtr() const;

	// reimplemented (iview::IShape)
	virtual int GetLayerType() const;
	imod::IModel* GetShapeModelPtr() const;
	virtual i2d::CRect GetBoundingBox() const;
    virtual void SetUserColorShema(const IColorShema* shemaPtr);
    virtual const IColorShema* GetUserColorShema() const;

	// reimplemented (iview::IVisualizable)
	virtual bool IsVisible() const;
	virtual bool IsDisplayAccepted(const IDisplay& display) const;
	virtual void OnConnectDisplay(IDisplay* displayPtr);
	virtual void OnDisconnectDisplay(IDisplay* displayPtr);
	virtual bool OnDisplayChange(int flags);

	// reimplemented (iview::ITouchable)
	virtual ITouchable::TouchState IsTouched(istd::CIndex2d position) const;
	virtual QString GetShapeDescriptionAt(istd::CIndex2d position) const;

	// reimplemented (imod::IObserver)
	virtual void OnUpdate(int changeFlags, istd::IPolymorphic* updateParamsPtr);

protected:
	/**
		Get display changes mask.
		Only changes there are in mask will be accepted, rest will be ignored.
	*/
	virtual int GetDisplayChangesMask();
	
	/**
		Invalidate bounding box.
		You can overload this method to provide validation of your internal attributes,
		because this method is called from Invalidate(int) and OnDisplayChange(int) methods.
		Please don't forget base method call in your implementations.
	*/
	virtual void InvalidateBoundingBox();

	/**
		Check if display is connected.
	*/
	bool IsDisplayConnected() const;

	/**
		Get color shema for this shape.
		It can be called only when display is connected \sa IsDisplayConnected().
	*/
	const IColorShema& GetColorShema() const;
	
	/**
		Get display client rectangle.
		It can be called only when display is connected \sa IsDisplayConnected().
	*/
	i2d::CRect GetClientRect() const;

	bool IsBoundingBoxValid() const;

	void DisconnectDisplay();

	// reimplemented (iview::CShapeControl)
	virtual const iview::CScreenTransform& GetViewToScreenTransform() const;

	// abstract methods

	/**
		Calculate bounding box.
		You have to implement this method in your shapes implementations.
	*/
	virtual i2d::CRect CalcBoundingBox() const = 0;

private:
	const IColorShema* m_userColorShemaPtr;
	IDisplay* m_displayPtr;
	mutable bool m_isBoundingBoxValid;
	mutable i2d::CRect m_boundingBox;
	int m_layerType;
};


// public methods

// reimplemented (iview::CShapeControl)

template <class Base>
void TShapeBase<Base>::Invalidate(int /*changeFlags*/)
{
	InvalidateBoundingBox();

	if (m_displayPtr != NULL){
		m_displayPtr->OnChangeShape(this);
	}
}


template <class Base>
inline IDisplay* TShapeBase<Base>::GetDisplayPtr() const
{
	return m_displayPtr;
}


// reimplemented (iview::IShape)
template <class Base>
int TShapeBase<Base>::GetLayerType() const
{
	return m_layerType;
}

template <class Base>
imod::IModel* TShapeBase<Base>::GetShapeModelPtr() const
{
	return BaseClass3::GetModelPtr();
}


// reimplemented (iview::IShape)

// inline protected methods

template <class Base>
inline int TShapeBase<Base>::GetDisplayChangesMask()
{
	return CF_TRANSFORM | CF_COLORS;
}


template <class Base>
inline void TShapeBase<Base>::InvalidateBoundingBox()
{
	InvalidateTransforms();
	m_isBoundingBoxValid = false;
}


template <class Base>
inline bool TShapeBase<Base>::IsBoundingBoxValid() const
{
	return m_isBoundingBoxValid;
}


// reimplemented (iview::IShape)

template <class Base>
inline const IColorShema* TShapeBase<Base>::GetUserColorShema() const
{
	return m_userColorShemaPtr;
}


template <class Base>
bool TShapeBase<Base>::IsDisplayConnected() const
{
	I_ASSERT((m_displayPtr == NULL) || IsDisplayAccepted(*m_displayPtr));

	return (m_displayPtr != NULL);
}


// reimplemented (iview::IVisualizable)

template <class Base>
inline bool TShapeBase<Base>::IsVisible() const
{
	return BaseClass2::IsVisible();
}


template <class Base>
inline bool TShapeBase<Base>::IsDisplayAccepted(const IDisplay& /*display*/) const
{
	return true;
}


// non-inline public constructors

template <class Base>
TShapeBase<Base>::TShapeBase()
{
	m_userColorShemaPtr = NULL;
	m_displayPtr = NULL;
	m_isBoundingBoxValid = false;
	
	m_layerType = iview::IViewLayer::LT_INACTIVE;
}


template <class Base>
TShapeBase<Base>::TShapeBase(const TShapeBase<Base>& shape)
	:	Base(shape)
{
	I_ASSERT(shape.m_displayPtr == NULL);	// Copy contructor not allowed for shapes connected to view

	m_userColorShemaPtr = shape.m_userColorShemaPtr;
	m_displayPtr = NULL;
	m_isVisible = shape.m_isVisible;
	m_isBoundingBoxValid = false;
	m_shapeTransformMode = shape.m_shapeTransformMode;
	m_shapeTransform.Reset();
	m_layerType = shape.m_layerType;
}


template <class Base>
TShapeBase<Base>::~TShapeBase()
{
	SetVisible(false);

	if (m_displayPtr != NULL){
		m_displayPtr->DisconnectShape(this);
	}
}


// non-inline public methods

template <class Base>
void TShapeBase<Base>::DisconnectFromModel()
{
	imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		modelPtr->DetachObserver(this);
	}
}


template <class Base>
bool TShapeBase<Base>::AssignToLayer(int layerType)
{
	if (m_displayPtr != NULL){
		return false;
	}

	m_layerType = layerType;

	return true;
}


// reimplemented (iview::IShape)

template <class Base>
void TShapeBase<Base>::OnUpdate(int changeFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	Invalidate(changeFlags);
}


// reimplemented (iview::IShape)

template <class Base>
i2d::CRect TShapeBase<Base>::GetBoundingBox() const
{
	if (!m_isBoundingBoxValid){
		if (IsVisible()){
			m_boundingBox = CalcBoundingBox();
		}
		else{
			m_boundingBox.Reset();
		}

		m_isBoundingBoxValid = true;
	}

	return m_boundingBox;
}


template <class Base>
void TShapeBase<Base>::SetUserColorShema(const IColorShema* shemaPtr)
{
	m_userColorShemaPtr = shemaPtr;
	if (m_displayPtr != NULL){
		m_displayPtr->OnChangeShape(this);
	}
}


// reimplemented (iview::IVisualizable)

template <class Base>
void TShapeBase<Base>::OnConnectDisplay(IDisplay* displayPtr)
{
	I_ASSERT(displayPtr != NULL);
	I_ASSERT(m_displayPtr == NULL);
	I_ASSERT(IsDisplayAccepted(*displayPtr));

	m_displayPtr = displayPtr;
}


template <class Base>
void TShapeBase<Base>::OnDisconnectDisplay(IDisplay* I_IF_DEBUG(displayPtr))
{
	I_ASSERT(displayPtr != NULL);
	I_ASSERT(displayPtr == m_displayPtr);
	m_displayPtr = NULL;
}


template <class Base>
bool TShapeBase<Base>::OnDisplayChange(int flags)
{
	if (flags & GetDisplayChangesMask()){
		InvalidateBoundingBox();
		if ((flags & CF_TRANSFORM) != 0){
			InvalidateTransforms();
		}
		return true;
	}
	return false;
}


// reimplemented (iview::ITouchable)
template <class Base>
ITouchable::TouchState TShapeBase<Base>::IsTouched(istd::CIndex2d /*position*/) const
{
	return TS_NONE;
}


template <class Base>
QString TShapeBase<Base>::GetShapeDescriptionAt(istd::CIndex2d /*position*/) const
{
	return "";
}


// protected methods

template <class Base>
const IColorShema& TShapeBase<Base>::GetColorShema() const
{
	I_ASSERT(IsDisplayConnected());

	if (m_userColorShemaPtr != NULL){
		return *m_userColorShemaPtr;
	}
	else{
		return m_displayPtr->GetColorShema();
	}
}


template <class Base>
i2d::CRect TShapeBase<Base>::GetClientRect() const
{
	I_ASSERT(IsDisplayConnected());

	return m_displayPtr->GetClientRect();
}


template <class Base>
void TShapeBase<Base>::DisconnectDisplay()
{
	if (m_displayPtr != NULL){
		m_displayPtr->DisconnectShape(this);
		m_displayPtr = NULL;
	}
}


// reimplemented (iview::CShapeControl)

template <class Base>
const iview::CScreenTransform& TShapeBase<Base>::GetViewToScreenTransform() const
{
	I_ASSERT(IsDisplayConnected());

	return m_displayPtr->GetTransform();
}


} // namespace iview


#endif // !iview_TShapeBase_included


