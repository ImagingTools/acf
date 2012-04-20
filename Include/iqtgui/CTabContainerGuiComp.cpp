#include "iqtgui/CTabContainerGuiComp.h"


// Qt includes
#include <QtGui/QGridLayout>
#include <QtGui/QFrame>


namespace iqtgui
{


// protected methods

void CTabContainerGuiComp::UpdateVisualElements()
{
	QTabWidget* widgetPtr = GetQtWidget();
	I_ASSERT(widgetPtr != NULL);

	int visualProvidersCount = m_slaveWidgetsVisualCompPtr.GetCount();

	for (		TabToGuiIndexMap::ConstIterator iter = m_tabToGuiIndexMap.begin();
				iter != m_tabToGuiIndexMap.end();
				++iter){
		int tabIndex = iter.key();
		I_ASSERT(tabIndex >= 0);
		I_ASSERT(tabIndex < widgetPtr->count());

		int guiIndex = iter.value();
		I_ASSERT(guiIndex >= 0);
		I_ASSERT(guiIndex < m_slaveWidgetsCompPtr.GetCount());

		QIcon tabIcon;
		QString toolTip;

		if (guiIndex < visualProvidersCount){
			const IVisualStatusProvider* visualProviderPtr = m_slaveWidgetsVisualCompPtr[guiIndex];
			if (visualProviderPtr != NULL){
				tabIcon = visualProviderPtr->GetStatusIcon();
				toolTip = visualProviderPtr->GetStatusText();
			}
		}

		widgetPtr->setTabIcon(tabIndex, tabIcon);
		widgetPtr->setTabToolTip(tabIndex, toolTip);
	}
}


// reimplemented (iqtgui::CComponentBase)

void CTabContainerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	I_ASSERT(IsGuiCreated());

	QTabWidget* widgetPtr = GetQtWidget();
	I_ASSERT(widgetPtr != NULL);
	if (widgetPtr == NULL){
		return;
	}

	if (*m_useTriangularTabsAttrPtr){
		widgetPtr->setTabShape(QTabWidget::Triangular);
	}

	widgetPtr->setTabPosition(QTabWidget::TabPosition(*m_tabOrientationAttrPtr));

	if (m_iconSizeAttrPtr.IsValid()){
		QSize iconSize = QSize(m_iconSizeAttrPtr->GetValue(), m_iconSizeAttrPtr->GetValue());
		if (!iconSize.isNull() && iconSize.isValid() && !iconSize.isEmpty()){
			widgetPtr->setIconSize(iconSize);
		}
	}

	if (m_tabNamesAttrPtr.IsValid()){
		int tabCount = qMin(m_tabNamesAttrPtr.GetCount(), m_slaveWidgetsCompPtr.GetCount());
		for (int guiIndex = 0; guiIndex < tabCount; guiIndex++){
			QString tabName = m_tabNamesAttrPtr[guiIndex];

			iqtgui::IGuiObject* guiPtr = m_slaveWidgetsCompPtr[guiIndex];
			if ((guiPtr == NULL) || !guiPtr->CreateGui(widgetPtr)){
				continue;
			}

			QWidget* pageWidget = new QWidget(widgetPtr);
			QVBoxLayout* pageLayout = new QVBoxLayout(pageWidget);

			pageLayout->addWidget(guiPtr->GetWidget());

			int tabIndex = widgetPtr->addTab(pageWidget, tabName);
			m_tabToGuiIndexMap[tabIndex] = guiIndex;

			if (guiIndex < m_slaveWidgetsModelCompPtr.GetCount()){
				imod::IModel* modelPtr = m_slaveWidgetsModelCompPtr[guiIndex];
				if (modelPtr != NULL){
					RegisterModel(modelPtr);
				}
			}
		}
	}

	// setup the corner widget:
	if (m_cornerGuiCompPtr.IsValid()){
		if (m_cornerGuiCompPtr->CreateGui(NULL)){
			widgetPtr->setCornerWidget(m_cornerGuiCompPtr->GetWidget());
		}
	}

	UpdateVisualElements();
}


void CTabContainerGuiComp::OnGuiDestroyed()
{
	I_ASSERT(m_slaveWidgetsCompPtr.IsValid());

	m_tabToGuiIndexMap.clear();

	if (m_slaveWidgetsCompPtr.IsValid()){
		int slaveWidgetsCount = m_slaveWidgetsCompPtr.GetCount();
		for (int widgetIndex = 0; widgetIndex < slaveWidgetsCount; widgetIndex++){
			m_slaveWidgetsCompPtr[widgetIndex]->DestroyGui();
		}
	}

	BaseClass::OnGuiDestroyed();
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CTabContainerGuiComp::OnModelChanged(int /*modelId*/, int /*changeFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	UpdateVisualElements();
}


} // namespace iqtgui


