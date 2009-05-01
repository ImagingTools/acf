#include "iqtgui/CLayoutGuiComp.h"


#include <QLayout>
#include <QSplitter>
#include <QFrame>


namespace iqtgui
{


// public methods

// reimplemented (iqtgui::CComponentBase)

void CLayoutGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QWidget* widgetPtr = GetWidget();
	I_ASSERT(widgetPtr != NULL);
	if (widgetPtr == NULL){
		return;
	}

	int widgetsCount = 0;
	if (m_slaveWidgetsCompPtr.IsValid()){
		widgetsCount = 	m_slaveWidgetsCompPtr.GetCount();
	}

	QSplitter* splitterPtr = NULL;
	QLayout* layoutPtr = new QVBoxLayout(GetWidget());

	if (m_layoutOrientationAttrPtr.IsValid()){
		switch (*m_layoutOrientationAttrPtr){
			case 0:
				splitterPtr = new QSplitter(Qt::Horizontal, GetWidget());
				break;
			case 1:
				splitterPtr = new QSplitter(Qt::Vertical, GetWidget());
				break;
		}
	}

	if (splitterPtr != NULL){
		layoutPtr->addWidget(splitterPtr);

		for (int slaveIndex = 0; slaveIndex < widgetsCount; slaveIndex++){
			iqtgui::IGuiObject* guiPtr = m_slaveWidgetsCompPtr[slaveIndex];
			if (guiPtr != NULL){
				if (guiPtr->CreateGui(splitterPtr)){
					splitterPtr->addWidget(guiPtr->GetWidget());
				}
			}
		}
	}
}


void CLayoutGuiComp::OnGuiDestroyed()
{
	I_ASSERT(m_slaveWidgetsCompPtr.IsValid());

	if (m_slaveWidgetsCompPtr.IsValid()){
		int slaveWidgetsCount = m_slaveWidgetsCompPtr.GetCount();
		for (int widgetIndex = 0; widgetIndex < slaveWidgetsCount; widgetIndex++){
			m_slaveWidgetsCompPtr[widgetIndex]->DestroyGui();
		}
	}

	BaseClass::OnGuiDestroyed();
}


} // namespace iqtgui


