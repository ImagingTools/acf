#ifndef iqt2d_CTubePolylineParamsGuiComp_included
#define iqt2d_CTubePolylineParamsGuiComp_included


// ACF includes
#include "i2d/CTubePolyline.h"

#include "iview/CInteractiveTubePolylineShape.h"

#include "iqt2d/TPolygonBasedParamsGuiComp.h"


namespace iqt2d
{


class CTubePolylineParamsGuiComp: public TPolygonBasedParamsGuiComp<iview::CInteractiveTubePolylineShape, i2d::CTubePolyline>
{
	Q_OBJECT

public:
	typedef TPolygonBasedParamsGuiComp<iview::CInteractiveTubePolylineShape, i2d::CTubePolyline> BaseClass;

	I_BEGIN_COMPONENT(CTubePolylineParamsGuiComp);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected Q_SLOTS:
	void OnParamsChanged();

	void on_InsertButton_clicked();
	void on_RemoveButton_clicked();

protected:
	// reimplemented (iqt2d::TPolygonBasedParamsGuiComp)
	virtual void OnInsertNode();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiRetranslate();
};


} // namespace iqt2d


#endif // !iqt2d_CTubePolylineParamsGuiComp_included
