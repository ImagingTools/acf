#include "iqtwiz/CWizardGuiComp.h"


// Qt includes
#include <QVBoxLayout>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iprm/IParamsSet.h"
#include "iprm/ISelectionConstraints.h"


namespace iqtwiz
{


// protected slots

void CWizardGuiComp::OnCurrentPageChanged(int pageId)
{
	if (IsUpdateBlocked()){
		return;
	}

	UpdateBlocker updateBlocker(this);

	iwiz::IParamsManagerWizard* objectPtr = GetObjectPtr();
	if ((objectPtr != NULL) && (pageId >= 0)){
		objectPtr->SetSelectedOptionIndex(pageId);
	}
}


void CWizardGuiComp::OnHelpRequested()
{
	if (m_helpViewerCompPtr.IsValid()){
		m_helpViewerCompPtr->ShowHelp(istd::CString(), this);
	}
}


// protected methods

int CWizardGuiComp::GetNextPageId() const
{
	iwiz::IParamsManagerWizard* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		return objectPtr->GetNextPageIndex();
	}

	return -1;
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CWizardGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iwiz::IParamsManagerWizard* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int paramsCount = objectPtr->GetParamsSetsCount();
		int connectionsCount = istd::Min(m_observersCompPtr.GetCount(), paramsCount);

		for (int connectionIndex = 0; connectionIndex < connectionsCount; connectionIndex++){
			imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(objectPtr->GetParamsSet(connectionIndex));
			if (paramsModelPtr != NULL && m_observersCompPtr.IsValid()){
				imod::IObserver* paramsObserverPtr = m_observersCompPtr[connectionIndex];
				if (paramsObserverPtr != NULL){
					paramsModelPtr->AttachObserver(paramsObserverPtr);
				}
			}
		}
		QWizard* wizardPtr = GetQtWidget();
		I_ASSERT(wizardPtr != NULL);

		UpdateBlocker updateBlocker(this);
		
		int pageIndex = objectPtr->GetSelectedOptionIndex();
		if (pageIndex < 0){
			pageIndex = 0;

			objectPtr->SetSelectedOptionIndex(0);
		}

		wizardPtr->restart();

		GoToPage(pageIndex);
		wizardPtr->setStartId(pageIndex);
	}
}


void CWizardGuiComp::OnGuiModelDetached()
{
	iwiz::IParamsManagerWizard* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int paramsCount = objectPtr->GetParamsSetsCount();
		int connectionsCount = istd::Min(m_observersCompPtr.GetCount(), paramsCount);

		for (int connectionIndex = 0; connectionIndex < connectionsCount; connectionIndex++){
			imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(objectPtr->GetParamsSet(connectionIndex));
			if (paramsModelPtr != NULL && m_observersCompPtr.IsValid()){
				imod::IObserver* paramsObserverPtr = m_observersCompPtr[connectionIndex];
				if (paramsObserverPtr != NULL && paramsModelPtr->IsAttached(paramsObserverPtr)){
					paramsModelPtr->DetachObserver(paramsObserverPtr);
				}
			}
		}
	}

	BaseClass::OnGuiModelDetached();
}


void CWizardGuiComp::UpdateGui(int updateFlags)
{
	I_ASSERT(IsGuiCreated());

	QWizard* wizardPtr = GetQtWidget();
	I_ASSERT(wizardPtr != NULL);

	wizardPtr->setStartId(0);

	iwiz::IParamsManagerWizard* objectPtr = GetObjectPtr();
	if ((objectPtr != NULL) && ((updateFlags & iprm::ISelectionParam::CF_SELECTION_CHANGED) != 0)){
		int pageIndex = objectPtr->GetSelectedOptionIndex();
		if (pageIndex < 0){
			pageIndex = 0;
		}

		GoToPage(pageIndex);
	}
}


// reimplemented (iqtgui::CComponentBase)

