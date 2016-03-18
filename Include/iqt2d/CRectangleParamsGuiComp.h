#ifndef iqt2d_CRectangleParamsGuiComp_included
#define iqt2d_CRectangleParamsGuiComp_included


#include "i2d/CRectangle.h"

#include "iview/CRectangleShape.h"

#include "iqt2d/TShapeParamsGuiCompBase.h"
#include "GeneratedFiles/iqt2d/ui_CRectangleParamsGuiComp.h"


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
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiRetranslate();

protected Q_SLOTS:
	void OnParamsChanged(double value);
};


} // namespace iqt2d


#endif // !iqt2d_CRectangleParamsGuiComp_included


