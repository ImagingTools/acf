#ifndef iqtgui_CFileListProviderGuiComp_included
#define iqtgui_CFileListProviderGuiComp_included


#include "ibase/IFileListProvider.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/Generated/ui_CFileListProviderGuiComp.h"


namespace iqtgui
{


class CFileListProviderGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CFileListProviderGuiComp, ibase::IFileListProvider>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CFileListProviderGuiComp, ibase::IFileListProvider> BaseClass;

	I_BEGIN_COMPONENT(CFileListProviderGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);
};


} // namespace iqtgui


#endif // !iqtgui_CFileListProviderGuiComp_included


