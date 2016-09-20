#ifndef ifilegui_CFileListProviderGuiComp_included
#define ifilegui_CFileListProviderGuiComp_included


// ACF includes
#include <ifile/IFileListProvider.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>

#include <GeneratedFiles/ifilegui/ui_CFileListProviderGuiComp.h>


namespace ifilegui
{


class CFileListProviderGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CFileListProviderGuiComp, ifile::IFileListProvider>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CFileListProviderGuiComp, ifile::IFileListProvider> BaseClass;

	I_BEGIN_COMPONENT(CFileListProviderGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);
};


} // namespace ifilegui


#endif // !ifilegui_CFileListProviderGuiComp_included


