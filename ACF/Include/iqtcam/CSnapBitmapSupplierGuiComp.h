#ifndef iqtcam_CSnapBitmapSupplierGuiComp_included
#define iqtcam_CSnapBitmapSupplierGuiComp_included


#include "iser/IFileLoader.h"

#include "imod/IObserver.h"

#include "iproc/IIdManager.h"

#include "icam/CSnapBitmapSupplierComp.h"

#include "iqt/IGuiObject.h"
#include "iqt/TDesignerGuiObserverCompBase.h"

#include "iqtproc/TSupplierGuiCompBase.h"

#include "iqtcam/iqtcam.h"

#include "iqtcam/Generated/ui_CSnapBitmapSupplierGuiComp.h"


namespace iqtcam
{


class CSnapBitmapSupplierGuiComp: public iqtproc::TSupplierGuiCompBase<
			Ui::CSnapBitmapSupplierGuiComp,
			icam::CSnapBitmapSupplierComp>
{
	Q_OBJECT

public:
	typedef iqtproc::TSupplierGuiCompBase<
				Ui::CSnapBitmapSupplierGuiComp,
				icam::CSnapBitmapSupplierComp> BaseClass;

	I_BEGIN_COMPONENT(CSnapBitmapSupplierGuiComp)
		I_ASSIGN(m_bitmapLoaderCompPtr, "BitmapLoader", "Saves bitmap to file", false, "BitmapLoader");
	I_END_COMPONENT

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

	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (iqtproc::TSupplierGuiCompBase)
	virtual QWidget* GetParamsWidget() const;

	// reimplemented (iqt2d::TSceneExtenderCompBase)
	virtual void CreateShapes(int sceneId, bool inactiveOnly, Shapes& result);

	// reimplemented (iqt::TGuiObserverWrap)
	virtual void OnGuiModelAttached();

private:
	I_REF(iser::IFileLoader, m_bitmapLoaderCompPtr);
};


} // namespace iqtcam


#endif // !iqtcam_CSnapBitmapSupplierGuiComp_included


