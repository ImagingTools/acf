#ifndef iqtgui_TRestorableGuiWrap_included
#define iqtgui_TRestorableGuiWrap_included


// ACF includes
#include <iqt/ISettingsProvider.h>
#include <icomp/CComponentBase.h>


namespace iqtgui
{


/**
	A wrapper for saving/restoring of GUI component states in the application settings.
*/
template <class Gui>
class TRestorableGuiWrap: public Gui
{
public:
	typedef Gui BaseClass;

	I_BEGIN_BASE_COMPONENT(TRestorableGuiWrap);
		I_ASSIGN(m_settingsProviderCompPtr, "SettingsProvider", "Application settings provider", false, "SettingsProvider");
	I_END_COMPONENT;

protected:
	// abstract methods
	virtual void OnRestoreSettings(const QSettings& settings) = 0;
	virtual void OnSaveSettings(QSettings& settings) const = 0;

	// pseudo-reimplemented (iqtgui::CGuiComponentBase)
	virtual bool CreateGui(QWidget* parentPtr);
	virtual bool DestroyGui();

private:
	I_REF(iqt::ISettingsProvider, m_settingsProviderCompPtr);
};


// protected methods

template <class Gui>
bool TRestorableGuiWrap<Gui>::CreateGui(QWidget* parentPtr)
{
	bool retVal = BaseClass::CreateGui(parentPtr);

	if (m_settingsProviderCompPtr.IsValid()){
		OnRestoreSettings(m_settingsProviderCompPtr->GetSettings());
	}

	return retVal;
}


template <class Gui>
bool TRestorableGuiWrap<Gui>::DestroyGui()
{
	if (m_settingsProviderCompPtr.IsValid()){
		OnSaveSettings(m_settingsProviderCompPtr->GetSettings());
	}

	return BaseClass::DestroyGui();
}


} // namespace iqtgui


#endif // !iqtgui_TRestorableGuiWrap_included