void CWizardGuiComp::OnGuiCreated()
{
	QWizard* wizardPtr = GetQtWidget();
	I_ASSERT(wizardPtr != NULL);

	QWizard::WizardStyle wizardStyle = QWizard::ClassicStyle;

	if (m_wizardStyleAttrPtr.IsValid()){
		switch (*m_wizardStyleAttrPtr){
		case 0:
			wizardStyle = QWizard::ClassicStyle;
			break;

		case 1:
			wizardStyle = QWizard::ModernStyle;
			break;

		case 2:
			wizardStyle = QWizard::MacStyle;
			break;

		case 3:
			wizardStyle = QWizard::AeroStyle;
			break;
		}
	}

	wizardPtr->setWizardStyle(wizardStyle);

	QWizard::WizardOptions options;
	
	if (m_helpViewerCompPtr.IsValid()){
		options |= QWizard::HaveHelpButton;
	}

	wizardPtr->setOptions(options);

	int pagesCount = m_guisCompPtr.GetCount();
	for (int pageIndex = 0; pageIndex < pagesCount; pageIndex++){
		istd::TDelPtr<QWizardPage> wizardPage(new CWizardPage(*this));
		QVBoxLayout* layoutPtr = new QVBoxLayout(wizardPage.GetPtr());
		layoutPtr->setMargin(0);

		QString pageTitel;
		if (m_titelsAttrPtr.IsValid() && pageIndex < m_titelsAttrPtr.GetCount()){
			pageTitel = iqt::GetQString(m_titelsAttrPtr[pageIndex]);
		}

		QString pageSubTitel;
		if (m_titelsAttrPtr.IsValid() && pageIndex < m_subTitelsAttrPtr.GetCount()){
			pageSubTitel = iqt::GetQString(m_subTitelsAttrPtr[pageIndex]);
		}

		wizardPage->setTitle(pageTitel);
		wizardPage->setSubTitle(pageSubTitel);

		iqtgui::IGuiObject* pageGuiObjectPtr = m_guisCompPtr[pageIndex];
		if (pageGuiObjectPtr != NULL){
			if (pageGuiObjectPtr->CreateGui(wizardPage.GetPtr())){
				wizardPtr->setPage(pageIndex, wizardPage.PopPtr());
			}
		}
	}

#if QT_VERSION > 0x040700
	if (m_sideGuiCompPtr.IsValid()){
		QWidget* sideWidgetContainer = new QWidget(wizardPtr);
		sideWidgetContainer->setMinimumWidth(250);
		QVBoxLayout* layoutPtr = new QVBoxLayout(sideWidgetContainer);
		layoutPtr->setMargin(0);

		if (m_sideGuiCompPtr->CreateGui(sideWidgetContainer)){
			wizardPtr->setSideWidget(sideWidgetContainer);
		}
	}
#endif

	connect(wizardPtr, SIGNAL(currentIdChanged(int)), this, SLOT(OnCurrentPageChanged(int)));
	connect(wizardPtr, SIGNAL(helpRequested()), this, SLOT(OnHelpRequested()));

	BaseClass::OnGuiCreated();
}


void CWizardGuiComp::OnGuiDestroyed()
{
	if (m_sideGuiCompPtr.IsValid()){
		m_sideGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


// private methods

void CWizardGuiComp::GoToPage(int pageIndex)
{
	QWizard* wizardPtr = GetQtWidget();
	I_ASSERT(wizardPtr != NULL);

	while (wizardPtr->currentId() > pageIndex){
		wizardPtr->back();
	}

	if (wizardPtr->currentId() >= 0){
		while (wizardPtr->currentId() < pageIndex){
			wizardPtr->next();
		}
	}
}


CWizardGuiComp::CWizardPage::CWizardPage(CWizardGuiComp& parent)
:	BaseClass(parent.GetQtWidget()),
	m_parent(parent)
{
}


// reimplemented (QWizardPage)

int CWizardGuiComp::CWizardPage::nextId() const
{
	return m_parent.GetNextPageId();
}


} // namespace iqtwiz


