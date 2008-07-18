#ifndef iqt2d_TSceneExtenderCompBase_included
#define iqt2d_TSceneExtenderCompBase_included


// Qt includes
#include <QGraphicsItem>

#include "istd/TPointerVector.h"

#include "iqt2d/ISceneProvider.h"
#include "iqt2d/ISceneExtender.h"


namespace iqt2d
{


template <class Base>
class TSceneExtenderCompBase: public Base, virtual public ISceneExtender
{
public:
	typedef Base BaseClass;

	I_BEGIN_BASE_COMPONENT(TSceneExtenderCompBase);
		I_REGISTER_INTERFACE(ISceneExtender);
		I_ASSIGN(m_slaveExtenderCompPtr, "SlaveSceneExtender", "Scene extender will be used to provide background shapes", false, "SlaveSceneExtender");
		I_ASSIGN_MULTI_0(m_idFiltersAttrPtr, "SceneIdFilters", "Optional scene ID filters allowing to ignore some scene providers", false);
	I_END_COMPONENT;

	// reimplemented (iqt2d::ISceneExtender)
	virtual void AddItemsToScene(ISceneProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(ISceneProvider* providerPtr);

protected:
	typedef istd::TPointerVector<QGraphicsItem> Shapes;

	bool IsSceneIdSupported(int id) const;

	// abstract methods
	virtual void CreateShapes(int sceneId, bool inactiveOnly, Shapes& result) = 0;

private:
	typedef std::map<ISceneProvider*, Shapes> ShapesMap;

	ShapesMap m_shapesMap;

	I_REF(ISceneExtender, m_slaveExtenderCompPtr);
	I_MULTIATTR(int, m_idFiltersAttrPtr);
};


// public methods

// reimplemented (iqt2d::ISceneExtender)

template <class Base>
void TSceneExtenderCompBase<Base>::AddItemsToScene(ISceneProvider* providerPtr, int flags)
{
	I_ASSERT(providerPtr != NULL);

	int id = providerPtr->GetSceneId();
	QGraphicsScene* scenePtr = providerPtr->GetScene();

	if ((scenePtr != NULL) && IsSceneIdSupported(id) && (m_shapesMap.find(providerPtr) == m_shapesMap.end())){
		Shapes& shapes = m_shapesMap[providerPtr];

		bool isBackground = ((flags & SF_BACKGROUND) != 0);
		CreateShapes(id, isBackground, shapes);

		int shapesCount = shapes.GetCount();
		for (int i = 0; i < shapesCount; ++i){
			QGraphicsItem* shapePtr = shapes.GetAt(i);
			if (shapePtr != NULL){
				scenePtr->addItem(shapePtr);
			}
		}
	}
}


template <class Base>
void TSceneExtenderCompBase<Base>::RemoveItemsFromScene(ISceneProvider* providerPtr)
{
	I_ASSERT(providerPtr != NULL);

	ShapesMap::iterator iter = m_shapesMap.find(providerPtr);
	if (iter != m_shapesMap.end()){
		QGraphicsScene* scenePtr = providerPtr->GetScene();

		if (scenePtr != NULL){
			Shapes& shapes = iter->second;

			int shapesCount = shapes.GetCount();
			for (int i = 0; i < shapesCount; ++i){
				QGraphicsItem* shapePtr = shapes.GetAt(i);
				if (shapePtr != NULL){
					scenePtr->removeItem(shapePtr);
				}
			}
		}

		m_shapesMap.erase(iter);
	}
}


// protected methods

template <class Base>
bool TSceneExtenderCompBase<Base>::IsSceneIdSupported(int id) const
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


} // namespace iqt2d


#endif // !iqt2d_TSceneExtenderCompBase_included


