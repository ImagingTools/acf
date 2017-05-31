#ifndef iqt2d_CImageViewComp_included
#define iqt2d_CImageViewComp_included


// ACF includes
#include <icomp/CComponentBase.h>
#include <i2d/ICalibration2d.h>
#include <i2d/ICalibrationProvider.h>
#include <iqtgui/TGuiComponentBase.h>
#include <iqtgui/TGuiObserverWrap.h>
#include <iview/CImageShape.h>
#include <iqt2d/CViewProviderGuiComp.h>


namespace iqt2d
{


/**
	Show observed bitmap on the 2D-console.
	This class extends standard 2D-console to provide background bitmap object.
*/
class CImageViewComp:
			public iqtgui::TGuiObserverWrap<CViewProviderGuiComp, iview::CImageShape>

{
public:
	typedef iqtgui::TGuiObserverWrap<CViewProviderGuiComp, iview::CImageShape> BaseClass;

	I_BEGIN_COMPONENT(CImageViewComp);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_REGISTER_INTERFACE(imod::IModelEditor);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtui::CGuiComponentBase)
	virtual void OnGuiCreated();
};


} // namespace iqt2d


#endif // !iqt2d_CImageViewComp_included


