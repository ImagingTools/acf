#include "iqtgui/CContainerGuiBase.h"


namespace iqtgui
{


// protected methods

void CContainerGuiBase::UpdateVisualElements()
{
	int visualProvidersCount = m_slaveWidgetsVisualCompPtr.GetCount();

	for (		TabToGuiIndexMap::ConstIterator iter = m_tabToGuiIndexMap.begin();
		iter != m_tabToGuiIndexMap.end();
		++iter){
			int tabIndex = iter.key();
			Q_ASSERT(tabIndex >= 0);
			Q_ASSERT(tabIndex < m_tabToGuiIndexMap.count());

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

			UpdateItem(tabIndex, tabIcon, toolTip);
	}
}


// reimplemented (iqtgui::CComponentBase)

void CContainerGuiBase::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	Q_ASSERT(IsGuiCreated());

	QSize iconSize(16, 16);
	if (m_iconSizeAttrPtr.IsValid()){
		QSize newIconSize = QSize(m_iconSizeAttrPtr->GetValue(), m_iconSizeAttrPtr->GetValue());
		if (!newIconSize.isNull() && newIconSize.isValid() && !newIconSize.isEmpty()){
			iconSize = newIconSize;
		}
	}

	if (!CreateMainGui(iconSize)){
		Q_ASSERT(false);

		return;
	}

	int tabsCount = m_tabModel.GetOptionsCount();
	for (int guiIndex = 0; guiIndex < tabsCount; guiIndex++){
		iqtgui::IGuiObject* guiPtr = m_slaveWidgetsCompPtr[guiIndex];

		QString tabName = m_tabNamesAttrPtr[guiIndex];

		int tabIndex = CreatePageGui(tabName, iconSize, guiPtr);

		if (tabIndex < 0){
			m_tabModel.SetOptionEnabled(guiIndex, false);

			continue;
		}

		m_tabToGuiIndexMap[tabIndex] = guiIndex;

		if (		(guiIndex < m_slaveWidgetsVisualCompPtr.GetCount()) &&
			(guiIndex < m_slaveWidgetsModelCompPtr.GetCount())){
				imod::IModel* modelPtr = m_slaveWidgetsModelCompPtr[guiIndex];
				if ((m_slaveWidgetsVisualCompPtr[guiIndex] != NULL) && (modelPtr != NULL)){
					RegisterModel(modelPtr, guiIndex);
				}
		}
	}

	UpdateVisualElements();

	int selectedIndex = m_tabModel.GetSelectedOptionIndex();
	SetSelectedIndex(selectedIndex);
}


void CContainerGuiBase::OnGuiDestroyed()
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

void CContainerGuiBase::OnModelChanged(int /*modelId*/, int /*changeFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (IsGuiCreated()){
		UpdateVisualElements();
	}
}

// reimplemented (icomp::CComponentBase)

void CContainerGuiBase::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_tabModel.SetParent(this);
}


// public methods of the embedded class TabModel

CContainerGuiBase::TabModel::TabModel()
:m_parentPtr(NULL)
{
	m_selectedOptionIndex = 0;

	BaseClass::SetSelectionConstraints(this);
}


void CContainerGuiBase::TabModel::SetParent(CContainerGuiBase* parentPtr)
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

bool CContainerGuiBase::TabModel::SetSelectedOptionIndex(int index)
{
	if (BaseClass::SetSelectedOptionIndex(index)){
		if (m_parentPtr != NULL && m_parentPtr->IsGuiCreated()){
			return m_parentPtr->SetSelectedIndex(index);
		}
	}

	return false;
}


// reimplemented (iser::ISerializable)

bool CContainerGuiBase::TabModel::Serialize(iser::IArchive& archive)
{
	return BaseClass::Serialize(archive) && BaseClass2::Serialize(archive);
}


// reimplemented (iprm::IOptionsManager)

bool CContainerGuiBase::TabModel::SetOptionEnabled(int index, bool isEnabled)
{
	if (		(m_parentPtr != NULL) &&
		m_parentPtr->IsGuiCreated() &&
		BaseClass2::SetOptionEnabled(index, isEnabled)){

		return m_parentPtr->SetEnabled(index, isEnabled);
	}

	return false;
}


bool CContainerGuiBase::TabModel::RemoveOption(int /*index*/)
{
	return false;
}


bool CContainerGuiBase::TabModel::InsertOption(
	QString& /*optionName*/,
	const QByteArray& /*optionId*/,
	const QString& /*optionDescription*/, 
	int /*index*/)
{
	return false;
}


} // namespace iqtgui
