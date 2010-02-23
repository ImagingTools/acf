#ifndef iqt2d_CAnnulusSegmentParamsGuiComp_included
#define iqt2d_CAnnulusSegmentParamsGuiComp_included


#include "i2d/CAnnulusSegment.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqt2d/TSceneExtenderCompBase.h"
#include "iqt2d/Generated/ui_CAnnulusSegmentParamsGuiComp.h"


namespace iqt2d
{


class CAnnulusSegmentParamsGuiComp: public iqt2d::TSceneExtenderCompBase<iqtgui::TDesignerGuiObserverCompBase<
			Ui::CAnnulusSegmentParamsGuiComp,
			i2d::CAnnulusSegment> >
{
	Q_OBJECT

public:
	typedef iqt2d::TSceneExtenderCompBase<iqtgui::TDesignerGuiObserverCompBase<
				Ui::CAnnulusSegmentParamsGuiComp,
				i2d::CAnnulusSegment> > BaseClass;

	I_BEGIN_COMPONENT(CAnnulusSegmentParamsGuiComp)
		I_ASSIGN(m_unitNameAttrPtr, "UnitName", "Name of geometric units e.g. mm", false, "mm");
		I_ASSIGN(m_annulusZValueAttrPtr, "AnnulusZValue", "Describe draw priority on display console (the objects with bigger value will overlap the other ones)", true, 1);
	I_END_COMPONENT

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

	// reimplemented (iqt2d::TSceneExtenderCompBase)
	virtual void CreateShapes(int sceneId, bool inactiveOnly, Shapes& result);

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

protected slots:
	void OnParamsChanged(double value);

private:
	I_ATTR(istd::CString, m_unitNameAttrPtr);
	I_ATTR(double, m_annulusZValueAttrPtr);
};


} // namespace iqt2d


#endif // !iqt2d_CAnnulusSegmentParamsGuiComp_included


