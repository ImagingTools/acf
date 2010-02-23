#ifndef iqt2d_CRectangleParamsGuiComp_included
#define iqt2d_CRectangleParamsGuiComp_included


#include "i2d/CRectangle.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqt2d/TSceneExtenderCompBase.h"
#include "iqt2d/Generated/ui_CRectangleParamsGuiComp.h"


namespace iqt2d
{


class CRectangleParamsGuiComp: public iqt2d::TSceneExtenderCompBase<iqtgui::TDesignerGuiObserverCompBase<
			Ui::CRectangleParamsGuiComp,
			i2d::CRectangle> >
{
	Q_OBJECT

public:
	typedef iqt2d::TSceneExtenderCompBase<iqtgui::TDesignerGuiObserverCompBase<
				Ui::CRectangleParamsGuiComp,
				i2d::CRectangle> > BaseClass;

	I_BEGIN_COMPONENT(CRectangleParamsGuiComp)
		I_ASSIGN(m_rectZValueAttrPtr, "RectZValue", "Describe draw priority on display console (the objects with bigger value will overlap the other ones)", true, 1);
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
	I_ATTR(double, m_rectZValueAttrPtr);
};


} // namespace iqt2d


#endif // !iqt2d_CRectangleParamsGuiComp_included


