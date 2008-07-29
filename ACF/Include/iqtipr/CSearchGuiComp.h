#ifndef iqtipr_CSearchGuiComp_included
#define iqtipr_CSearchGuiComp_included



#include "imod/IModel.h"

#include "icomp/CComponentBase.h"

#include "iprm/IParamsSet.h"

#include "iipr/ISearchProcessor.h"

#include "icam/IBitmapAcquisition.h"

#include "iqt2d/TSceneExtenderCompBase.h"

#include "iqt/TDesignerGuiCompBase.h"

#include "iqtipr/Generated/ui_CSearchGuiComp.h"


namespace iqtipr
{


class CSearchGuiComp: public iqt2d::TSceneExtenderCompBase<iqt::TDesignerGuiCompBase<Ui::CSearchGuiComp, QWidget> >
{
	Q_OBJECT
public:
	typedef iqt2d::TSceneExtenderCompBase<iqt::TDesignerGuiCompBase<Ui::CSearchGuiComp, QWidget> > BaseClass;

	I_BEGIN_COMPONENT(CSearchGuiComp)
		I_ASSIGN(m_searchResultsModelCompPtr, "SearchResults", "Search results model", true, "SearchResults");
		I_ASSIGN(m_searchResultsCompPtr, "SearchResults", "Search results model", true, "SearchResults");
		I_ASSIGN(m_paramsSetCompPtr, "ParamsSet", "Parameters set", false, "ParamsSet");
		I_ASSIGN(m_paramsSetModelCompPtr, "ParamsSet", "Parameters set", false, "ParamsSet");
		I_ASSIGN(m_paramsSetGuiCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
		I_ASSIGN(m_paramsSetObserverCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
		I_ASSIGN(m_bitmapAcquisitionCompPtr, "BitmapAcquisition", "Gets image", true, "BitmapAcquisition");
		I_ASSIGN(m_searchProcessorCompPtr, "SearchProcessor", "Does model search in an image", true, "SearchProcessor");
		I_ASSIGN(m_bitmapCompPtr, "Bitmap", "Bitmap", true, "Bitmap");
		I_ASSIGN(m_bitmapModelCompPtr, "Bitmap", "Bitmap", true, "Bitmap");
	I_END_COMPONENT

	// reimplemented (iqt2d::TSceneExtenderCompBase)
	virtual void CreateShapes(int sceneId, bool inactiveOnly, Shapes& result);

protected:
	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

protected slots:
	void on_FindButton_clicked();

private:
	void DoSearch();

private:
	I_ATTR(istd::CString, m_unitNameAttrPtr);
	I_REF(iipr::ISearchProcessor, m_searchProcessorCompPtr);
	I_REF(imod::IModel, m_searchResultsModelCompPtr);
	I_REF(iipr::IFeaturesConsumer, m_searchResultsCompPtr);
	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(imod::IModel, m_paramsSetModelCompPtr);
	I_REF(iqt::IGuiObject, m_paramsSetGuiCompPtr);
	I_REF(imod::IObserver, m_paramsSetObserverCompPtr);
	I_REF(icam::IBitmapAcquisition, m_bitmapAcquisitionCompPtr);
	I_REF(iimg::IBitmap, m_bitmapCompPtr);
	I_REF(imod::IModel, m_bitmapModelCompPtr);
};


} // namespace iqtipr


#endif // !iqtipr_CSearchGuiComp_included

