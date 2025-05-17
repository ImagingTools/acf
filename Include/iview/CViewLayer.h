#ifndef iview_CLayerBase_included
#define iview_CLayerBase_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include <iview/IViewLayer.h>
#include <iview/IShape.h>


namespace iview
{


/**
	Standard implementation of view layer.
	It contains non interactive shapes only.
*/
class CViewLayer: virtual public IViewLayer
{
public:
	CViewLayer();
	virtual ~CViewLayer();

	// reimplemented (iview::IViewLayer)
	virtual void OnConnectView(IShapeView* viewPtr) override;
	virtual void OnDisconnectView(IShapeView* viewPtr) override;
	virtual IShapeView* GetViewPtr() const override;
	virtual bool IsShapeConnected(IShape* shapePtr) override;
	virtual bool ConnectShape(IShape* shapePtr) override;
	virtual int GetShapesCount() const override;
	virtual Shapes GetShapes() const override;
	virtual void UpdateAllShapes(const istd::IChangeable::ChangeSet& changeSet) override;
	virtual void DisconnectAllShapes() override;
	virtual void DrawShapes(QPainter& drawContext) override;
	virtual bool IsVisible() const override;
	virtual void SetVisible(bool state = true) override;
	
	// reimplemented (iview::IDisplay)
	virtual IDisplay* GetParentDisplayPtr() const override;
	virtual const CScreenTransform& GetTransform() const override;
	virtual i2d::CRect GetBoundingBox() const override;
	virtual i2d::CRect GetClientRect() const override;
	virtual const IColorSchema& GetColorSchema() const override;
	virtual void OnAreaInvalidated(const i2d::CRect& prevArea, const i2d::CRect& newArea) override;

	// reimplemented (iview::IShapeObserver)
	virtual void OnChangeShape(IShape* shapePtr) override;
	virtual bool DisconnectShape(IShape* shapePtr) override;

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const override;
	virtual QString GetShapeDescriptionAt(istd::CIndex2d position) const override;
	virtual QString GetToolTipAt(istd::CIndex2d position) const override;

protected:
	struct ShapeWithBoundingBox
	{
		ShapeWithBoundingBox(){}
		ShapeWithBoundingBox(IShape* shapePtr, const i2d::CRect& box) : shapePtr(shapePtr), box(box){}

		IShape* shapePtr;
		i2d::CRect box;
	};
	typedef QVector<ShapeWithBoundingBox> ShapeList;

	/**
		\internal
		Internal invalidate shape method.
	*/
	bool OnChangeShapeElement(ShapeList::Iterator elementIter);
	
	/**
		\internal
		Internal disconnect shape method.
	*/
	void DisconnectShapeElement(ShapeList& map, ShapeList::iterator iter);

	void InvalidateBoundingBox();
	i2d::CRect& GetBoundingBoxRef() const;
	void SetBoundingBoxValid() const;

	/**
		Recalculate all shapes after view changes.
	*/
	virtual i2d::CRect RecalcAllShapes(const istd::IChangeable::ChangeSet& changeSet);
	/**
		Calculate bounding box for all shapes.
	*/
	virtual i2d::CRect CalcBoundingBox() const;

	ShapeList m_shapes;

private:
	IShapeView* m_viewPtr;
	bool m_isVisible;

	mutable i2d::CRect m_boundingBox;
	mutable bool m_isBoundingBoxValid;
};


// inline methods

inline bool CViewLayer::IsVisible() const
{
	return m_isVisible;
}


} // namespace iview


#endif // !iview_CLayerBase_included


