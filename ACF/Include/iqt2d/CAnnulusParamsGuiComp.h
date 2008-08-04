#ifndef iqt2d_CAnnulusParamsGuiComp_included
#define iqt2d_CAnnulusParamsGuiComp_included


#include "i2d/CAnnulus.h"

#include "iqt/TDesignerGuiObserverCompBase.h"

#include "iqt2d/TSceneExtenderCompBase.h"

#include "iqt2d/Generated/ui_CAnnulusParamsGuiComp.h"


namespace iqt2d
{


class CAnnulusParamsGuiComp: public iqt2d::TSceneExtenderCompBase<iqt::TDesignerGuiObserverCompBase<
			Ui::CAnnulusParamsGuiComp,
			i2d::CAnnulus> >
{
	Q_OBJECT

public:
	typedef iqt2d::TSceneExtenderCompBase<iqt::TDesignerGuiObserverCompBase<
				Ui::CAnnulusParamsGuiComp,
				i2d::CAnnulus> > BaseClass;

	I_BEGIN_COMPONENT(CAnnulusParamsGuiComp);
		I_ASSIGN(m_unitNameAttrPtr, "UnitName", "Name of geometric units e.g. mm", false, "mm");
		I_ASSIGN(m_annulusZValueAttrPtr, "AnnulusZValue", "Describe draw priority on display console (the objects with bigger value will overlap the other ones)", true, 1);
	I_END_COMPONENT;

	// reimplemented (iqt::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor();

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

	// reimplemented (iqt2d::TSceneExtenderCompBase)
	virtual void CreateShapes(int sceneId, bool inactiveOnly, Shapes& result);

protected slots:
	void OnParamsChanged(double value);

private:
	I_ATTR(istd::CString, m_unitNameAttrPtr);
	I_ATTR(double, m_annulusZValueAttrPtr);
};


} // namespace iqt2d


#endif // !iqt2d_CAnnulusParamsGuiComp_included


