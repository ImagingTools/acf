#ifndef iview_CLayerBase_included
#define iview_CLayerBase_included


#include "iview/ILayer.h"
#include "iview/TLayerBaseEx.h"


namespace iview
{


class CLayerBase: public TLayerBaseEx<ILayer>
{
public:
	typedef TLayerBaseEx<ILayer> BaseClass;

	CLayerBase();
	virtual ~CLayerBase();

	// reimplemented (iview::TLayerBaseEx)
	void CalcBoundingBox(i2d::CRect& result) const;

	// reimplemented (iview::ILayer)
	virtual bool IsShapeConnected(IShape* shapePtr);
	virtual bool ConnectShape(IShape* shapePtr);
	virtual int GetShapesCount() const;
	virtual void UpdateAllShapes(int changeFlag);
	virtual void DisconnectAllShapes();
	virtual void DrawShapes(QPainter& drawContext);
	
	// reimplemented (iview::IShapeObserver)
	virtual void OnChangeShape(IShape* shapePtr);
	virtual void DisconnectShape(IShape* shapePtr);

protected:
	ShapeMap m_shapes;
};


} // namespace iview


#endif // !iview_CLayerBase_included


