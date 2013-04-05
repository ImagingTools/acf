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
	Q_ASSERT(widgetPtr != NULL);

	int visualProvidersCount = m_slaveWidgetsVisualCompPtr.GetCount();

	for (		TabToGuiIndexMap::ConstIterator iter = m_tabToGuiIndexMap.begin();
				iter != m_tabToGuiIndexMap.end();
				++iter){
		int tabIndex = iter.key();
		Q_ASSERT(tabIndex >= 0);
		Q_ASSERT(tabIndex < widgetPtr->count());

		int guiIndex = iter.value();
		Q_ASSERT(guiIndex >= 0);
		Q_ASSERT(guiIndex < m_slaveWidgetsCompPtr.GetCount());

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


// reimplemented (iqtgui::CGuiComponentBase)

void CTabContainerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	Q_ASSERT(IsGuiCreated());

	QTabWidget* widgetPtr = GetQtWidget();
	Q_ASSERT(widgetPtr != NULL);
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

	int tabsCount = m_tabModel.GetOptionsCount();

	bool isFlat = *m_flatStyleAttrPtr;

	if (isFlat){
		widgetPtr->setStyleSheet("QTabWidget::pane{border:0px none;}");
	}

	for (int guiIndex = 0; guiIndex < tabsCount; guiIndex++){
		QString tabName = m_tabNamesAttrPtr[guiIndex];
		iqtgui::IGuiObject* guiPtr = m_slaveWidgetsCompPtr[guiIndex];
		if ((guiPtr != NULL) && guiPtr->CreateGui(widgetPtr)){
			QWidget* pageWidget = new QWidget(widgetPtr);
			QVBoxLayout* pageLayout = new QVBoxLayout(pageWidget);

			pageLayout->addWidget(guiPtr->GetWidget());

			if (isFlat){
				pageLayout->setContentsMargins(0, 0, 0, 0);
			}

			int tabIndex = widgetPtr->addTab(pageWidget, tabName);
			m_tabToGuiIndexMap[tabIndex] = guiIndex;

			if (		(guiIndex < m_slaveWidgetsVisualCompPtr.GetCount()) &&
						(guiIndex < m_slaveWidgetsModelCompPtr.GetCount())){
				imod::IModel* modelPtr = m_slaveWidgetsModelCompPtr[guiIndex];
				if ((m_slaveWidgetsVisualCompPtr[guiIndex] != NULL) && (modelPtr != NULL)){
					RegisterModel(modelPtr, guiIndex);
				}
			}
		}
	}

	widgetPtr->setCurrentIndex(m_tabModel.GetSelectedOptionIndex());

	// setup the corner widget:
	if (m_cornerGuiCompPtr.IsValid()){
		if (m_cornerGuiCompPtr->CreateGui(NULL)){
			widgetPtr->setCornerWidget(m_cornerGuiCompPtr->GetWidget());
		}
	}

	UpdateVisualElements();
	m_tabModel.UpdateTabsState();
}


void CTabContainerGuiComp::OnGuiDestroyed()
{
	Q_ASSERT(m_slaveWidgetsCompPtr.IsValid());

	m_tabToGuiIndexMap.clear();

	if (m_slaveWidgetsCompPtr.IsValid()){
		int slaveWidgetsCount = m_slaveWidgetsCompPtr.GetCount();
		for (int widgetIndex = 0; widgetIndex < slaveWidgetsCount; widgetIndex++){
			iqtgui::IGuiObject* guiPtr = m_slaveWidgetsCompPtr[widgetIndex];
			if (guiPtr != NULL){
				guiPtr->DestroyGui();
			}
		}
	}

	BaseClass::OnGuiDestroyed();
}


// reimplemented (icomp::CComponentBase)

void CTabContainerGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_tabModel.SetParent(this);
}


