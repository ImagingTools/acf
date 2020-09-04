#include <iqtgui/CMultiPageGuiCompBase.h>


// Qt includes
#include <QtCore/QtGlobal>
#include <QtCore/QVariant>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#else
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#endif

// ACF includes
#include <istd/CChangeGroup.h>


namespace iqtgui
{


// public methods

CMultiPageGuiCompBase::CMultiPageGuiCompBase()
{
}


iprm::ISelectionParam& CMultiPageGuiCompBase::GetPageModelRef()
{
	return m_pageModel;
}


bool CMultiPageGuiCompBase::EnsurePageInitialized(int pageIndex)
{
	Q_ASSERT(pageIndex >= 0);

	PageInfo& pageInfo = m_pageIndexToInfoMap[pageIndex];
	if (pageInfo.isCreated){
		return true;
	}

	iqtgui::IGuiObject* pageGuiPtr = GetPageGuiComponent(pageIndex);
	if ((pageGuiPtr == NULL) || (pageInfo.widgetIndex < 0) || (pageInfo.widgetPtr == NULL)){
		return false;
	}

	pageInfo.isCreated = pageGuiPtr->CreateGui(pageInfo.widgetPtr);

	return true;
}


// protected methods

int CMultiPageGuiCompBase::GetLogicalPageIndex(int physicalWidgetIndex) const
{
	if (physicalWidgetIndex >= 0){
		for (		PageIndexToInfoMap::ConstIterator pageInfoIter = m_pageIndexToInfoMap.constBegin();
					pageInfoIter != m_pageIndexToInfoMap.constEnd();
					++pageInfoIter){
			if (pageInfoIter->widgetIndex == physicalWidgetIndex){
				return pageInfoIter.key();
			}
		}
	}

	return -1;
}


bool CMultiPageGuiCompBase::CreatePage(int pageIndex)
{
	Q_ASSERT(pageIndex >= 0);

	iqtgui::IGuiObject* guiPtr = GetPageGuiComponent(pageIndex);
	if (guiPtr != NULL){
		PageInfo& pageInfo = m_pageIndexToInfoMap[pageIndex];

		if (pageInfo.widgetPtr == NULL){
			pageInfo.widgetPtr = new QWidget(GetWidget());
			QVBoxLayout* pageContainerLayoutPtr = new QVBoxLayout(pageInfo.widgetPtr);
			pageContainerLayoutPtr->setMargin(0);
		}

		iwidgets::CMultiPageWidget* multiPageWidgetPtr = dynamic_cast<iwidgets::CMultiPageWidget*>(GetWidget());
		Q_ASSERT(multiPageWidgetPtr != NULL);

		QString pageTitle = *m_supressPageTitleAttrPtr ? QString() : pageInfo.pageTitle;

		pageInfo.widgetIndex = multiPageWidgetPtr->InsertPage(pageInfo.widgetPtr, pageTitle);

		if (		(pageIndex < m_slaveWidgetsVisualCompPtr.GetCount()) &&
					(pageIndex < m_slaveWidgetsModelCompPtr.GetCount())){
			imod::IModel* modelPtr = m_slaveWidgetsModelCompPtr[pageIndex];
			if ((m_slaveWidgetsVisualCompPtr[pageIndex] != NULL) && (modelPtr != NULL)){
				RegisterModel(modelPtr, pageIndex);
			}
		}

		return true;
	}

	return false;
}


void CMultiPageGuiCompBase::RemovePage(int pageIndex)
{
	if (m_pageIndexToInfoMap.contains(pageIndex)){
		UnregisterModel(pageIndex);

		iwidgets::CMultiPageWidget* multiPageWidgetPtr = dynamic_cast<iwidgets::CMultiPageWidget*>(GetWidget());
		Q_ASSERT(multiPageWidgetPtr != NULL);

		PageInfo& pageInfo = m_pageIndexToInfoMap[pageIndex];

		if (pageInfo.isCreated){
			iqtgui::IGuiObject* guiPtr = GetPageGuiComponent(pageIndex);
			Q_ASSERT(guiPtr != NULL);

			guiPtr->DestroyGui();

			pageInfo.isCreated = false;
		}

		if (pageInfo.widgetIndex >= 0){
			multiPageWidgetPtr->RemovePage(pageInfo.widgetIndex);
			pageInfo.widgetIndex = -1;
			pageInfo.widgetPtr = NULL;
		}
	}
}


void CMultiPageGuiCompBase::SetCurrentPage(int pageIndex)
{
	if (m_pageIndexToInfoMap.contains(pageIndex)){
		iwidgets::CMultiPageWidget* multiPageWidgetPtr = dynamic_cast<iwidgets::CMultiPageWidget*>(GetWidget());
		Q_ASSERT(multiPageWidgetPtr != NULL);

		const PageInfo& pageInfo = m_pageIndexToInfoMap[pageIndex];

		if (pageInfo.widgetIndex >= 0){
			multiPageWidgetPtr->SetCurrentPage(pageInfo.widgetIndex);
		}
	}
}


void CMultiPageGuiCompBase::UpdateVisualElements()
{
	iwidgets::CMultiPageWidget* multiPageWidgetPtr = dynamic_cast<iwidgets::CMultiPageWidget*>(GetWidget());
	Q_ASSERT(multiPageWidgetPtr != NULL);

	int visualProvidersCount = m_slaveWidgetsVisualCompPtr.GetCount();

	for (		PageIndexToInfoMap::ConstIterator iter = m_pageIndexToInfoMap.constBegin();
				iter != m_pageIndexToInfoMap.constEnd();
				++iter){
		int pageIndex = iter.key();
		Q_ASSERT(pageIndex >= 0);
		Q_ASSERT(pageIndex < GetPagesCount());

		const PageInfo& pageInfo = iter.value();

		if (pageInfo.widgetIndex >= 0){
			QIcon pageIcon;
			QString pageToolTip;

			if (pageIndex < visualProvidersCount){
				const IVisualStatus* visualProviderPtr = m_slaveWidgetsVisualCompPtr[pageIndex];
				if (visualProviderPtr != NULL){
					pageIcon = visualProviderPtr->GetStatusIcon();
					pageToolTip = visualProviderPtr->GetStatusText();
				}
			}

			multiPageWidgetPtr->SetPageIcon(pageInfo.widgetIndex, pageIcon);
			multiPageWidgetPtr->SetPageToolTip(pageInfo.widgetIndex, pageToolTip);
		}
	}

	m_pageModel.UpdatePageState();
}


void CMultiPageGuiCompBase::CreatePages()
{
	int designType = GetDesignType();

	bool isLazyInit = *m_lazyPagesInitializationAttrPtr && iwidgets::CMultiPageWidget::IsPageIndexChangeSupported(designType);

	bool firstPageInitialized = false;

	int intialPageIndex = *m_defaultPageIndexAttrPtr;

	int pagesCount = GetPagesCount();
	for (int pageIndex = 0; pageIndex < pagesCount; pageIndex++){
		CreatePage(pageIndex);

		bool initPage = !isLazyInit;

		if (!firstPageInitialized && m_pageModel.IsOptionEnabled(pageIndex)){
			if (pageIndex >= intialPageIndex){
				firstPageInitialized = true;

				initPage = true;

				intialPageIndex = pageIndex;
			}
		}

		if (initPage){
			EnsurePageInitialized(pageIndex);
		}
	}

	m_pageModel.SetSelectedOptionIndex(intialPageIndex);

	UpdateVisualElements();
}


void CMultiPageGuiCompBase::ResetPages()
{
	UnregisterAllModels();

	m_pageModel.SetSelectedOptionIndex(iprm::ISelectionParam::NO_SELECTION);

	for (		PageIndexToInfoMap::Iterator iter = m_pageIndexToInfoMap.begin();
				iter != m_pageIndexToInfoMap.end();
				++iter){
		int pageIndex = iter.key();
		Q_ASSERT(pageIndex >= 0);
		Q_ASSERT(pageIndex < GetPagesCount());

		PageInfo& pageInfo = iter.value();
		if (pageInfo.isCreated){
			iqtgui::IGuiObject* guiPtr = GetPageGuiComponent(pageIndex);
			Q_ASSERT(guiPtr != NULL);

			guiPtr->DestroyGui();

			pageInfo.isCreated = false;
			pageInfo.widgetIndex = -1;
			pageInfo.widgetPtr = NULL;
		}
	}

	iwidgets::CMultiPageWidget* multiPageWidgetPtr = dynamic_cast<iwidgets::CMultiPageWidget*>(GetWidget());
	Q_ASSERT(multiPageWidgetPtr != NULL);

	multiPageWidgetPtr->ResetPages();
}


// reimplemented (iqtgui::CGuiComponentBase)

QWidget* CMultiPageGuiCompBase::CreateQtWidget(QWidget* parentPtr)
{
	bool insertSpacer = m_insertSpacerAttrPtr.IsValid() ? *m_insertSpacerAttrPtr : false;

	int containerGuiFlags = insertSpacer ? iwidgets::IMultiPageWidgetDelegate::CGF_COMPACT : iwidgets::IMultiPageWidgetDelegate::CGF_NONE;

	if (m_useSameStretchingFactorAttrPtr.IsValid() && *m_useSameStretchingFactorAttrPtr){
		containerGuiFlags |= iwidgets::IMultiPageWidgetDelegate::CGF_UNIFORM_SIZES;
	}

	iwidgets::CMultiPageWidget* widgetPtr = new iwidgets::CMultiPageWidget(
				parentPtr,
				GetDesignType(),
				containerGuiFlags,
				*m_useHorizontalLayoutAttrPtr ? Qt::Horizontal : Qt::Vertical);

	if (m_iconSizeAttrPtr.IsValid()){
		widgetPtr->SetPageIconSize(QSize(*m_iconSizeAttrPtr, *m_iconSizeAttrPtr));
	}

	connect(widgetPtr, SIGNAL(EmitPageIndexChanged(int)), this, SLOT(OnPageChanged(int)));

	m_pageModel.SetSelectedOptionIndex(GetLogicalPageIndex(widgetPtr->GetCurrentPage()));

	return widgetPtr;
}


// reimplemented (iqtgui::CGuiComponentBase)

void CMultiPageGuiCompBase::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	CreatePages();
}


