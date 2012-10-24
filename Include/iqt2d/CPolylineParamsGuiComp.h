#ifndef iqt2d_CPolylineParamsGuiComp_included
#define iqt2d_CPolylineParamsGuiComp_included


// ACF includes
#include "i2d/CPolyline.h"

#include "iview/CInteractivePolylineShape.h"

#include "iqt2d/TPolygonBasedParamsGuiComp.h"


namespace iqt2d
{


class CPolylineParamsGuiComp: public TPolygonBasedParamsGuiComp<iview::CInteractivePolylineShape, i2d::CPolyline>
{
	Q_OBJECT

public:
	typedef TPolygonBasedParamsGuiComp<iview::CInteractivePolylineShape, i2d::CPolyline> BaseClass;

	I_BEGIN_COMPONENT(CPolylineParamsGuiComp);
		I_ASSIGN(m_showOrientationAttrPtr, "ShowOrientation", "Show orientation on display console", true, false);
	I_END_COMPONENT;

protected:
	// reimplemented (iqt2d::TShapeParamsGuiCompBase)
	virtual iview::CInteractivePolylineShape* CreateShape() const;

protected Q_SLOTS:
	void OnParamsChanged();

	void on_InsertButton_clicked();
	void on_RemoveButton_clicked();
	void on_CopyButton_clicked();
	void on_PasteButton_clicked();
	void on_NodeParamsTable_itemSelectionChanged();
	void on_CloseLineCheckBox_stateChanged(int state);

	// reimplemented from TPolygonBasedParamsGuiComp<iview::CInteractivePolylineShape, i2d::CPolyline>
	void OnGuiModelAttached();
	void OnGuiModelDetached();
	void OnGuiCreated();

private:
	/**
		Change state of a line close check box: disabled and hidden, according 
		to the attached model type (only visible for CPolyline) and list selection 
		(uncheckable if a list element is selected, always checkable).
	*/
	void UpdateClosedLineCheckBox(bool forceEnabled, bool forceHidden);

	I_ATTR(bool, m_showOrientationAttrPtr);
};


} // namespace iqt2d


#endif // !iqt2d_CPolylineParamsGuiComp_included
