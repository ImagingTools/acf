#ifndef iqt_CSnapImageGuiComp_included
#define iqt_CSnapImageGuiComp_included


// Qt includes
#include <QTimer>

#include "iser/IFileLoader.h"

#include "imod/IModel.h"
#include "imod/IObserver.h"

#include "iimg/IBitmap.h"

#include "iprm/IParamsSet.h"

#include "icam/IBitmapAcquisition.h"

#include "iqt/IGuiObject.h"
#include "iqt/TDesignerGuiCompBase.h"

#include "iqt/Generated/ui_CSnapImageGuiComp.h"


namespace iqt
{


class CSnapImageGuiComp: public TDesignerGuiCompBase<Ui::CSnapImageGuiComp, QWidget>
{
	Q_OBJECT

public:
	typedef TDesignerGuiCompBase<Ui::CSnapImageGuiComp, QWidget> BaseClass;

	I_BEGIN_COMPONENT(CSnapImageGuiComp)
		I_ASSIGN(m_bitmapCompPtr, "Bitmap", "Bitmap will be shown", true, "Bitmap");
		I_ASSIGN(m_bitmapModelCompPtr, "Bitmap", "Bitmap will be shown", true, "Bitmap");
		I_ASSIGN(m_bitmapGuiCompPtr, "BitmapGui", "Show test bitmap as a part of view window", true, "BitmapView");
		I_ASSIGN(m_bitmapObserverCompPtr, "BitmapGui", "Show test bitmap as a part of view window", true, "BitmapView");
		I_ASSIGN(m_bitmapAcquisitionCompPtr, "BitmapAcquisition", "Bitmap acquision obje for image snap", false, "BitmapAcquisition");
		I_ASSIGN(m_bitmapLoaderCompPtr, "BitmapLoader", "Saves bitmap to file", false, "BitmapLoader");
		I_ASSIGN(m_paramsLoaderCompPtr, "ParamsLoader", "Loads and saves parameters from and to file", false, "ParamsLoader");
		I_ASSIGN(m_paramsSetCompPtr, "ParamsSet", "Parameters set", false, "ParamsSet");
		I_ASSIGN(m_paramsSetModelCompPtr, "ParamsSet", "Parameters set", false, "ParamsSet");
		I_ASSIGN(m_paramsSetGuiCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
		I_ASSIGN(m_paramsSetObserverCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
	I_END_COMPONENT

	CSnapImageGuiComp();

protected slots:
	void on_SnapImageButton_clicked();
	void on_LiveImageButton_toggled(bool checked);
	void on_SaveImageButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();
	void OnTimerReady();

protected:
	bool SnapImage();

	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_REF(iimg::IBitmap, m_bitmapCompPtr);
	I_REF(imod::IModel, m_bitmapModelCompPtr);
	I_REF(iqt::IGuiObject, m_bitmapGuiCompPtr);
	I_REF(imod::IObserver, m_bitmapObserverCompPtr);
	I_REF(icam::IBitmapAcquisition, m_bitmapAcquisitionCompPtr);

	I_REF(iser::IFileLoader, m_bitmapLoaderCompPtr);
	I_REF(iser::IFileLoader, m_paramsLoaderCompPtr);

	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(imod::IModel, m_paramsSetModelCompPtr);
	I_REF(iqt::IGuiObject, m_paramsSetGuiCompPtr);
	I_REF(imod::IObserver, m_paramsSetObserverCompPtr);

	QTimer m_timer;
};


} // namespace iqt


#endif // !iqt_CSnapImageGuiComp_included


