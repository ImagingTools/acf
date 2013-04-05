#include "iqtgui/CSettingsContainerGuiComp.h"


// Qt includes
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>


namespace iqtgui
{


// protected methods

void CSettingsContainerGuiComp::UpdateVisualElements()
{
	Q_ASSERT(m_stackedWidgetPtr != NULL);

	int visualProvidersCount = m_slaveWidgetsVisualCompPtr.GetCount();

	for (		TabToGuiIndexMap::ConstIterator iter = m_tabToGuiIndexMap.begin();
				iter != m_tabToGuiIndexMap.end();
				++iter){
		int tabIndex = iter.key();
		Q_ASSERT(tabIndex >= 0);
		Q_ASSERT(tabIndex < m_stackedWidgetPtr->count());

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

		QListWidgetItem* itemPtr = m_menuListPtr->item(tabIndex);
		if (itemPtr != NULL){
			itemPtr->setIcon(tabIcon);
			itemPtr->setToolTip(toolTip);
		}
	}
}


// reimplemented (iqtgui::CComponentBase)

void CSettingsContainerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	Q_ASSERT(IsGuiCreated());

	QWidget* widgetPtr = GetQtWidget();
	Q_ASSERT(widgetPtr != NULL);
	if (widgetPtr == NULL){
		return;
	}

	QHBoxLayout* mainLayoutPtr = new QHBoxLayout(widgetPtr);
	mainLayoutPtr->setContentsMargins(0, 0, 0, 0);
	mainLayoutPtr->setSpacing(0);

	m_menuListPtr = new QListWidget(widgetPtr);
	mainLayoutPtr->addWidget(m_menuListPtr);

	m_stackedWidgetPtr = new QStackedWidget(widgetPtr);
	m_stackedWidgetPtr->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
	mainLayoutPtr->addWidget(m_stackedWidgetPtr);

	QSize iconSize(16, 16);
	if (m_iconSizeAttrPtr.IsValid()){
		QSize newIconSize = QSize(m_iconSizeAttrPtr->GetValue(), m_iconSizeAttrPtr->GetValue());
		if (!newIconSize.isNull() && newIconSize.isValid() && !newIconSize.isEmpty()){
			iconSize = newIconSize;
		}
	}

	m_menuListPtr->setIconSize(iconSize);
	m_menuListPtr->setUniformItemSizes(true);
	m_menuListPtr->setAlternatingRowColors(*m_alternateColorsAttrPtr);

	if (m_fixedWidthAttrPtr.IsValid()){
		int width = qMax(iconSize.width(), *m_fixedWidthAttrPtr);
		m_menuListPtr->setFixedWidth(width);
	}

	int tabsCount = m_tabModel.GetOptionsCount();
	for (int guiIndex = 0; guiIndex < tabsCount; guiIndex++){
		iqtgui::IGuiObject* guiPtr = m_slaveWidgetsCompPtr[guiIndex];
		if ((guiPtr == NULL) || !guiPtr->CreateGui(widgetPtr)){
			m_tabModel.SetOptionEnabled(guiIndex, false);

			continue;
		}

		QString tabName = m_tabNamesAttrPtr[guiIndex];
		m_menuListPtr->addItem(new QListWidgetItem(tabName));

		int tabIndex = m_stackedWidgetPtr->addWidget(guiPtr->GetWidget());
		m_tabToGuiIndexMap[tabIndex] = guiIndex;

		m_stackedWidgetPtr->widget(tabIndex)->layout()->setContentsMargins(9,9,9,9);

		if (		(guiIndex < m_slaveWidgetsVisualCompPtr.GetCount()) &&
			(guiIndex < m_slaveWidgetsModelCompPtr.GetCount())){
				imod::IModel* modelPtr = m_slaveWidgetsModelCompPtr[guiIndex];
				if ((m_slaveWidgetsVisualCompPtr[guiIndex] != NULL) && (modelPtr != NULL)){
					RegisterModel(modelPtr, guiIndex);
				}
		}
	}

	connect(m_menuListPtr, SIGNAL(currentRowChanged(int)), m_stackedWidgetPtr, SLOT(setCurrentIndex(int)));

	int selectedIndex = m_tabModel.GetSelectedOptionIndex();
	m_menuListPtr->setCurrentRow(selectedIndex);

	UpdateVisualElements();
}


void CSettingsContainerGuiComp::OnGuiDestroyed()
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


// reimplemented (imod::CMultiModelDispatcherBase)

void CSettingsContainerGuiComp::OnModelChanged(int /*modelId*/, int /*changeFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (IsGuiCreated()){
		UpdateVisualElements();
	}
}

// reimplemented (icomp::CComponentBase)

void CSettingsContainerGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_tabModel.SetParent(this);
}


// public methods of the embedded class TabModel

CSettingsContainerGuiComp::TabModel::TabModel()
	:m_parentPtr(NULL)
{
	m_selectedOptionIndex = 0;

	BaseClass::SetSelectionConstraints(this);
}


void CSettingsContainerGuiComp::TabModel::SetParent(CSettingsContainerGuiComp* parentPtr)
{
	Q_ASSERT(parentPtr != NULL);

	m_parentPtr = parentPtr;

	if ((m_parentPtr != NULL) && m_parentPtr->m_tabNamesAttrPtr.IsValid()){
		int tabsCount = qMin(m_parentPtr->m_tabNamesAttrPtr.GetCount(), m_parentPtr->m_slaveWidgetsCompPtr.GetCount());

		for(int tabIndex = 0; tabIndex < tabsCount; tabIndex++){
			QString tabName = m_parentPtr->m_tabNamesAttrPtr[tabIndex];

			BaseClass2::InsertOption(tabName, tabName.toLocal8Bit());
		}
	}
}


// reimplemented (iprm::ISelectionParam)

bool CSettingsContainerGuiComp::TabModel::SetSelectedOptionIndex(int index)
{
	if (BaseClass::SetSelectedOptionIndex(index)){
		if (m_parentPtr != NULL && m_parentPtr->IsGuiCreated()){
			QStackedWidget* tabWidgetPtr = m_parentPtr->m_stackedWidgetPtr;
			Q_ASSERT(tabWidgetPtr != NULL);

			tabWidgetPtr->setCurrentIndex(index);
		}
	}

	return false;
}


// reimplemented (iser::ISerializable)

bool CSettingsContainerGuiComp::TabModel::Serialize(iser::IArchive& archive)
{
	return BaseClass::Serialize(archive) && BaseClass2::Serialize(archive);
}


// reimplemented (iprm::IOptionsManager)

bool CSettingsContainerGuiComp::TabModel::SetOptionEnabled(int index, bool isEnabled)
{
	if (		(m_parentPtr != NULL) &&
		m_parentPtr->IsGuiCreated() &&
		BaseClass2::SetOptionEnabled(index, isEnabled)){

			// TO DO

			return true;
	}

	return false;
}


bool CSettingsContainerGuiComp::TabModel::RemoveOption(int /*index*/)
{
	return false;
}


bool CSettingsContainerGuiComp::TabModel::InsertOption(
	QString& /*optionName*/,
	const QByteArray& /*optionId*/,
	const QString& /*optionDescription*/, 
	int /*index*/)
{
	return false;
}


} // namespace iqtgui
