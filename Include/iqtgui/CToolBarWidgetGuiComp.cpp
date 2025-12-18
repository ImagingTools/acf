#include <iqtgui/CToolBarWidgetGuiComp.h>


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QMainWindow>
#else
#include <QtGui/QHBoxLayout>
#include <QtGui/QComboBox>
#include <QtGui/QMainWindow>
#endif


namespace iqtgui
{


// protected methods

// reimplemented (CGuiComponentBase)

void CToolBarWidgetGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QToolBar* toolBarPtr = GetQtWidget();
	Q_ASSERT(toolBarPtr != NULL);
	if (toolBarPtr == NULL){
		return;
	}

	if (m_toolbarWidgetCompPtr.IsValid()){
		iqtgui::IGuiObject* guiPtr = m_toolbarWidgetCompPtr.GetPtr();
		if (guiPtr != NULL){
			Q_ASSERT(!guiPtr->IsGuiCreated());

			if (guiPtr->CreateGui(NULL)){
				toolBarPtr->addWidget(guiPtr->GetWidget());
			}
		}
	}
}


void CToolBarWidgetGuiComp::OnGuiDestroyed()
{
	if (m_toolbarWidgetCompPtr.IsValid()){
		iqtgui::IGuiObject* guiPtr = m_toolbarWidgetCompPtr.GetPtr();
		if (guiPtr != NULL){
			guiPtr->DestroyGui();
		}
	}

	BaseClass::OnGuiDestroyed();
}


} // namespace iqtgui