void CTabContainerGuiComp::OnComponentDestroyed()
{
	m_tabModel.SetParent(NULL);

	BaseClass::OnComponentDestroyed();
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CTabContainerGuiComp::OnModelChanged(int /*modelId*/, int /*changeFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (IsGuiCreated()){
		UpdateVisualElements();
	}
}


// public methods of the embedded class TabModel

CTabContainerGuiComp::TabModel::TabModel()
:	m_parentPtr(NULL)
{
	m_selectedOptionIndex = 0;

	BaseClass::SetSelectionConstraints(this);
}


void CTabContainerGuiComp::TabModel::SetParent(CTabContainerGuiComp* parentPtr)
{
	if (parentPtr != m_parentPtr){
		if (m_parentPtr != NULL){
			EnsureModelsDetached();
		}

		if (parentPtr != NULL){
			int activatorsCount = qMin(parentPtr->m_tabNamesAttrPtr.GetCount(), parentPtr->m_tabActivatorsModelCompPtr.GetCount());
			for (int tabIndex = 0; tabIndex < activatorsCount; ++tabIndex){
				imod::IModel* modelPtr = parentPtr->m_tabActivatorsModelCompPtr[tabIndex];

				modelPtr->AttachObserver(this);
			}
		}

		m_parentPtr = parentPtr;
	}
}


void CTabContainerGuiComp::TabModel::UpdateTabsState()
{
	Q_ASSERT(m_parentPtr != NULL);

	QTabWidget* tabWidgetPtr = m_parentPtr->GetQtWidget();
	Q_ASSERT(tabWidgetPtr != NULL);

	int activatorsCount = qMin(m_parentPtr->m_tabActivatorsCompPtr.GetCount(), tabWidgetPtr->count());
	for (int i = 0; i < activatorsCount; ++i){
		const iprm::IEnableableParam* paramPtr = m_parentPtr->m_tabActivatorsCompPtr[i];
		if (paramPtr != NULL){
			tabWidgetPtr->setTabEnabled(i, paramPtr->IsEnabled());
		}
	}
}


// reimplemented (iprm::ISelectionParam)

bool CTabContainerGuiComp::TabModel::SetSelectedOptionIndex(int index)
{
	Q_ASSERT(m_parentPtr != NULL);

	if (BaseClass::SetSelectedOptionIndex(index)){
		QTabWidget* tabWidgetPtr = m_parentPtr->GetQtWidget();
		if (tabWidgetPtr != NULL){
			tabWidgetPtr->setCurrentIndex(index);
		}
	}

	return false;
}


// reimplemented (iprm::IOptionsList)

int CTabContainerGuiComp::TabModel::GetOptionsFlags() const
{
	return SCF_SUPPORT_DISABLED | SCF_SUPPORT_UNIQUE_ID;
}


int CTabContainerGuiComp::TabModel::GetOptionsCount() const
{
	Q_ASSERT(m_parentPtr != NULL);

	return m_parentPtr->m_tabNamesAttrPtr.GetCount();
}


QString CTabContainerGuiComp::TabModel::GetOptionName(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);
	Q_ASSERT(index < m_parentPtr->m_tabNamesAttrPtr.GetCount());

	return m_parentPtr->m_tabNamesAttrPtr[index];
}


QString CTabContainerGuiComp::TabModel::GetOptionDescription(int /*index*/) const
{
	return "";
}


QByteArray CTabContainerGuiComp::TabModel::GetOptionId(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);
	Q_ASSERT(index < m_parentPtr->m_tabNamesAttrPtr.GetCount());

	return m_parentPtr->m_tabNamesAttrPtr[index].toUtf8();
}


bool CTabContainerGuiComp::TabModel::IsOptionEnabled(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);

	if (index >= m_parentPtr->m_slaveWidgetsCompPtr.GetCount()){
		return false;
	}

	if (index < m_parentPtr->m_tabActivatorsCompPtr.GetCount()){
		const iprm::IEnableableParam* paramPtr = m_parentPtr->m_tabActivatorsCompPtr[index];
		if (paramPtr != NULL){
			return paramPtr->IsEnabled();
		}
	}

	return true;
}


// protected methods of the embedded class TabModel

// reimplemented (istd::IChangeable)

void CTabContainerGuiComp::TabModel::OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	Q_ASSERT(m_parentPtr != NULL);

	BaseClass::OnEndChanges(changeFlags, changeParamsPtr);

	if (m_parentPtr->IsGuiCreated()){
		UpdateTabsState();
	}
}


} // namespace iqtgui


