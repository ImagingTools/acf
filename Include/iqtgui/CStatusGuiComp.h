#pragma once


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QLabel>
#else
#include <QtGui/QLabel>
#endif

// ACF includes
#include <istd/IInformationProvider.h>
#include <imod/TSingleModelObserverBase.h>
#include <iqtgui/TGuiComponentBase.h>
#include <iqtgui/TGuiObserverWrap.h>
#include <iqtgui/TMakeStateIconWrapper.h>


namespace iqtgui
{


class CStatusGuiComp:
			public iqtgui::StateIconWrapper< 
				iqtgui::TGuiObserverWrap<
						iqtgui::TGuiComponentBase<QLabel>,
						imod::TSingleModelObserverBase<istd::IInformationProvider> > >
{
public:
	typedef iqtgui::StateIconWrapper< 
		iqtgui::TGuiObserverWrap<
				iqtgui::TGuiComponentBase<QLabel>,
				imod::TSingleModelObserverBase<istd::IInformationProvider> > > BaseClass;

	I_BEGIN_COMPONENT(CStatusGuiComp);
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size of status icon", true, 64);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;

private:
	I_ATTR(int, m_iconSizeAttrPtr);
};


} // namespace iqtgui


