#ifndef iview_CShapeBase_included
#define iview_CShapeBase_included


// ACF includes
#include "imod/TSingleModelObserverBase.h"
#include "imod/CSingleModelObserverBase.h"
#include "iview/IShape.h"
#include "iview/IDisplay.h"
#include "iview/CScreenTransform.h"

namespace iview
{


class CShapeBase:
			public imod::CSingleModelObserverBase,
			virtual public IShape
{
public:
	typedef imod::CSingleModelObserverBase BaseClass;

	/**
		Describe how shape tranformation is interpreted.
	*/
	enum ShapeTransformMode
	{
		/**
			Shape transformation will be ignored, view transformation will be used.
		*/
		STM_VIEW,
		
		/**
			View transformation will be ignored, shape transformation will be used.
		*/
		STM_SHAPE,
		
		/**
			Both transformation will be used, shape transformation will be used as local transformation.
		*/
		STM_COMBINE
	};

	CShapeBase();
	CShapeBase(const CShapeBase& shape);
	virtual ~CShapeBase();

	/**
		Make this shape visible.
	*/
	virtual void SetVisible(bool state = true);
	virtual bool AssignToLayer(int layerType);

	virtual void Invalidate(int changeFlags = 0);

	/**
		Get access to currently connected display.
	*/
	iview::IDisplay* GetDisplayPtr() const;

	/**
		Get shape transformation mode.
		This mode describes which transformation will be used to display shape.
	*/
	ShapeTransformMode GetTransformMode() const;
	
	/**
		Set shape transformation mode.
		This mode describes which transformation will be used to display shape.
	*/
	void SetTransformMode(ShapeTransformMode mode);

	// reimplemented (iview::IShape)
	virtual int GetLayerType() const;
	virtual i2d::CRect GetBoundingBox() const;
	virtual const IColorSchema* GetUserColorSchema() const;
	virtual void SetUserColorSchema(const IColorSchema* schemaPtr);

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
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);
	virtual void OnUpdate(int changeFlags, istd::IPolymorphic* updateParamsPtr);

protected:
	/**
		Get screen position based on logical position.
	*/
	i2d::CVector2d GetScreenPosition(const i2d::CVector2d& logPosition) const;
	/**
		Get logical position based on screen position.
	*/
	i2d::CVector2d GetLogPosition(const i2d::CVector2d& screenPosition) const;

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
		Get color schema for this shape.
		It can be called only when display is connected \sa IsDisplayConnected().
	*/
	const IColorSchema& GetColorSchema() const;
	
	/**
		Get display client rectangle.
		It can be called only when display is connected \sa IsDisplayConnected().
	*/
	i2d::CRect GetClientRect() const;

	bool IsBoundingBoxValid() const;

	void DisconnectDisplay();

	virtual const iview::CScreenTransform& GetViewToScreenTransform() const;

	// abstract methods
	/**
		Calculate bounding box.
		You have to implement this method in your shapes implementations.
	*/
	virtual i2d::CRect CalcBoundingBox() const = 0;

private:
	class CalibrationObserver: public imod::TSingleModelObserverBase<i2d::ICalibration2d>
	{
	public:
		CalibrationObserver(CShapeBase* parentPtr);

		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CShapeBase* m_parentPtr;
	};

	IDisplay* m_displayPtr;

	// calibration management
	CalibrationObserver m_calibrationObserver;

	// display meta information
	const IColorSchema* m_userColorSchemaPtr;

	// managemant of bounding box
	mutable bool m_isBoundingBoxValid;
	mutable i2d::CRect m_boundingBox;

	// attributes
	bool m_isVisible;
	int m_layerType;
	ShapeTransformMode m_shapeTransformMode;
};


} // namespace iview


#endif // !iview_CShapeBase_included


