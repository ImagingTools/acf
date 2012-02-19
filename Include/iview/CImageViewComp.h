#ifndef iview_CImageViewComp_included
#define iview_CImageViewComp_included


// ACF includes
#include "icomp/CComponentBase.h"

#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/TGuiObserverWrap.h"


// QSF includes
#include "iview/CImageShape.h"

#include "iview/CNoneCalibration.h"
#include "iview/CNoneCalibrationShape.h"
#include "iview/CConsoleGui.h"


namespace iview
{


/**
	Show observed bitmap on the 2D-console.
	This class extends standard 2D-console to provide background bitmap object.
*/
class CImageViewComp:
			public iqtgui::TGuiObserverWrap<
						iqtgui::TGuiComponentBase<iview::CConsoleGui>,
						iview::CImageShape>,
			virtual public ibase::ICommandsProvider
{
public:
	typedef iqtgui::TGuiObserverWrap<
				iqtgui::TGuiComponentBase<iview::CConsoleGui>,
				iview::CImageShape> BaseClass;

	I_BEGIN_COMPONENT(CImageViewComp);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_REGISTER_INTERFACE(imod::IModelEditor);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN(m_showControlButtonsAttrPtr, "ShowControlButtons", "If true, control buttons will be shown", true, false);
	I_END_COMPONENT;

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags);
	virtual void OnGuiModelAttached();

private:
	I_ATTR(bool, m_showControlButtonsAttrPtr);
	iview::CNoneCalibration m_calibration;
	iview::CNoneCalibrationShape m_calibrationShape;
};


} // namespace iview


#endif // !iview_CImageViewComp_included


