#pragma once


// ACF includes
#include <i2d/CQuadrangle.h>
#include <iview/CParallelogramShape.h>
#include <iqt2d/TShapeParamsGuiCompBase.h>
#include <GeneratedFiles/iqt2d/ui_CQuadrangleParamsGuiComp.h>


namespace iqt2d
{


class CQuadrangleParamsGuiComp:
			public iqt2d::TShapeParamsGuiCompBase<
						Ui::CQuadrangleParamsGuiComp,
						iview::CParallelogramShape,
						i2d::CQuadrangle>
{
	Q_OBJECT

public:
	typedef iqt2d::TShapeParamsGuiCompBase<
				Ui::CQuadrangleParamsGuiComp,
				iview::CParallelogramShape,
				i2d::CQuadrangle> BaseClass;

	I_BEGIN_COMPONENT(CQuadrangleParamsGuiComp);
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


