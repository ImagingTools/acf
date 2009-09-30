#ifndef iqtmm_CVideoPlayerGuiComp_included
#define iqtmm_CVideoPlayerGuiComp_included


#include "imod/IModel.h"
#include "imod/CSingleModelObserverBase.h"

#include "iprm/IFileNameParam.h"

#include "imm/IMediaController.h"

#include "iqtgui/TDesignerGuiCompBase.h"

#include "generated/ui_CVideoPlayerGuiComp.h"


namespace iqtmm
{


/**
	GUI component used to display video player.
*/
class CVideoPlayerGuiComp:
			public iqtgui::TDesignerGuiCompBase<Ui::CVideoPlayerGuiComp>,
			protected imod::CSingleModelObserverBase
{
public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CVideoPlayerGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CVideoPlayerGuiComp);
		I_ASSIGN(m_mediaControllerCompPtr, "MediaController", "Controller used to open and display media", true, "MediaController");
		I_ASSIGN(m_videoGuiCompPtr, "VideoGui", "View for a video display", true, "VideoGui");
		I_ASSIGN(m_urlParamsGuiCompPtr, "UrlGui", "Sequence directory selection GUI", false, "UrlGui");
		I_ASSIGN(m_controllerBarGuiCompPtr, "ControllerBarGui", "Displayed on the bottom used typically to controll of playback", false, "ControllerBarGui");
		I_ASSIGN(m_urlParamCompPtr, "UrlParams", "Parameter storing media URL or file or directory", true, "UrlParams");
		I_ASSIGN(m_urlParamModelCompPtr, "UrlParams", "Parameter storing media URL or file or directory", true, "UrlParams");
	I_END_COMPONENT();

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

protected:
	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	I_REF(imm::IMediaController, m_mediaControllerCompPtr);
	I_REF(iqtgui::IGuiObject, m_videoGuiCompPtr);
	I_REF(iqtgui::IGuiObject, m_urlParamsGuiCompPtr);
	I_REF(iqtgui::IGuiObject, m_controllerBarGuiCompPtr);
	I_REF(iprm::IFileNameParam, m_urlParamCompPtr);
	I_REF(imod::IModel, m_urlParamModelCompPtr);
};


} // namespace iqtmm


#endif // !iqtmm_CVideoPlayerGuiComp_included

