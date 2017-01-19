#ifndef iqt2d_CQuadrangleParamsGuiComp_included
#define iqt2d_CQuadrangleParamsGuiComp_included


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
	virtual void UpdateModel() const;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnGuiRetranslate();

protected Q_SLOTS:
	void OnParamsChanged(double value);
};


} // namespace iqt2d


#endif // !iqt2d_CQuadrangleParamsGuiComp_included


