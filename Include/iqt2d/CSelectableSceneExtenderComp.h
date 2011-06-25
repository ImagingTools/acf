#ifndef iqt2d_CSelectableSceneExtenderComp_included
#define iqt2d_CSelectableSceneExtenderComp_included


// ACF includes
#include "imod/TSingleModelObserverBase.h"

#include "icomp/CComponentBase.h"

#include "iprm/ISelectionParam.h"

#include "iqt2d/ISceneExtender.h"


namespace iqt2d
{


class CSelectableSceneExtenderComp:
			public icomp::CComponentBase,
			public imod::TSingleModelObserverBase<iprm::ISelectionParam>,
			virtual public iqt2d::ISceneExtender
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::TSingleModelObserverBase<iprm::ISelectionParam> BaseClass2;

	CSelectableSceneExtenderComp();

	I_BEGIN_COMPONENT(CSelectableSceneExtenderComp);
		I_REGISTER_INTERFACE(iqt2d::ISceneExtender);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_ASSIGN_MULTI_0(m_extendersCompPtr, "SceneExtenders", "List of scene extenders", true);
	I_END_COMPONENT;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

	// reimplemented (iqt2d::ISceneExtender)
	virtual void AddItemsToScene(iqt2d::ISceneProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(iqt2d::ISceneProvider* providerPtr);

protected:
	void AttachCurrent();
	void DetachCurrent();

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	I_MULTIREF(iqt2d::ISceneExtender, m_extendersCompPtr);

	typedef std::map<iqt2d::ISceneProvider*, int> ConnectedSceneFlags;	// maps connected scene provider to connection flags
	ConnectedSceneFlags m_connectedSceneFlags;

	int m_currentSelectedIndex;
};


} // namespace iqt2d


#endif // !iqt2d_CSelectableSceneExtenderComp_included


