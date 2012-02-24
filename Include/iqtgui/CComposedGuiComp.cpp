#include "iqtgui/CComposedGuiComp.h"


// Qt includes
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QToolBox>
#include <QTabWidget>
#include <QGroupBox>


namespace iqtgui
{


// public methods

CComposedGuiComp::CComposedGuiComp()
	:m_splitterPtr(NULL)
{
}


// reimplemented (TRestorableGuiWrap)

void CComposedGuiComp::OnRestoreSettings(const QSettings& settings)
{
	I_ASSERT(IsGuiCreated());

	if (m_splitterPtr != NULL){
		// preserve overriding of splitter orientation:
		Qt::Orientation splitterOrientation = m_splitterPtr->orientation();

		QByteArray splitterState = settings.value(GetSettingsKey()).toByteArray();

		m_splitterPtr->restoreState(splitterState);

		m_splitterPtr->setOrientation(splitterOrientation);
	}
}


void CComposedGuiComp::OnSaveSettings(QSettings& settings) const
{
	I_ASSERT(IsGuiCreated());

	if (m_splitterPtr != NULL){
		QByteArray splitterState = m_splitterPtr->saveState();

		settings.setValue(GetSettingsKey(), splitterState);
	}
}


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
					name = m_namesAttrPtr[i];
				}

				guiPtr->CreateGui(panelPtr);

				toolBoxPtr->addItem(panelPtr, name);

				// TODO: candidate to remove
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
					name = m_namesAttrPtr[i];
				}

				guiPtr->CreateGui(panelPtr);

				tabWidgetPtr->addTab(panelPtr, name);

				// TODO: candidate to remove
				QSpacerItem* spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

				panelLayoutPtr->addItem(spacerPtr);
			}
		}

		layoutPtr->addWidget(tabWidgetPtr);
	}
	else if (*m_designTypeAttrPtr == 3){
		int elementsCount = m_guisCompPtr.GetCount();
		m_splitterPtr = new QSplitter(widgetPtr);

		if (*m_useHorizontalLayoutAttrPtr){
			m_splitterPtr->setOrientation(Qt::Horizontal);
		}
		else{
			m_splitterPtr->setOrientation(Qt::Vertical);
		}

		for (int i = 0; i < elementsCount; ++i){
			iqtgui::IGuiObject* guiPtr = m_guisCompPtr[i];
			if (guiPtr != NULL){
				guiPtr->CreateGui(widgetPtr);

				m_splitterPtr->addWidget(guiPtr->GetWidget());
			}
		}

		layoutPtr->addWidget(m_splitterPtr);
	}
	else{
		int elementsCount = m_guisCompPtr.GetCount();
		for (int i = 0; i < elementsCount; ++i){
			iqtgui::IGuiObject* guiPtr = m_guisCompPtr[i];

			QWidget* elementParentPtr = widgetPtr;

			if (i < m_namesAttrPtr.GetCount()){
				QLayout* parentLayoutPtr = elementParentPtr->layout();

				elementParentPtr = new QGroupBox(m_namesAttrPtr[i], elementParentPtr);
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


// private methods

QString CComposedGuiComp::GetSettingsKey() const
{
	QString settingsKey = "Splitter";
	if (m_settingsKeyAttrPtr.IsValid()){
		settingsKey = *m_settingsKeyAttrPtr + QString("/") + settingsKey;
	}

	return settingsKey;
}


} // namespace iqtgui


