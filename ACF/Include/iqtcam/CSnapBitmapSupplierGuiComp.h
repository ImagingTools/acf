#ifndef iqtcam_CSnapBitmapSupplierGuiComp_included
#define iqtcam_CSnapBitmapSupplierGuiComp_included


// Qt includes
#include <QTimer>

#include "iser/IFileLoader.h"

#include "imod/IObserver.h"

#include "iproc/IIdManager.h"

#include "icam/CSnapBitmapSupplierComp.h"

#include "iqt/IGuiObject.h"
#include "iqt/TDesignerGuiObserverCompBase.h"

#include "iqt2d/TSceneExtenderCompBase.h"

#include "iqtcam/iqtcam.h"

#include "iqtcam/Generated/ui_CSnapBitmapSupplierGuiComp.h"


namespace iqtcam
{


class CSnapBitmapSupplierGuiComp: public iqt2d::TSceneExtenderCompBase<iqt::TDesignerGuiObserverCompBase<
			Ui::CSnapBitmapSupplierGuiComp,
			icam::CSnapBitmapSupplierComp> >
{
	Q_OBJECT

public:
	typedef iqt2d::TSceneExtenderCompBase<iqt::TDesignerGuiObserverCompBase<
				Ui::CSnapBitmapSupplierGuiComp,
				icam::CSnapBitmapSupplierComp> > BaseClass;

	I_BEGIN_COMPONENT(CSnapBitmapSupplierGuiComp)
		I_ASSIGN(m_bitmapLoaderCompPtr, "BitmapLoader", "Saves bitmap to file", false, "BitmapLoader");
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
	void on_SnapImageButton_clicked();
	void on_SaveImageButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();

protected:
	const iimg::IBitmap* GetCurrentBitmap() const;
	bool SnapImage();

	// reimplemented (iqt2d::TSceneExtenderCompBase)
	virtual void CreateShapes(int sceneId, bool inactiveOnly, Shapes& result);

	// reimplemented (iqt::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

private:
	I_REF(iser::IFileLoader, m_bitmapLoaderCompPtr);
	I_REF(iser::IFileLoader, m_paramsLoaderCompPtr);

	I_REF(iqt::IGuiObject, m_paramsSetGuiCompPtr);
	I_REF(imod::IObserver, m_paramsSetObserverCompPtr);
	I_REF(iqt2d::ISceneExtender, m_paramsSetExtenderCompPtr);
	I_REF(iproc::IIdManager, m_idManagerCompPtr);
};


} // namespace iqtcam


#endif // !iqtcam_CSnapBitmapSupplierGuiComp_included