void CMultiPageGuiCompBase::OnGuiDestroyed()
{
	ResetPages();

	BaseClass::OnGuiDestroyed();
}


void CMultiPageGuiCompBase::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	// Modify pages with their translated names:
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

		m_pageIndexToInfoMap[pageIndex].pageTitle = name;
	}
}


void CMultiPageGuiCompBase::OnTryClose(bool* ignoredPtr)
{
	int pageCount = GetPagesCount();
	for (int i = 0; i < pageCount; i++){
		iqtgui::IGuiObject* guiObjectPtr = GetPageGuiComponent(i);
		if (guiObjectPtr != nullptr){
			guiObjectPtr->OnTryClose(ignoredPtr);

			if (ignoredPtr != nullptr && *ignoredPtr == true){
				SetCurrentPage(i);
				return;
			}
		}
	}
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

		m_pageIndexToInfoMap[pageIndex].pageTitle = name;
	}

	m_pageModel.SetParent(this);
}


void CMultiPageGuiCompBase::OnComponentDestroyed()
{
	m_pageIndexToInfoMap.clear();

	m_pageModel.SetParent(NULL);

	BaseClass::OnComponentDestroyed();
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CMultiPageGuiCompBase::OnModelChanged(int /*modelId*/, const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	if (IsGuiCreated()){
		UpdateVisualElements();
	}
}


// protected slots

void CMultiPageGuiCompBase::OnPageChanged(int widgetIndex)
{
	int pageIndex = GetLogicalPageIndex(widgetIndex);
	if (pageIndex >= 0){
		EnsurePageInitialized(pageIndex);
	}

	m_pageModel.SetSelectedOptionIndex(pageIndex);
}


// public methods of the embedded class PageModel

CMultiPageGuiCompBase::PageModel::PageModel()
:	m_parentPtr(NULL),
	m_updateBridge(this)
{
}


void CMultiPageGuiCompBase::PageModel::SetParent(CMultiPageGuiCompBase* parentPtr)
{
	if (parentPtr != m_parentPtr){
		if (m_parentPtr != NULL){
			m_updateBridge.EnsureModelsDetached();
		}

		if (parentPtr != NULL){
			int activatorsCount = qMin(parentPtr->m_pageActivatorsModelCompPtr.GetCount(), parentPtr->GetPagesCount());
			for (int pageIndex = 0; pageIndex < activatorsCount; ++pageIndex){
				imod::IModel* modelPtr = parentPtr->m_pageActivatorsModelCompPtr[pageIndex];

				if ((modelPtr != NULL) && !modelPtr->IsAttached(&m_updateBridge)){
					modelPtr->AttachObserver(&m_updateBridge);
				}
			}


			int visActivatorsCount = qMin(parentPtr->m_pageVisibilityActivatorsModelCompPtr.GetCount(), parentPtr->GetPagesCount());
			for (int pageIndex = 0; pageIndex < visActivatorsCount; ++pageIndex){
				imod::IModel* modelPtr = parentPtr->m_pageVisibilityActivatorsModelCompPtr[pageIndex];

				if ((modelPtr != NULL) && !modelPtr->IsAttached(&m_updateBridge)){
					modelPtr->AttachObserver(&m_updateBridge);
				}
			}
		}

		m_parentPtr = parentPtr;

		if (parentPtr != NULL){
			BaseClass::SetSelectedOptionIndex(*m_parentPtr->m_defaultPageIndexAttrPtr);

			BaseClass::SetSelectionConstraints(this);
		}
	}
}


void CMultiPageGuiCompBase::PageModel::UpdatePageState()
{
	Q_ASSERT(m_parentPtr != NULL);

	iwidgets::CMultiPageWidget* multiPageWidgetPtr = dynamic_cast<iwidgets::CMultiPageWidget*>(m_parentPtr->GetWidget());
	Q_ASSERT(multiPageWidgetPtr != NULL);

	for (		PageIndexToInfoMap::ConstIterator iter = m_parentPtr->m_pageIndexToInfoMap.constBegin();
				iter != m_parentPtr->m_pageIndexToInfoMap.constEnd();
				++iter){
		int pageIndex = iter.key();
		Q_ASSERT(pageIndex >= 0);
		Q_ASSERT(pageIndex < m_parentPtr->GetPagesCount());

		const PageInfo& pageInfo = iter.value();
		if (pageInfo.widgetIndex >= 0){
			if (pageIndex < m_parentPtr->m_pageActivatorsCompPtr.GetCount()){
				const iprm::IEnableableParam* paramPtr = m_parentPtr->m_pageActivatorsCompPtr[pageIndex];
				if (paramPtr != NULL){
					multiPageWidgetPtr->SetPageEnabled(pageInfo.widgetIndex, paramPtr->IsEnabled());
				}
			}

			if (pageIndex < m_parentPtr->m_pageVisibilityActivatorsCompPtr.GetCount()){
				const iprm::IEnableableParam* paramPtr = m_parentPtr->m_pageVisibilityActivatorsCompPtr[pageIndex];
				if (paramPtr != NULL){
					multiPageWidgetPtr->SetPageVisible(pageInfo.widgetIndex, paramPtr->IsEnabled());
				}
			}
		}
	}
}


// reimplemented (iprm::ISelectionParam)

bool CMultiPageGuiCompBase::PageModel::SetSelectedOptionIndex(int index)
{
	Q_ASSERT(m_parentPtr != NULL);

	istd::CChangeGroup changeGroup(this);

	iwidgets::CMultiPageWidget* multiPageWidgetPtr = dynamic_cast<iwidgets::CMultiPageWidget*>(m_parentPtr->GetWidget());
	if ((multiPageWidgetPtr != NULL) && BaseClass::SetSelectedOptionIndex(index)){
		int widgetIndex = -1;
		if (index >= 0){
			const PageInfo& pageInfo = m_parentPtr->m_pageIndexToInfoMap[index];
			widgetIndex = pageInfo.widgetIndex;
		}

		if (widgetIndex >= 0){
			return multiPageWidgetPtr->SetCurrentPage(widgetIndex);
		}
	}

	return false;
}


iprm::ISelectionParam* CMultiPageGuiCompBase::PageModel::GetSubselection(int index) const
{
	CMultiPageGuiCompBase* pageGuiPtr = dynamic_cast<CMultiPageGuiCompBase*>(m_parentPtr->GetPageGuiComponent(index));
	if (pageGuiPtr != NULL){
		// Sub-selection page control is enabled for stack design only:
		int designType = pageGuiPtr->GetDesignType();
		if (designType == iwidgets::CMultiPageWidget::DT_STACK){
			return &pageGuiPtr->GetPageModelRef();
		}
	}

	return NULL;
}


// reimplemented (iprm::IOptionsList)

int CMultiPageGuiCompBase::PageModel::GetOptionsFlags() const
{
	return SFC_DISABLE_ALLOWED | SCF_SUPPORT_UNIQUE_ID;
}


int CMultiPageGuiCompBase::PageModel::GetOptionsCount() const
{
	Q_ASSERT(m_parentPtr != NULL);

	return m_parentPtr->GetPagesCount();
}


QString CMultiPageGuiCompBase::PageModel::GetOptionName(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);
	Q_ASSERT(index >= 0);

	if (index < m_parentPtr->m_pageNamesAttrPtr.GetCount()){
		return m_parentPtr->m_pageNamesAttrPtr[index];
	}

	return QString();
}


