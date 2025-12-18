#ifndef iview_CShapeInfoGuiComp_included
#define iview_CShapeInfoGuiComp_included


// ACF includes
#include <iqtgui/TDesignerGuiObserverCompBase.h>

#include <iview/IShapeStatusInfo.h>
#include <GeneratedFiles/iview/ui_CShapeInfoGuiComp.h>


namespace iview
{


class CShapeInfoGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<Ui::CShapeInfoGuiComp, IShapeStatusInfo>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<Ui::CShapeInfoGuiComp, IShapeStatusInfo> BaseClass;

	I_BEGIN_COMPONENT(CShapeInfoGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;
};


} // namespace iview


#endif // !iview_CShapeInfoGuiComp_included


