#include "iqtgui/CMultiPageGuiCompBase.h"


// Qt includes
#include <QtCore/QtGlobal>
#include <QtCore/QVariant>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QVBoxLayout>
#else
#include <QtGui/QVBoxLayout>
#endif


namespace iqtgui
{


// public methods

CMultiPageGuiCompBase::CMultiPageGuiCompBase()
	:m_currentPageIndex(-1)
{
}


// protected methods

int CMultiPageGuiCompBase::AddPageToContainerWidget(
			iqtgui::IGuiObject& pageGui,
			const QString& pageTitle)
{
	CMultiPageWidget* multiPageWidgetPtr = dynamic_cast<CMultiPageWidget*>(GetWidget());
	Q_ASSERT(multiPageWidgetPtr != NULL);
	QWidget* pageContainerPtr = new QWidget(multiPageWidgetPtr);
	QVBoxLayout* pageContainerLayoutPtr = new QVBoxLayout(pageContainerPtr);
	pageContainerLayoutPtr->setMargin(0);

	if (pageGui.GetWidget() != NULL){
		pageContainerLayoutPtr->addWidget(pageGui.GetWidget());
	}
	else{
		pageGui.CreateGui(pageContainerPtr);
	}

	int retVal = multiPageWidgetPtr->InsertPage(pageContainerPtr, pageTitle);
	if (retVal >= 0){
		m_currentPageIndex = multiPageWidgetPtr->GetCurrentPage();
	}

	return retVal;
}


QString CMultiPageGuiCompBase::GetPageGuiName(const iqtgui::IGuiObject& pageGui) const
{
	if (m_guiNamesMap.contains(&pageGui)){
		return m_guiNamesMap[&pageGui];
	}

	return QString();
}


bool CMultiPageGuiCompBase::CreatePage(int guiIndex)
{
	iqtgui::IGuiObject* guiPtr = GetPageGuiComponent(guiIndex);
	if (guiPtr != NULL){
		QString pageTitle = GetPageGuiName(*guiPtr);

		int pageIndex = AddPageToContainerWidget(*guiPtr, pageTitle);
		if (pageIndex < 0){
			pageIndex = guiIndex;
		}

		m_pageToGuiIndexMap[pageIndex] = guiIndex;

		if (		(guiIndex < m_slaveWidgetsVisualCompPtr.GetCount()) &&
					(guiIndex < m_slaveWidgetsModelCompPtr.GetCount())){
			imod::IModel* modelPtr = m_slaveWidgetsModelCompPtr[guiIndex];
			if ((m_slaveWidgetsVisualCompPtr[guiIndex] != NULL) && (modelPtr != NULL)){
				RegisterModel(modelPtr, guiIndex);
			}
		}

		return true;
	}

	return false;
}


void CMultiPageGuiCompBase::UpdateVisualElements()
{
	CMultiPageWidget* multiPageWidgetPtr = dynamic_cast<CMultiPageWidget*>(GetWidget());
	Q_ASSERT(multiPageWidgetPtr != NULL);

	int visualProvidersCount = m_slaveWidgetsVisualCompPtr.GetCount();

	for (		PageToGuiIndexMap::ConstIterator iter = m_pageToGuiIndexMap.constBegin();
				iter != m_pageToGuiIndexMap.constEnd();
				++iter){
		int pageIndex = iter.key();
		Q_ASSERT(pageIndex >= 0);
		Q_ASSERT(pageIndex < GetPagesCount());

		int guiIndex = iter.value();
		Q_ASSERT(guiIndex >= 0);
		Q_ASSERT(guiIndex < GetPagesCount());

		QIcon pageIcon;
		QString pageToolTip;

		if (guiIndex < visualProvidersCount){
			const IVisualStatusProvider* visualProviderPtr = m_slaveWidgetsVisualCompPtr[guiIndex];
			if (visualProviderPtr != NULL){
				pageIcon = visualProviderPtr->GetStatusIcon();
				pageToolTip = visualProviderPtr->GetStatusText();
			}
		}

		multiPageWidgetPtr->SetPageIcon(pageIndex, pageIcon);
		multiPageWidgetPtr->SetPageToolTip(pageIndex, pageToolTip);
	}

	m_pageModel.UpdatePageState();
}


// reimplemented (iqtgui::CGuiComponentBase)

QWidget* CMultiPageGuiCompBase::InitWidgetToParent(QWidget* parentPtr)
{
	CMultiPageWidget* widgetPtr = new CMultiPageWidget(
				parentPtr,
				GetDesignType(),
				*m_useHorizontalLayoutAttrPtr ? Qt::Horizontal : Qt::Vertical);

	if (m_iconSizeAttrPtr.IsValid()){
		widgetPtr->SetPageIconSize(QSize(*m_iconSizeAttrPtr, *m_iconSizeAttrPtr));
	}

	m_currentPageIndex = widgetPtr->GetCurrentPage();

	return widgetPtr;
}


// reimplemented (iqtgui::CGuiComponentBase)

void CMultiPageGuiCompBase::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	int pagesCount = GetPagesCount();
	for (int pageIndex = 0; pageIndex < pagesCount; pageIndex++){
		CreatePage(pageIndex);
	}

