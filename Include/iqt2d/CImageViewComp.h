#ifndef iqt2d_CImageViewComp_included
#define iqt2d_CImageViewComp_included


// ACF includes
#include "icomp/CComponentBase.h"

#include "i2d/ICalibrationProvider.h"

#include "ibase/TModelObserverCompWrap.h"

#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/TGuiObserverWrap.h"

#include "iview/CImageShape.h"

#include "iqt2d/CViewProviderGuiComp.h"


namespace iqt2d
{


/**
	Show observed bitmap on the 2D-console.
	This class extends standard 2D-console to provide background bitmap object.
*/
class CImageViewComp:
			public ibase::TModelObserverCompWrap<
						iqtgui::TGuiObserverWrap<CViewProviderGuiComp, iview::CImageShape> >,
						virtual public i2d::ICalibrationProvider

{
public:
	typedef ibase::TModelObserverCompWrap<
				iqtgui::TGuiObserverWrap<CViewProviderGuiComp, iview::CImageShape> > BaseClass;

	I_BEGIN_COMPONENT(CImageViewComp);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_REGISTER_INTERFACE(imod::IModelEditor);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
	I_END_COMPONENT;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ITransformation2d* GetCalibration() const;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags);

	// reimplemented (iqtui::CGuiComponentBase)
	virtual void OnGuiCreated();
};


} // namespace iqt2d


#endif // !iqt2d_CImageViewComp_included


