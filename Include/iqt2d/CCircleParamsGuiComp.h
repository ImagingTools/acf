#ifndef iqt2d_CCircleParamsGuiComp_included
#define iqt2d_CCircleParamsGuiComp_included


#include "i2d/CCircle.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqt2d/TSceneExtenderCompBase.h"

#include "iqt2d/Generated/ui_CCircleParamsGuiComp.h"


namespace iqt2d
{


class CCircleParamsGuiComp: public iqt2d::TSceneExtenderCompBase<iqtgui::TDesignerGuiObserverCompBase<
			Ui::CCircleParamsGuiComp,
			i2d::CCircle> >
{
	Q_OBJECT

public:
	typedef iqt2d::TSceneExtenderCompBase<iqtgui::TDesignerGuiObserverCompBase<
				Ui::CCircleParamsGuiComp,
				i2d::CCircle> > BaseClass;

	I_BEGIN_COMPONENT(CCircleParamsGuiComp)
		I_ASSIGN(m_unitNameAttrPtr, "UnitName", "Name of geometric units e.g. mm", false, "mm");
		I_ASSIGN(m_circleZValueAttrPtr, "CircleZValue", "Describe draw priority on display console (the objects with bigger value will overlap the other ones)", true, 1);
	I_END_COMPONENT

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

	// reimplemented (iqt2d::TSceneExtenderCompBase)
	virtual void CreateShapes(int sceneId, bool inactiveOnly, Shapes& result);

protected slots:
	void OnParamsChanged(double value);

private:
	I_ATTR(istd::CString, m_unitNameAttrPtr);
	I_ATTR(double, m_circleZValueAttrPtr);
};


} // namespace iqt2d


#endif // !iqt2d_CCircleParamsGuiComp_included


