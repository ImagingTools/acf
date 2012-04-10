#ifndef iview_TLayerBaseEx_included
#define iview_TLayerBaseEx_included


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include "iview/IShape.h"
#include "iview/TLayerBase.h"


namespace iview
{


/**
	This is internal class to generate common implementation
	for diffrent layers implementations.
	Template paramener must be derivate from iview::ILayer interface.
*/
template <class Layer, class View = IShapeView>
class TLayerBaseEx: public TLayerBase<Layer, View>
{
public:
	typedef TLayerBase<Layer, View> BaseClass;

	TLayerBaseEx();

	// reimplemented (iview::IShapeView)
	virtual void SetVisible(bool state);

	// reimplemented (iview::IDisplay)
	virtual i2d::CRect GetBoundingBox() const;
	virtual void OnAreaInvalidated(const i2d::CRect& prevArea, const i2d::CRect& newArea);

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

	// abstract methods
	virtual void CalcBoundingBox(i2d::CRect& result) const = 0;

private:
	mutable i2d::CRect m_boundingBox;
	mutable bool m_isBoundingBoxValid;
};


// public methods

template <class Layer, class View>
TLayerBaseEx<Layer, View>::TLayerBaseEx()
{
	m_boundingBox.Reset();
	m_isBoundingBoxValid = true;
}


// reimplemented (iview::IDisplay)

template <class Layer, class View>
inline void TLayerBaseEx<Layer, View>::SetVisible(bool state)
{
	m_isBoundingBoxValid = false;
	BaseClass::SetVisible(state);
}


// reimplemented (iview::IDisplay)

template <class Layer, class View>
i2d::CRect TLayerBaseEx<Layer, View>::GetBoundingBox() const
{
	if (!m_isBoundingBoxValid){
		CalcBoundingBox(m_boundingBox);
	}

	m_isBoundingBoxValid = true;
	
	return m_boundingBox;
}


template <class Layer, class View>
void TLayerBaseEx<Layer, View>::OnAreaInvalidated(const i2d::CRect& prevArea, const i2d::CRect& newArea)
{
	if (m_isBoundingBoxValid){
		I_ASSERT(!m_isBoundingBoxValid || GetBoundingBoxRef().IsInside(prevArea));

		if (prevArea.IsEmpty() || (
						(m_boundingBox.GetLeft() < prevArea.GetLeft()) &&
						(m_boundingBox.GetRight() > prevArea.GetRight()) &&
						(m_boundingBox.GetTop() < prevArea.GetTop()) &&
						(m_boundingBox.GetBottom() > prevArea.GetBottom()))){
			m_boundingBox.Union(newArea);
		}
		else{
			m_isBoundingBoxValid = false;
		}
		I_ASSERT(!m_isBoundingBoxValid || GetBoundingBoxRef().IsInside(newArea));
	}

	BaseClass::OnAreaInvalidated(prevArea, newArea);
}


// protected methods

template <class Layer, class View>
bool TLayerBaseEx<Layer, View>::OnChangeShapeElement(ShapeMap::Iterator elementIter)
{
	I_ASSERT(elementIter.key() != NULL);
	I_ASSERT(GetBoundingBox().IsInside(elementIter.value()));

	const i2d::CRect oldBoundingBox = elementIter.value();
	const iview::IShape* shapePtr = elementIter.key();
	I_ASSERT(shapePtr != NULL);

	elementIter.value() = shapePtr->GetBoundingBox();

	OnAreaInvalidated(oldBoundingBox, elementIter.value());

	return true;
}


template <class Layer, class View>
void TLayerBaseEx<Layer, View>::DisconnectShapeElement(ShapeMap& map, ShapeMap::iterator iter)
{
	iview::IShape* shapePtr = iter.key();
	const i2d::CRect boundingBox = iter.value();
	map.erase(iter);

	OnAreaInvalidated(i2d::CRect::GetEmpty(), boundingBox);

	shapePtr->OnDisconnectDisplay(this);
}


template <class Layer, class View>
void TLayerBaseEx<Layer, View>::InvalidateBoundingBox()
{
	m_isBoundingBoxValid = false;
}


template <class Layer, class View>
i2d::CRect& TLayerBaseEx<Layer, View>::GetBoundingBoxRef() const
{
	return m_boundingBox;
}


template <class Layer, class View>
void TLayerBaseEx<Layer, View>::SetBoundingBoxValid() const
{
	m_isBoundingBoxValid = true;
}


} // namespace iview


#endif // !iview_TLayerBaseEx_included



