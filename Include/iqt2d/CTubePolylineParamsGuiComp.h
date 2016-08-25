#ifndef iqt2d_CTubePolylineParamsGuiComp_included
#define iqt2d_CTubePolylineParamsGuiComp_included


// ACF includes
#include "i2d/CTubePolyline.h"

#include "iview/CTubePolylineShape.h"

#include "iqt2d/TPolygonBasedParamsGuiComp.h"


namespace iqt2d
{


class CTubePolylineParamsGuiComp: public TPolygonBasedParamsGuiComp<iview::CTubePolylineShape, i2d::CTubePolyline>
{
	Q_OBJECT

public:
	typedef TPolygonBasedParamsGuiComp<iview::CTubePolylineShape, i2d::CTubePolyline> BaseClass;

	I_BEGIN_COMPONENT(CTubePolylineParamsGuiComp);
	I_END_COMPONENT;

protected Q_SLOTS:

	void OnActionTriggered(QAction* actionPtr);

	void on_InsertButton_clicked();
	void on_RemoveButton_clicked();
	void on_CopyButton_clicked();
	void on_PasteButton_clicked();

protected:

	// reimplemented (iqt2d::TPolygonBasedParamsGuiComp<iview::CTubePolylineShape, i2d::CTubePolyline>)
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
};


} // namespace iqt2d


#endif // !iqt2d_CTubePolylineParamsGuiComp_included
