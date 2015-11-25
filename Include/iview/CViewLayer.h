#ifndef iview_CLayerBase_included
#define iview_CLayerBase_included


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include "iview/IViewLayer.h"
#include "iview/IShape.h"


namespace iview
{


/**
	Standard implementation of view layer.
	It contains non interactive shapes only.
*/
class CViewLayer: virtual public IViewLayer
{
public:
	typedef QList<IShape*> Shapes;

	CViewLayer();
	virtual ~CViewLayer();

	Shapes GetAllShapes() const;

	// reimplemented (iview::IViewLayer)
	virtual void OnConnectView(IShapeView* viewPtr);
	virtual void OnDisconnectView(IShapeView* viewPtr);
	virtual IShapeView* GetViewPtr() const;
	virtual bool IsShapeConnected(IShape* shapePtr);
	virtual bool ConnectShape(IShape* shapePtr);
	virtual int GetShapesCount() const;
	virtual void UpdateAllShapes(const istd::IChangeable::ChangeSet& changeSet);
	virtual void DisconnectAllShapes();
	virtual void DrawShapes(QPainter& drawContext);
	virtual bool IsVisible() const;
	virtual void SetVisible(bool state = true);
	
	// reimplemented (iview::IDisplay)
	virtual IDisplay* GetParentDisplayPtr() const;
	virtual const CScreenTransform& GetTransform() const;
	virtual i2d::CRect GetBoundingBox() const;
	virtual i2d::CRect GetClientRect() const;
	virtual const IColorSchema& GetColorSchema() const;
	virtual void OnAreaInvalidated(const i2d::CRect& prevArea, const i2d::CRect& newArea);

	// reimplemented (iview::IShapeObserver)
	virtual void OnChangeShape(IShape* shapePtr);
	virtual bool DisconnectShape(IShape* shapePtr);

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const;
	virtual QString GetShapeDescriptionAt(istd::CIndex2d position) const;

protected:
	typedef QMap<IShape*, i2d::CRect> ShapeMap;

	/**
		\internal
		Internal invalidate shape method.
	*/
	bool OnChangeShapeElement(ShapeMap::Iterator elementIter);
	
	/**
		\internal
		Internal disconnect shape method.
	*/
	void DisconnectShapeElement(ShapeMap& map, ShapeMap::iterator iter);

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

	ShapeMap m_shapes;

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


