#ifndef iqt2d_CCircleParamsGuiComp_included
#define iqt2d_CCircleParamsGuiComp_included


// ACF includes
#include <i2d/CCircle.h>
#include <iview/CCircleShape.h>
#include <iqt2d/TCircleBasedParamsGuiComp.h>
#include <i2d/IObject2dProvider.h>

#include <GeneratedFiles/iqt2d/ui_CCircleParamsGuiComp.h>


namespace iqt2d
{


class CCircleParamsGuiComp: 
			public iqt2d::TCircleBasedParamsGuiComp<
						iview::CCircleShape,
						i2d::CCircle>
{
	Q_OBJECT

public:
	typedef iqt2d::TCircleBasedParamsGuiComp<
				iview::CCircleShape,
				i2d::CCircle> BaseClass;

	I_BEGIN_COMPONENT(CCircleParamsGuiComp);
		I_ASSIGN(m_centerScreenPointProviderCompPtr, "CenterScreenPointProvider", "Provides center screen point (i2d::CPosition2d)", false, "CenterScreenPointProvider");
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;

protected Q_SLOTS:
	void OnParamsChanged(double value);
	void OnActionTriggered(QAction* actionPtr);
	void on_MoveToCenterButton_clicked(bool = false);

private:
	I_REF(i2d::IObject2dProvider, m_centerScreenPointProviderCompPtr);
};


} // namespace iqt2d


#endif // !iqt2d_CCircleParamsGuiComp_included


