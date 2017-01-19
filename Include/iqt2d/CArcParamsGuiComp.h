#ifndef iqt2d_CArcParamsGuiComp_included
#define iqt2d_CArcParamsGuiComp_included


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


#endif // !iqt2d_CArcParamsGuiComp_included


