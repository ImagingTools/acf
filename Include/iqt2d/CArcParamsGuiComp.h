#pragma once


#include <i2d/CArc.h>

#include <iview/CArcShape.h>

#include <iqt2d/TShapeParamsGuiCompBase.h>
#include <GeneratedFiles/iqt2d/ui_CArcParamsGuiComp.h>


namespace iqt2d
{


class CArcParamsGuiComp: 
			public iqt2d::TShapeParamsGuiCompBase<
						Ui::CArcParamsGuiComp,
						iview::CArcShape,
						i2d::CArc>
{
	Q_OBJECT

public:
	typedef iqt2d::TShapeParamsGuiCompBase<
				Ui::CArcParamsGuiComp,
				iview::CArcShape,
				i2d::CCircle> CArc;

	I_BEGIN_COMPONENT(CArcParamsGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateModel() const override;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiDestroyed() override;
	virtual void OnGuiRetranslate() override;

protected Q_SLOTS:
	void OnParamsChanged(double value);
};


} // namespace iqt2d

