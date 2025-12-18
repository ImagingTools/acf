#ifndef iqtgui_CImagePropertyGuiComp_included
#define iqtgui_CImagePropertyGuiComp_included


#include <iimg/IBitmap.h>

#include <iqtgui/TDesignerGuiObserverCompBase.h>
#include <GeneratedFiles/iqtgui/ui_CImagePropertyGuiComp.h>


namespace iqtgui
{


class CImagePropertyGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CImagePropertyGuiComp, iimg::IBitmap>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CImagePropertyGuiComp, iimg::IBitmap> BaseClass;

	I_BEGIN_COMPONENT(CImagePropertyGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;
};


} // namespace iqtgui


#endif // !iqtgui_CImagePropertyGuiComp_included


