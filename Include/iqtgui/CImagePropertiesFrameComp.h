#ifndef iqtgui_CImagePropertiesFrameComp_included
#define iqtgui_CImagePropertiesFrameComp_included


#include "iimg/IBitmap.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/Generated/ui_CImagePropertiesFrameComp.h"


namespace iqtgui
{


class CImagePropertiesFrameComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CImagePropertiesFrameComp, iimg::IBitmap>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CImagePropertiesFrameComp, iimg::IBitmap> BaseClass;

	I_BEGIN_COMPONENT(CImagePropertiesFrameComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);
	virtual void OnGuiModelDetached();
};


} // namespace iqtgui


#endif // !iqtgui_CImagePropertiesFrameComp_included


