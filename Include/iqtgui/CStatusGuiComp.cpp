#include <iqtgui/CStatusGuiComp.h>


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QGroupBox>
#else
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QStatusBar>
#include <QtGui/QGroupBox>
#endif


namespace iqtgui
{


// reimplemented (iqtgui::TGuiObserverWrap)

void CStatusGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	static QIcon stateUnknownIcon(":/Icons/StateUnknown");
	static QIcon stateOkIcon(":/Icons/StateOk");
	static QIcon stateWarningIcon(":/Icons/StateWarning");
	static QIcon stateInvalidIcon(":/Icons/StateInvalid");

	QIcon icon;

	istd::IInformationProvider* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		switch (objectPtr->GetInformationCategory()){
		case istd::IInformationProvider::IC_INFO:
			icon = stateOkIcon;
			break;

		case istd::IInformationProvider::IC_WARNING:
			icon = stateWarningIcon;
			break;

		case istd::IInformationProvider::IC_ERROR:
			icon = stateInvalidIcon;
			break;

		case istd::IInformationProvider::IC_CRITICAL:
			icon = stateInvalidIcon;
			break;

		default:
			icon = stateUnknownIcon;
			break;
		}
	}
	else{
		icon = stateUnknownIcon;
	}

	QLabel* labelPtr = GetQtWidget();
	Q_ASSERT(labelPtr != NULL);

	labelPtr->setPixmap(icon.pixmap(*m_iconSizeAttrPtr));
}


// reimplemented (iqtgui::CGuiComponentBase)

void CStatusGuiComp::OnGuiCreated()
{
	QLabel* labelPtr = GetQtWidget();
	Q_ASSERT(labelPtr != NULL);

	labelPtr->setMinimumSize(*m_iconSizeAttrPtr, *m_iconSizeAttrPtr);
	labelPtr->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	labelPtr->setScaledContents(false);
}


} // namespace iqtgui


