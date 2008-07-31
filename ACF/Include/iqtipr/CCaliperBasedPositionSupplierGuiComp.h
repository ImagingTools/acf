#ifndef iqtipr_CCaliperBasedPositionSupplierGuiComp_included
#define iqtipr_CCaliperBasedPositionSupplierGuiComp_included


#include "iser/IFileLoader.h"

#include "imod/IObserver.h"
#include "imod/TModelWrap.h"

#include "i2d/CPosition2d.h"

#include "iproc/IIdManager.h"

#include "iipr/CCaliperBasedPositionSupplierComp.h"

#include "iqt/IGuiObject.h"
#include "iqt/TDesignerGuiObserverCompBase.h"

#include "iqt2d/TSceneExtenderCompBase.h"

#include "iqtipr/iqtipr.h"

#include "iqtipr/Generated/ui_CCaliperBasedPositionSupplierGuiComp.h"


namespace iqtipr
{


class CCaliperBasedPositionSupplierGuiComp: public iqt2d::TSceneExtenderCompBase<iqt::TDesignerGuiObserverCompBase<
			Ui::CCaliperBasedPositionSupplierGuiComp,
			iipr::CCaliperBasedPositionSupplierComp> >
{
	Q_OBJECT

public:
	typedef iqt2d::TSceneExtenderCompBase<iqt::TDesignerGuiObserverCompBase<
				Ui::CCaliperBasedPositionSupplierGuiComp,
				iipr::CCaliperBasedPositionSupplierComp> > BaseClass;

	I_BEGIN_COMPONENT(CCaliperBasedPositionSupplierGuiComp)
		I_ASSIGN(m_paramsLoaderCompPtr, "ParamsLoader", "Loads and saves parameters from and to file", false, "ParamsLoader");
		I_ASSIGN(m_paramsSetGuiCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
		I_ASSIGN(m_paramsSetObserverCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
		I_ASSIGN(m_paramsSetExtenderCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
		I_ASSIGN(m_idManagerCompPtr, "IdManager", "ID manager used to generate next processing ID", false, "IdManager");
	I_END_COMPONENT

	// reimplemented (iqt2d::ISceneExtender)
	virtual void AddItemsToScene(iqt2d::ISceneProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(iqt2d::ISceneProvider* providerPtr);

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor();

protected slots:
	void on_TestButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();

protected:
	bool DoTest();

	// reimplemented (iqt2d::TSceneExtenderCompBase)
	virtual void CreateShapes(int sceneId, bool inactiveOnly, Shapes& result);

	// reimplemented (iqt::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

private:
	I_REF(iser::IFileLoader, m_paramsLoaderCompPtr);

	I_REF(iqt::IGuiObject, m_paramsSetGuiCompPtr);
	I_REF(imod::IObserver, m_paramsSetObserverCompPtr);
	I_REF(iqt2d::ISceneExtender, m_paramsSetExtenderCompPtr);
	I_REF(iproc::IIdManager, m_idManagerCompPtr);

	imod::TModelWrap<i2d::CPosition2d> m_foundPosition;
};


} // namespace iqtipr


#endif // !iqtipr_CCaliperBasedPositionSupplierGuiComp_included