	UpdateVisualElements();
}


void CMultiPageGuiCompBase::OnGuiDestroyed()
{
	m_pageToGuiIndexMap.clear();

	int pagesCount = GetPagesCount();
	for (int pageIndex = 0; pageIndex < pagesCount; pageIndex++){
		iqtgui::IGuiObject* guiPtr = GetPageGuiComponent(pageIndex);
		if (guiPtr != NULL){
			guiPtr->DestroyGui();
		}
	}

	CMultiPageWidget* multiPageWidgetPtr = dynamic_cast<CMultiPageWidget*>(GetWidget());
	Q_ASSERT(multiPageWidgetPtr != NULL);

	multiPageWidgetPtr->ResetPages();

	BaseClass::OnGuiDestroyed();
}


// reimplemented (icomp::CComponentBase)

void CMultiPageGuiCompBase::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	// Map page GUIs with their names:
	int pagesCount = GetPagesCount();
	for (int pageIndex = 0; pageIndex < pagesCount; ++pageIndex){
		iqtgui::IGuiObject* guiPtr = GetPageGuiComponent(pageIndex);
		if (guiPtr == NULL){
			continue;
		}

		QString name;
		if (pageIndex < m_pageNamesAttrPtr.GetCount()){
			name = m_pageNamesAttrPtr[pageIndex];
		}
		else{
			continue;
		}

		m_guiNamesMap[guiPtr] = name;
	}

	m_pageModel.SetParent(this);
}


