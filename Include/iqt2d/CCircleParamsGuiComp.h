#ifndef iqt2d_CCircleParamsGuiComp_included
#define iqt2d_CCircleParamsGuiComp_included


#include "i2d/CCircle.h"

#include "iqt2d/TShapeParamsGuiCompBase.h"
#include "iqt2d/CCircleShape.h"

#include "iqt2d/Generated/ui_CCircleParamsGuiComp.h"


namespace iqt2d
{


class CCircleParamsGuiComp: 
			public iqt2d::TShapeParamsGuiCompBase<
						Ui::CCircleParamsGuiComp,
						iqt2d::CCircleShape,
						i2d::CCircle>
{
	Q_OBJECT

public:
	typedef iqt2d::TShapeParamsGuiCompBase<
				Ui::CCircleParamsGuiComp,
				iqt2d::CCircleShape,
				i2d::CCircle> BaseClass;

	I_BEGIN_COMPONENT(CCircleParamsGuiComp);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(int updateFlags = 0);

protected Q_SLOTS:
	void OnParamsChanged(double value);

private:
	I_ATTR(QString, m_unitNameAttrPtr);
	I_ATTR(double, m_circleZValueAttrPtr);
};


} // namespace iqt2d


#endif // !iqt2d_CCircleParamsGuiComp_included


