#ifndef iqt2d_TSceneExtenderCompBase_included
#define iqt2d_TSceneExtenderCompBase_included


// ACF includes
#include "istd/TPointerVector.h"
#include "i2d/ICalibration2d.h"
#include "iview/IShapeView.h"
#include "iview/CShapeBase.h"
#include "iview/CCalibratedViewBase.h"
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
		I_ASSIGN(m_sceneExtenderModeAttrPtr, "ViewExtenderMode", "Control how the view extender works:\n\t0 - combine slave with current shapes\n\t1 - use slave by direct calls only\n\t2 - use slave always and current shapes indirect only\n\t3 - use slave by direct calls only and current shapes indirect only", true, 0);
		I_ASSIGN_MULTI_0(m_idFiltersAttrPtr, "SceneIdFilters", "Optional scene ID filters allowing to ignore some scene providers", false);
	I_END_COMPONENT;

	TViewExtenderCompBase();

	// reimplemented (iqt2d::IViewExtender)
	virtual void AddItemsToScene(IViewProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(IViewProvider* providerPtr);

protected:
	enum ExtenderMode
	{
		EM_COMBINE,
		EM_SLAVE_DIRECT_ONLY,
		EM_OWN_SHAPES_INDIRECT,
		EM_SLAVE_DIRECT_ONLY_OWN_SHAPES_INDIRECT
	};

	typedef istd::TPointerVector<iview::IShape> Shapes;
	typedef QMap<IViewProvider*, Shapes> ShapesMap;

	bool IsViewIdSupported(int viewId) const;
	const ShapesMap& GetShapesMap() const;

	/**
		It calls \c Update for all used views.
	*/
	void UpdateAllViews();

	// abstract methods
	/**
		Create shapes for the view.
		\param 
	*/
	virtual void CreateShapes(int viewId, Shapes& result) = 0;

private:
	ShapesMap m_shapesMap;
	bool m_isSlaveShown;

	I_REF(IViewExtender, m_slaveExtenderCompPtr);
	I_ATTR(int, m_sceneExtenderModeAttrPtr);
	I_MULTIATTR(int, m_idFiltersAttrPtr);
};


// public methods

template <class Base>
TViewExtenderCompBase<Base>::TViewExtenderCompBase()
:	m_isSlaveShown(false)
{
}


// reimplemented (iqt2d::IViewExtender)

template <class Base>
void TViewExtenderCompBase<Base>::AddItemsToScene(IViewProvider* providerPtr, int flags)
{
	Q_ASSERT(providerPtr != NULL);

	int viewId = providerPtr->GetViewId();
	iview::IShapeView* viewPtr = providerPtr->GetView();

	bool showOwnShapes = true;
	bool showSlaveShapes = true;

	switch (*m_sceneExtenderModeAttrPtr){
	case EM_SLAVE_DIRECT_ONLY:
		showSlaveShapes = ((flags & SF_DIRECT) != 0);
		break;

	case EM_OWN_SHAPES_INDIRECT:
		showOwnShapes = ((flags & SF_DIRECT) == 0);
		break;

	case EM_SLAVE_DIRECT_ONLY_OWN_SHAPES_INDIRECT:
		showOwnShapes = ((flags & SF_DIRECT) == 0);
		showSlaveShapes = ((flags & SF_DIRECT) != 0);
		break;

	default:
		break;
	};


	if (		showOwnShapes &&
				(viewPtr != NULL) &&
				IsViewIdSupported(viewId) &&
				(m_shapesMap.find(providerPtr) == m_shapesMap.end())){
		Shapes& shapes = m_shapesMap[providerPtr];

		bool isBackground = ((flags & SF_BACKGROUND) != 0);
		CreateShapes(viewId, shapes);

		int shapesCount = shapes.GetCount();
		for (int i = 0; i < shapesCount; ++i){
			iview::IShape* shapePtr = shapes.GetAt(i);
			if (shapePtr != NULL){
				// If the shape should be placed in the scene background
				// and is not already a background shape, move it to inactive layer:
				if (isBackground && (shapePtr->GetLayerType() != iview::IViewLayer::LT_BACKGROUND)){
					iview::CShapeBase* shapeImplPtr = dynamic_cast<iview::CShapeBase*>(shapePtr);
					if (shapeImplPtr != NULL){
						shapeImplPtr->AssignToLayer(iview::IViewLayer::LT_INACTIVE);
					}
				}

				viewPtr->ConnectShape(shapePtr);
			}
		}
	}

	if (showSlaveShapes && m_slaveExtenderCompPtr.IsValid()){
		m_slaveExtenderCompPtr->AddItemsToScene(providerPtr, (flags | SF_BACKGROUND) & ~SF_DIRECT);

		m_isSlaveShown = true;
	}
	else{
		m_isSlaveShown = false;
	}
}


template <class Base>
void TViewExtenderCompBase<Base>::RemoveItemsFromScene(IViewProvider* providerPtr)
{
	Q_ASSERT(providerPtr != NULL);

	if (m_isSlaveShown){
		Q_ASSERT(m_slaveExtenderCompPtr.IsValid());

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
bool TViewExtenderCompBase<Base>::IsViewIdSupported(int viewId) const
{
	if (m_idFiltersAttrPtr.IsValid()){
		int filtersCount = m_idFiltersAttrPtr.GetCount();
		for (int i = 0; i < filtersCount; ++i){
			int filterId = m_idFiltersAttrPtr[i];
			if (viewId == filterId){
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
		Q_ASSERT(viewProvderPtr != NULL);

		iview::IShapeView* viewPtr = viewProvderPtr->GetView();
		Q_ASSERT(viewPtr != NULL);

		viewPtr->Update();
	}
}


} // namespace iqt2d


#endif // !iqt2d_TSceneExtenderCompBase_included


