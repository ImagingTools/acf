#ifndef iqt2d_CPolylineParamsGuiComp_included
#define iqt2d_CPolylineParamsGuiComp_included


// ACF includes
#include "i2d/CPolyline.h"
#include "iview/CPolylineShape.h"
#include "iqt2d/TPolygonBasedParamsGuiComp.h"


namespace iqt2d
{


class CPolylineParamsGuiComp: public TPolygonBasedParamsGuiComp<iview::CPolylineShape, i2d::CPolyline>
{
	Q_OBJECT

public:
	typedef TPolygonBasedParamsGuiComp<iview::CPolylineShape, i2d::CPolyline> BaseClass;

	I_BEGIN_COMPONENT(CPolylineParamsGuiComp);
		I_ASSIGN(m_showOrientationAttrPtr, "ShowOrientation", "Show orientation on display console", true, false);
	I_END_COMPONENT;

	CPolylineParamsGuiComp();

	// reimplemented (iview::IShapeFactory)
	virtual iview::IShape* CreateShape(const istd::IChangeable* objectPtr, bool connectToModel = false) const;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemented (TPolygonBasedParamsGuiComp)
	virtual bool PopulateActions(CActionAdapter& host, imod::IModel* modelPtr);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiRetranslate();

protected Q_SLOTS:
	void OnParamsChanged();

	void on_InsertButton_clicked();
	void on_RemoveButton_clicked();
	void on_CopyButton_clicked();
	void on_PasteButton_clicked();

	// reimplemented (TPolygonBasedParamsGuiComp)
	void OnActionTriggered(QAction* actionPtr);

private:
	I_ATTR(bool, m_showOrientationAttrPtr);

	QAction m_openCloseAction;
};


} // namespace iqt2d


#endif // !iqt2d_CPolylineParamsGuiComp_included
