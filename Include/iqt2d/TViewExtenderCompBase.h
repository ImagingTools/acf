#ifndef iqt2d_TSceneExtenderCompBase_included
#define iqt2d_TSceneExtenderCompBase_included


// Qt includes
#include <QtGui/QGraphicsItem>

// ACF includes
#include "istd/TPointerVector.h"

#include "iview/CShapeBase.h"

#include "iqt2d/IViewProvider.h"
#include "iqt2d/IViewExtender.h"


namespace iqt2d
{


template <class Base>
class TViewExtenderCompBase: public Base, virtual public IViewExtender
{
public:
	typedef Base BaseClass;

	I_BEGIN_BASE_COMPONENT(TViewExtenderCompBase);
		I_REGISTER_INTERFACE(IViewExtender);
		I_ASSIGN(m_slaveExtenderCompPtr, "SlaveSceneExtender", "Scene extender will be used to provide background shapes", false, "SlaveSceneExtender");
		I_ASSIGN_MULTI_0(m_idFiltersAttrPtr, "SceneIdFilters", "Optional scene ID filters allowing to ignore some scene providers", false);
	I_END_COMPONENT;

	// reimplemented (iqt2d::IViewExtender)
	virtual void AddItemsToScene(IViewProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(IViewProvider* providerPtr);

protected:
	typedef istd::TPointerVector<iview::IShape> Shapes;
	typedef QMap<IViewProvider*, Shapes> ShapesMap;

	bool IsSceneIdSupported(int id) const;
	const ShapesMap& GetShapesMap() const;

	void UpdateAllViews();

	// abstract methods
	virtual void CreateShapes(int sceneId, Shapes& result) = 0;

private:
	ShapesMap m_shapesMap;

	I_REF(IViewExtender, m_slaveExtenderCompPtr);
	I_MULTIATTR(int, m_idFiltersAttrPtr);
};


// public methods

// reimplemented (iqt2d::IViewExtender)

template <class Base>
void TViewExtenderCompBase<Base>::AddItemsToScene(IViewProvider* providerPtr, int flags)
{
	I_ASSERT(providerPtr != NULL);

	int id = providerPtr->GetViewId();
	iview::IShapeView* viewPtr = providerPtr->GetView();

	if ((viewPtr != NULL) && IsSceneIdSupported(id) && (m_shapesMap.find(providerPtr) == m_shapesMap.end())){
		Shapes& shapes = m_shapesMap[providerPtr];

		bool isBackground = ((flags & SF_BACKGROUND) != 0);
		CreateShapes(id, shapes);

		int shapesCount = shapes.GetCount();
		for (int i = 0; i < shapesCount; ++i){
			iview::IShape* shapePtr = shapes.GetAt(i);
			if (shapePtr != NULL){
				if (isBackground){
					iview::CShapeBase* shapeImplPtr = dynamic_cast<iview::CShapeBase*>(shapePtr);
					if (shapeImplPtr != NULL){
						shapeImplPtr->AssignToLayer(iview::IViewLayer::LT_INACTIVE);
					}
				}

				viewPtr->ConnectShape(shapePtr);
			}
		}
	}

	if (m_slaveExtenderCompPtr.IsValid()){
		m_slaveExtenderCompPtr->AddItemsToScene(providerPtr, (flags | SF_BACKGROUND) & ~SF_DIRECT);
	}
}


template <class Base>
void TViewExtenderCompBase<Base>::RemoveItemsFromScene(IViewProvider* providerPtr)
{
	I_ASSERT(providerPtr != NULL);

	if (m_slaveExtenderCompPtr.IsValid()){
		m_slaveExtenderCompPtr->RemoveItemsFromScene(providerPtr);
	}

	ShapesMap::iterator iter = m_shapesMap.find(providerPtr);
	if (iter != m_shapesMap.end()){
		iview::IShapeView* viewPtr = providerPtr->GetView();

		if (viewPtr != NULL){
			Shapes& shapes = iter.value();

			int shapesCount = shapes.GetCount();
			for (int i = 0; i < shapesCount; ++i){
				iview::IShape* shapePtr = shapes.GetAt(i);
				if (shapePtr != NULL){
					viewPtr->DisconnectShape(shapePtr);
				}
			}
		}

		m_shapesMap.erase(iter);
	}
}


// protected methods

template <class Base>
bool TViewExtenderCompBase<Base>::IsSceneIdSupported(int id) const
{
	if (m_idFiltersAttrPtr.IsValid()){
		int filtersCount = m_idFiltersAttrPtr.GetCount();
		for (int i = 0; i < filtersCount; ++i){
			int filterId = m_idFiltersAttrPtr[i];
			if (id == filterId){
				return true;
			}
		}

		return false;
	}

	return true;
}


template <class Base>
const typename TViewExtenderCompBase<Base>::ShapesMap& TViewExtenderCompBase<Base>::GetShapesMap() const
{
	return m_shapesMap;
}


template <class Base>
void TViewExtenderCompBase<Base>::UpdateAllViews()
{
	for (ShapesMap::iterator index = m_shapesMap.begin(); index != m_shapesMap.end(); index++){
		IViewProvider* viewProvderPtr = index.key();
		I_ASSERT(viewProvderPtr != NULL);

		iview::IShapeView* viewPtr = viewProvderPtr->GetView();
		I_ASSERT(viewPtr != NULL);

		viewPtr->Update();
	}
}


} // namespace iqt2d


#endif // !iqt2d_TSceneExtenderCompBase_included


