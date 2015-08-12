#include "iqtgui/CMultiPageGuiCompBase.h"


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


namespace iqtgui
{


// public methods

CMultiPageGuiCompBase::CMultiPageGuiCompBase()
{
}


// protected methods

int CMultiPageGuiCompBase::AddPageToContainerWidget(
			iqtgui::IGuiObject& pageGui,
			const QString& pageTitle)
{
	iwidgets::CMultiPageWidget* multiPageWidgetPtr = dynamic_cast<iwidgets::CMultiPageWidget*>(GetWidget());
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
		m_pageModel.SetSelectedOptionIndex(multiPageWidgetPtr->GetCurrentPage());
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


void CMultiPageGuiCompBase::RemovePage(int pageIndex)
{
	if (m_pageToGuiIndexMap.contains(pageIndex)){
		int guiIndex = m_pageToGuiIndexMap[pageIndex];
		UnregisterModel(guiIndex);

		iwidgets::CMultiPageWidget* multiPageWidgetPtr = dynamic_cast<iwidgets::CMultiPageWidget*>(GetWidget());
		Q_ASSERT(multiPageWidgetPtr != NULL);

		multiPageWidgetPtr->RemovePage(pageIndex);
	}
}


void CMultiPageGuiCompBase::SetCurrentPage(int pageIndex)
{
	iwidgets::CMultiPageWidget* multiPageWidgetPtr = dynamic_cast<iwidgets::CMultiPageWidget*>(GetWidget());
	Q_ASSERT(multiPageWidgetPtr != NULL);

	multiPageWidgetPtr->SetCurrentPage(pageIndex);
}


void CMultiPageGuiCompBase::UpdateVisualElements()
{
	iwidgets::CMultiPageWidget* multiPageWidgetPtr = dynamic_cast<iwidgets::CMultiPageWidget*>(GetWidget());
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
			const IVisualStatus* visualProviderPtr = m_slaveWidgetsVisualCompPtr[guiIndex];
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


void CMultiPageGuiCompBase::CreatePages()
{
	int pagesCount = GetPagesCount();
	for (int pageIndex = 0; pageIndex < pagesCount; pageIndex++){
		CreatePage(pageIndex);
	}

	for (int pageIndex = 0; pageIndex < pagesCount; pageIndex++){
		if (m_pageModel.IsOptionEnabled(pageIndex)){
			m_pageModel.SetSelectedOptionIndex(pageIndex);
			break;
		}
	}

	UpdateVisualElements();
}


void CMultiPageGuiCompBase::ResetPages()
{
	m_pageToGuiIndexMap.clear();

	UnregisterAllModels();

	int pagesCount = GetPagesCount();
	for (int pageIndex = 0; pageIndex < pagesCount; pageIndex++){
		iqtgui::IGuiObject* guiPtr = GetPageGuiComponent(pageIndex);
		if (guiPtr != NULL){
			guiPtr->DestroyGui();
		}
	}

	iwidgets::CMultiPageWidget* multiPageWidgetPtr = dynamic_cast<iwidgets::CMultiPageWidget*>(GetWidget());
	Q_ASSERT(multiPageWidgetPtr != NULL);

	multiPageWidgetPtr->ResetPages();

	m_pageModel.SetSelectedOptionIndex(NO_SELECTION);
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

	m_pageModel.SetSelectedOptionIndex(widgetPtr->GetCurrentPage());

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

void CMultiPageGuiCompBase::OnModelChanged(int /*modelId*/, const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	if (IsGuiCreated()){
		UpdateVisualElements();
	}
}


// protected slots

void CMultiPageGuiCompBase::OnPageChanged(int pageIndex)
{
	m_pageModel.SetSelectedOptionIndex(pageIndex);
}


// public methods of the embedded class PageModel

CMultiPageGuiCompBase::PageModel::PageModel()
:	imod::CMultiModelBridgeBase(this),
	m_parentPtr(NULL)
{
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

				if ((modelPtr != NULL) && !modelPtr->IsAttached(this)){
					modelPtr->AttachObserver(this);
				}
			}
		}

		m_parentPtr = parentPtr;

		if (parentPtr != NULL){
			BaseClass::SetSelectedOptionIndex(0);

			BaseClass::SetSelectionConstraints(this);
		}
	}
}


void CMultiPageGuiCompBase::PageModel::UpdatePageState()
{
	Q_ASSERT(m_parentPtr != NULL);

	iwidgets::CMultiPageWidget* multiPageWidgetPtr = dynamic_cast<iwidgets::CMultiPageWidget*>(m_parentPtr->GetWidget());

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

	iwidgets::CMultiPageWidget* multiPageWidgetPtr = dynamic_cast<iwidgets::CMultiPageWidget*>(m_parentPtr->GetWidget());
	if ((multiPageWidgetPtr != NULL) && BaseClass::SetSelectedOptionIndex(index)){
		return multiPageWidgetPtr->SetCurrentPage(index);
	}

	return false;
}


// reimplemented (iprm::IOptionsList)

int CMultiPageGuiCompBase::PageModel::GetOptionsFlags() const
{
	return SFC_DISABLE_ALLOWED;
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
	return GetOptionName(index).toUtf8();
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


// reimplemented (iqtgui::IMultiVisualStatusProvider)

int CMultiPageGuiCompBase::PageModel::GetStatusesCount() const
{
	return m_parentPtr->m_slaveWidgetsVisualCompPtr.GetCount();
}


const IVisualStatus* CMultiPageGuiCompBase::PageModel::GetVisualStatus(int statusIndex) const
{
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


