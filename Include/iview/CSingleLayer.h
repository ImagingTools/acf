#ifndef iview_CSingleLayer_included
#define iview_CSingleLayer_included


#include "iview/TLayerBase.h"


namespace iview
{


class CSingleLayer: public TLayerBase<ILayer>
{
public:
	typedef TLayerBase<ILayer> BaseClass;

	CSingleLayer();
	virtual ~CSingleLayer();

	virtual IShape* GetShapePtr() const;

	// reimplemented (iview::ILayer)
	virtual bool IsShapeConnected(IShape* shapePtr);
	virtual bool ConnectShape(IShape* shapePtr);
	virtual int GetShapesCount() const;
	virtual void UpdateAllShapes(int changeFlag);
	virtual void DisconnectAllShapes();
	virtual void DrawShapes(QPainter& drawContext);

	// reimplemented (iview::IDisplay)
	virtual i2d::CRect GetBoundingBox() const;

	// reimplemented (iview::IShapeObserver)
	virtual void OnChangeShape(IShape* shapePtr);
	virtual void DisconnectShape(IShape* shapePtr);

private:
	IShape* m_shapePtr;
	i2d::CRect m_boundingBox;
};


// inline methods

inline IShape* CSingleLayer::GetShapePtr() const
{
	return m_shapePtr;
}


// reimplemented (iview::ILayer)

inline bool CSingleLayer::IsShapeConnected(IShape* shapePtr)
{
	return (m_shapePtr == shapePtr);
}


} // namespace iview


#endif // !iview_CSingleLayer_included



