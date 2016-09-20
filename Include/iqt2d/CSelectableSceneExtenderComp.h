#ifndef iqt2d_CSelectableSceneExtenderComp_included
#define iqt2d_CSelectableSceneExtenderComp_included


// ACF includes
#include <imod/TSingleModelObserverBase.h>
#include <icomp/CComponentBase.h>
#include <iprm/ISelectionParam.h>
#include <iqt2d/IViewExtender.h>


namespace iqt2d
{


class CSelectableSceneExtenderComp:
			public icomp::CComponentBase,
			public imod::TSingleModelObserverBase<iprm::ISelectionParam>,
			virtual public iqt2d::IViewExtender
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::TSingleModelObserverBase<iprm::ISelectionParam> BaseClass2;

	CSelectableSceneExtenderComp();

	I_BEGIN_COMPONENT(CSelectableSceneExtenderComp);
		I_REGISTER_INTERFACE(iqt2d::IViewExtender);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_ASSIGN_MULTI_0(m_extendersCompPtr, "SceneExtenders", "List of scene extenders", true);
	I_END_COMPONENT;

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask);
	virtual bool OnModelDetached(imod::IModel* modelPtr);

	// reimplemented (iqt2d::IViewExtender)
	virtual void AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr);

protected:
	void AttachCurrent();
	void DetachCurrent();

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet);

private:
	I_MULTIREF(iqt2d::IViewExtender, m_extendersCompPtr);

	typedef QMap<iqt2d::IViewProvider*, int> ConnectedSceneFlags;	// maps connected scene provider to connection flags
	ConnectedSceneFlags m_connectedSceneFlags;

	int m_currentSelectedIndex;
};


} // namespace iqt2d


#endif // !iqt2d_CSelectableSceneExtenderComp_included


