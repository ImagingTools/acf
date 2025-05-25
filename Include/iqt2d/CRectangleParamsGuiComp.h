#ifndef iqt2d_CRectangleParamsGuiComp_included
#define iqt2d_CRectangleParamsGuiComp_included


#include <i2d/CRectangle.h>
#include <i2d/IObject2dProvider.h>

#include <iview/CRectangleShape.h>

#include <iqt2d/TShapeParamsGuiCompBase.h>
#include <GeneratedFiles/iqt2d/ui_CRectangleParamsGuiComp.h>


namespace iqt2d
{


class CRectangleParamsGuiComp:
			public iqt2d::TShapeParamsGuiCompBase<
						Ui::CRectangleParamsGuiComp,
						iview::CRectangleShape,
						i2d::CRectangle>
{
	Q_OBJECT

public:
	typedef iqt2d::TShapeParamsGuiCompBase<
				Ui::CRectangleParamsGuiComp,
				iview::CRectangleShape,
				i2d::CRectangle> BaseClass;

	I_BEGIN_COMPONENT(CRectangleParamsGuiComp);
		I_ASSIGN(m_centerScreenPointProviderCompPtr, "CenterScreenPointProvider", "Provides center screen point (i2d::CPosition2d)", false, "CenterScreenPointProvider");
		I_ASSIGN(m_showSpacerAttr, "ShowSpacer", "Show spacer", true, false);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached() override;
	virtual void OnGuiModelDetached() override;
	virtual void UpdateModel() const override;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiRetranslate() override;
	virtual void OnGuiCreated() override;

protected Q_SLOTS:
	void OnParamsChanged(double value);
	void on_MoveToCenterButton_clicked(bool = false);

private:
	I_REF(i2d::IObject2dProvider, m_centerScreenPointProviderCompPtr);
	I_ATTR(bool, m_showSpacerAttr);
};


} // namespace iqt2d


#endif // !iqt2d_CRectangleParamsGuiComp_included


