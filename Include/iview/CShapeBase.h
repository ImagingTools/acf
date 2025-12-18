#ifndef iview_CShapeBase_included
#define iview_CShapeBase_included


// ACF includes
#include <imod/TSingleModelObserverBase.h>
#include <imod/CSingleModelObserverBase.h>
#include <iview/IShape.h>
#include <iview/IDisplay.h>
#include <iview/CScreenTransform.h>


namespace iview
{


class IColorSchema;


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
		Assign this shape to same layer.
		See \c iview::IViewLayer::LayerType for layer types.
	*/
	virtual bool AssignToLayer(int layerType);

	virtual void Invalidate();

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

	/**
		Get screen position based on logical position.
	*/
	i2d::CVector2d GetScreenPosition(const i2d::CVector2d& logPosition) const;

	/**
		Get logical position based on screen position.
	*/
	i2d::CVector2d GetLogPosition(const i2d::CVector2d& screenPosition) const;

	// reimplemented (iview::IShape)
	virtual int GetLayerType() const override;
	virtual i2d::CRect GetBoundingBox() const override;
	virtual void SetVisible(bool state = true) override;
	virtual const IColorSchema* GetUserColorSchema() const override;
	virtual void SetUserColorSchema(const IColorSchema* schemaPtr) override;
	virtual void SetDefaultDescription(const QString& description) override;
	virtual void SetToolTip(const QString& toolTip) override;
	virtual bool IsInside(const istd::CIndex2d& screenPosition) const override;

	// reimplemented (iview::IVisualizable)
	virtual bool IsVisible() const override;
	virtual bool IsDisplayAccepted(const IDisplay& display) const override;
	virtual void OnConnectDisplay(IDisplay* displayPtr) override;
	virtual void OnDisconnectDisplay(IDisplay* displayPtr) override;
	virtual bool OnDisplayChange(const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (iview::ITouchable)
	virtual ITouchable::TouchState IsTouched(istd::CIndex2d position) const override;
	virtual QString GetShapeDescriptionAt(istd::CIndex2d position) const override;
	virtual QString GetToolTipAt(istd::CIndex2d position) const override;

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask) override;
	virtual bool OnModelDetached(imod::IModel* modelPtr) override;
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet) override;

protected:
	/**
		Get line with (in logical coordinates) for single screen position.
	*/
	double GetLocalLineWidth(const i2d::CVector2d& screenPosition) const;

	/**
		Get display changes mask.
		Only changes there are in mask will be accepted, rest will be ignored.
	*/
	virtual bool IsDisplayChangeImportant(const istd::IChangeable::ChangeSet& changeSet);

	/**
		Invalidate bounding box.
		You can overload this method to provide validation of your internal attributes,
		because this method is called from \c Invalidate and \c OnDisplayChange methods.
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
	/**
		\internal
		Observes changes of calibration to force shape update if calibration has changed.
	*/
	class CalibrationObserver: public imod::TSingleModelObserverBase<i2d::ICalibration2d>
	{
	public:
		explicit CalibrationObserver(CShapeBase* parentPtr);

		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet) override;

	private:
		CShapeBase* m_parentPtr = nullptr;
	};

	IDisplay* m_displayPtr = nullptr;

	// calibration management
	CalibrationObserver m_calibrationObserver;

	// display meta information
	const IColorSchema* m_userColorSchemaPtr = nullptr;

	// managemant of bounding box
	mutable bool m_isBoundingBoxValid = false;
	mutable i2d::CRect m_boundingBox;

	// attributes
	bool m_isVisible = true;
	int m_layerType = 0;
	ShapeTransformMode m_shapeTransformMode;

	QString m_defaultDescription;
	QString m_defaultToolTip;
};


} // namespace iview


#endif // !iview_CShapeBase_included


