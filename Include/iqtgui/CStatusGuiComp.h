#ifndef iqtgui_CStatusGuiComp_included
#define iqtgui_CStatusGuiComp_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QLabel>
#else
#include <QtGui/QLabel>
#endif

// ACF includes
#include <istd/IInformationProvider.h>
#include <ibase/TModelObserverCompWrap.h>
#include <iqtgui/TGuiComponentBase.h>
#include <iqtgui/TGuiObserverWrap.h>


namespace iqtgui
{


class CStatusGuiComp:
			public ibase::TModelObserverCompWrap<
						iqtgui::TGuiObserverWrap<
									iqtgui::TGuiComponentBase<QLabel>,
									imod::TSingleModelObserverBase<istd::IInformationProvider> > >
{
public:
	typedef ibase::TModelObserverCompWrap<
				iqtgui::TGuiObserverWrap<
							iqtgui::TGuiComponentBase<QLabel>,
							imod::TSingleModelObserverBase<istd::IInformationProvider> > > BaseClass;

	I_BEGIN_COMPONENT(CStatusGuiComp);
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size of status icon", true, 64);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

private:
	I_ATTR(int, m_iconSizeAttrPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CStatusGuiComp_included