void CMultiPageGuiCompBase::OnComponentDestroyed()
{
	m_guiNamesMap.clear();

	m_pageModel.SetParent(NULL);

	BaseClass::OnComponentDestroyed();
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CMultiPageGuiCompBase::OnModelChanged(int /*modelId*/, int /*changeFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (IsGuiCreated()){
		UpdateVisualElements();
	}
}


// public methods of the embedded class PageModel

CMultiPageGuiCompBase::PageModel::PageModel()
:	m_parentPtr(NULL)
{
	m_selectedOptionIndex = 0;

	BaseClass::SetSelectionConstraints(this);
}


void CMultiPageGuiCompBase::PageModel::SetParent(CMultiPageGuiCompBase* parentPtr)
{
	if (parentPtr != m_parentPtr){
		if (m_parentPtr != NULL){
			EnsureModelsDetached();
		}

		if (parentPtr != NULL){
			int activatorsCount = qMin(parentPtr->m_pageNamesAttrPtr.GetCount(), parentPtr->m_pageActivatorsModelCompPtr.GetCount());
			for (int pageIndex = 0; pageIndex < activatorsCount; ++pageIndex){
				imod::IModel* modelPtr = parentPtr->m_pageActivatorsModelCompPtr[pageIndex];

				if (!modelPtr->IsAttached(this)){
					modelPtr->AttachObserver(this);
				}
			}
		}

		m_parentPtr = parentPtr;
	}
}


void CMultiPageGuiCompBase::PageModel::UpdatePageState()
{
	Q_ASSERT(m_parentPtr != NULL);

	CMultiPageWidget* multiPageWidgetPtr = dynamic_cast<CMultiPageWidget*>(m_parentPtr->GetWidget());

	int activatorsCount = qMin(m_parentPtr->m_pageActivatorsCompPtr.GetCount(), m_parentPtr->GetPagesCount());
	for (int i = 0; i < activatorsCount; ++i){
		const iprm::IEnableableParam* paramPtr = m_parentPtr->m_pageActivatorsCompPtr[i];
		if ((paramPtr != NULL) && (multiPageWidgetPtr != NULL)){
			multiPageWidgetPtr->SetPageEnabled(i, paramPtr->IsEnabled());
		}
	}
}


// reimplemented (iprm::ISelectionParam)


bool CMultiPageGuiCompBase::PageModel::SetSelectedOptionIndex(int index)
{
	Q_ASSERT(m_parentPtr != NULL);

	CMultiPageWidget* multiPageWidgetPtr = dynamic_cast<CMultiPageWidget*>(m_parentPtr->GetWidget());
	if ((multiPageWidgetPtr != NULL) && BaseClass::SetSelectedOptionIndex(index)){
		return multiPageWidgetPtr->SetCurrentPage(index);
	}

	return false;
}


// reimplemented (iprm::IOptionsList)

int CMultiPageGuiCompBase::PageModel::GetOptionsFlags() const
{
	return SCF_SUPPORT_DISABLED | SCF_SUPPORT_UNIQUE_ID;
}


int CMultiPageGuiCompBase::PageModel::GetOptionsCount() const
{
	Q_ASSERT(m_parentPtr != NULL);

	return m_parentPtr->m_pageNamesAttrPtr.GetCount();
}


QString CMultiPageGuiCompBase::PageModel::GetOptionName(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);
	Q_ASSERT(index < m_parentPtr->m_pageNamesAttrPtr.GetCount());

	return m_parentPtr->m_pageNamesAttrPtr[index];
}


QString CMultiPageGuiCompBase::PageModel::GetOptionDescription(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);
	Q_ASSERT(index >= 0);

	CMultiPageWidget* multiPageWidgetPtr = dynamic_cast<CMultiPageWidget*>(m_parentPtr->GetWidget());
	if (multiPageWidgetPtr != NULL){
		return multiPageWidgetPtr->GetPageTitle(index);
	}

	return QString();
}


QByteArray CMultiPageGuiCompBase::PageModel::GetOptionId(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);
	Q_ASSERT(index < m_parentPtr->m_pageNamesAttrPtr.GetCount());

	return m_parentPtr->m_pageNamesAttrPtr[index].toUtf8();
}


bool CMultiPageGuiCompBase::PageModel::IsOptionEnabled(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);
	Q_ASSERT(index >= 0);

	if (index >= m_parentPtr->GetPagesCount()){
		return false;
	}

	if (index < m_parentPtr->m_pageActivatorsCompPtr.GetCount()){
		const iprm::IEnableableParam* paramPtr = m_parentPtr->m_pageActivatorsCompPtr[index];
		if (paramPtr != NULL){
			return paramPtr->IsEnabled();
		}
	}

	return true;
}


// protected methods of the embedded class PageModel

// reimplemented (istd::IChangeable)

void CMultiPageGuiCompBase::PageModel::OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	Q_ASSERT(m_parentPtr != NULL);

	BaseClass::OnEndChanges(changeFlags, changeParamsPtr);

	if (m_parentPtr->IsGuiCreated()){
		UpdatePageState();
	}
}


} // namespace iqtgui


