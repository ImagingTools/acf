#ifndef iqt2d_CPolygonParamsGuiComp_included
#define iqt2d_CPolygonParamsGuiComp_included


// ACF includes
#include <iview/CPolygonShape.h>

#include <iqt2d/TPolygonBasedParamsGuiComp.h>


namespace iqt2d
{


class CPolygonParamsGuiComp: public TPolygonBasedParamsGuiComp<iview::CPolygonShape, i2d::CPolygon>
{
	Q_OBJECT

public:
	typedef TPolygonBasedParamsGuiComp<iview::CPolygonShape, i2d::CPolygon> BaseClass;

	I_BEGIN_COMPONENT(CPolygonParamsGuiComp);
	I_END_COMPONENT;

	CPolygonParamsGuiComp();

protected Q_SLOTS:
	void OnParamsChanged();
	void OnActionTriggered(QAction* actionPtr);

	void on_InsertButton_clicked();
	void on_RemoveButton_clicked();
	void on_CopyButton_clicked();
	void on_PasteButton_clicked();
};


} // namespace iqt2d


#endif // !iqt2d_CPolygonParamsGuiComp_included
