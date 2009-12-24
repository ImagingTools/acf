#include "iqtgui/CComposedGuiComp.h"


// Qt includes
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QToolBox>
#include <QTabWidget>
#include <QGroupBox>

#include "imod/IModel.h"
#include "imod/IObserver.h"


namespace iqtgui
{


// reimplemented (iqtgui::CGuiComponentBase)

void CComposedGuiComp::OnGuiCreated()
{
	QWidget* widgetPtr = GetWidget();
	I_ASSERT(widgetPtr != NULL);	// GUI was created, main widget must be present

	QLayout* layoutPtr = NULL;
	if (*m_useHorizontalLayoutAttrPtr){
		layoutPtr = new QHBoxLayout(widgetPtr);
	}
	else{
		layoutPtr = new QVBoxLayout(widgetPtr);
	}

	layoutPtr->setMargin(0);

	if (*m_designTypeAttrPtr == 1){
		widgetPtr->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

		QToolBox* toolBoxPtr = new QToolBox(widgetPtr);
		int elementsCount = m_guisCompPtr.GetCount();
		for (int i = 0; i < elementsCount; ++i){
			iqtgui::IGuiObject* guiPtr = m_guisCompPtr[i];

			if (guiPtr != NULL){
				QWidget* panelPtr = new QWidget(widgetPtr);
				QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
				panelLayoutPtr->setContentsMargins(6, 0, 6, 0);
				QString name;
				if (i < m_namesAttrPtr.GetCount()){
					name = iqt::GetQString(m_namesAttrPtr[i]);
				}

				guiPtr->CreateGui(panelPtr);

				toolBoxPtr->addItem(panelPtr, name);
 
				QSpacerItem* spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

				panelLayoutPtr->addItem(spacerPtr);
			}
		}

		layoutPtr->addWidget(toolBoxPtr);
	}
	else if (*m_designTypeAttrPtr == 2){
		widgetPtr->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

		QTabWidget* tabWidgetPtr = new QTabWidget(widgetPtr);
		int elementsCount = m_guisCompPtr.GetCount();
		for (int i = 0; i < elementsCount; ++i){
			iqtgui::IGuiObject* guiPtr = m_guisCompPtr[i];

			if (guiPtr != NULL){
				QWidget* panelPtr = new QWidget(tabWidgetPtr);
				QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
				QString name;
				if (i < m_namesAttrPtr.GetCount()){
					name = iqt::GetQString(m_namesAttrPtr[i]);
				}

				guiPtr->CreateGui(panelPtr);

				tabWidgetPtr->addTab(panelPtr, name);

				QSpacerItem* spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

				panelLayoutPtr->addItem(spacerPtr);
			}
		}

		layoutPtr->addWidget(tabWidgetPtr);
	}
	else{
		int elementsCount = m_guisCompPtr.GetCount();
		for (int i = 0; i < elementsCount; ++i){
			iqtgui::IGuiObject* guiPtr = m_guisCompPtr[i];

			QWidget* elementParentPtr = widgetPtr;

			if (i < m_namesAttrPtr.GetCount()){
				QLayout* parentLayoutPtr = elementParentPtr->layout();

				elementParentPtr = new QGroupBox(iqt::GetQString(m_namesAttrPtr[i]), elementParentPtr);
				new QVBoxLayout(elementParentPtr);

				if (parentLayoutPtr != NULL){
					parentLayoutPtr->addWidget(elementParentPtr);
				}
			}

			if (guiPtr != NULL){
				guiPtr->CreateGui(elementParentPtr);
			}
		}
	}

	BaseClass::OnGuiCreated();
}


} // namespace iqtgui