QString CMultiPageGuiCompBase::PageModel::GetOptionDescription(int /*index*/) const
{
	return QString();
}


QByteArray CMultiPageGuiCompBase::PageModel::GetOptionId(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);
	Q_ASSERT(index >= 0);

	if (index < m_parentPtr->m_pageIdsAttrPtr.GetCount()){
		return m_parentPtr->m_pageIdsAttrPtr[index];
	}

	return GetOptionName(index).toUtf8();
}


bool CMultiPageGuiCompBase::PageModel::IsOptionEnabled(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);
	Q_ASSERT(index >= 0);

	if (index >= m_parentPtr->GetPagesCount()){
		return false;
	}

	bool retVal = true;
	if (index < m_parentPtr->m_pageActivatorsCompPtr.GetCount()){
		const iprm::IEnableableParam* paramPtr = m_parentPtr->m_pageActivatorsCompPtr[index];
		if (paramPtr != NULL){
			retVal = paramPtr->IsEnabled();
		}
	}

	if (retVal && (index < m_parentPtr->m_pageVisibilityActivatorsCompPtr.GetCount())){
		const iprm::IEnableableParam* paramPtr = m_parentPtr->m_pageVisibilityActivatorsCompPtr[index];
		if (paramPtr != NULL){
			retVal = retVal && paramPtr->IsEnabled();
		}
	}

	return retVal;
}


// reimplemented (iqtgui::IMultiVisualStatusProvider)

int CMultiPageGuiCompBase::PageModel::GetStatusesCount() const
{
	Q_ASSERT(m_parentPtr != NULL);

	return m_parentPtr->m_slaveWidgetsVisualCompPtr.GetCount();
}


const IVisualStatus* CMultiPageGuiCompBase::PageModel::GetVisualStatus(int statusIndex) const
{
	Q_ASSERT(m_parentPtr != NULL);

	return m_parentPtr->m_slaveWidgetsVisualCompPtr[statusIndex];
}


// protected methods of the embedded class PageModel

// reimplemented (istd::IChangeable)

void CMultiPageGuiCompBase::PageModel::OnEndChanges(const ChangeSet& changeSet)
{
	Q_ASSERT(m_parentPtr != NULL);

	BaseClass::OnEndChanges(changeSet);

	if (m_parentPtr->IsGuiCreated()){
		UpdatePageState();
	}
}


} // namespace iqtgui


