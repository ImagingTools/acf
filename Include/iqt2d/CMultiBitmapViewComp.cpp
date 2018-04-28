#include <iqt2d/CMultiBitmapViewComp.h>


// Qt includes
#if QT_VERSION >= 0x050000
#include <QtWidgets/QGridLayout>
#else
#include <QtGui/QGridLayout>
#include <QtGui/QApplication>
#endif

// ACF includes
#include <iview/CViewport.h>


namespace iqt2d
{


// protected methods

QIcon CMultiBitmapViewComp::GetCategoryIcon(istd::IInformationProvider::InformationCategory category)
{
	static QIcon defaultIcon(":/Icons/StateUnknown");
	static QIcon infoIcon(":/Icons/StateOk");
	static QIcon warningIcon(":/Icons/StateWarning");
	static QIcon errorIcon(":/Icons/StateInvalid");

	switch (category){
	case istd::IInformationProvider::IC_INFO:
		return infoIcon;

	case istd::IInformationProvider::IC_WARNING:
		return warningIcon;

	case istd::IInformationProvider::IC_ERROR:
	case istd::IInformationProvider::IC_CRITICAL:
		return errorIcon;

	default:
		return defaultIcon;
	}
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CMultiBitmapViewComp::OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	// update general status if available
	if (modelId == GeneralStatusModelId){
		istd::IInformationProvider::InformationCategory generalResultCategory = m_generalInformationProviderCompPtr->GetInformationCategory();

		SetStatusIcon(GetCategoryIcon(generalResultCategory));

		return;
	}

	if (modelId < 0){
		return; 
	}

	// view index is equal to the modelId
	UpdateInspectionCategory(modelId);
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CMultiBitmapViewComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	m_activeViewIndex.SetSelectedOptionIndex(-1);

	EnsureViewsCreated();

	iimg::IMultiBitmapProvider* objectPtr = GetObservedObject();
	if (objectPtr == NULL){
		return;
	}

	int bitmapsCount = objectPtr->GetBitmapsCount();
	int viewsCount = m_views.GetCount();

	int usedViewsCount = qMin(viewsCount, bitmapsCount);

	for (int viewIndex = 0; viewIndex < usedViewsCount; viewIndex++){
		CSingleView* viewPtr = m_views.GetAt(viewIndex);

		viewPtr->UpdateImage(objectPtr->GetBitmap(viewIndex));
	}

	const iprm::IOptionsList* bitmapListPtr = objectPtr->GetBitmapListInfo();
	if (bitmapListPtr != NULL){
		Q_ASSERT(bitmapsCount == bitmapListPtr->GetOptionsCount());
		for (int i = 0; i < bitmapsCount; ++i){
			Q_ASSERT(i < viewsCount);

			QString title = bitmapListPtr->GetOptionName(i);

			CSingleView* viewPtr = m_views.GetAt(i);
			Q_ASSERT(viewPtr != NULL);

			viewPtr->setTitle(title);
		}
	}

	if (m_viewExtenderCompPtr.IsValid()){
		for (int index = 0; index < bitmapsCount; index++){
			CSingleView* viewPtr = m_views.GetAt(index);
			Q_ASSERT(viewPtr != NULL);

			m_activeViewIndex.SetSelectedOptionIndex(index);

			m_viewExtenderCompPtr->RemoveItemsFromScene(viewPtr);
			m_viewExtenderCompPtr->AddItemsToScene(viewPtr, iqt2d::IViewExtender::SF_DIRECT);
		}
	}
}


void CMultiBitmapViewComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	m_activeViewIndex.SetSelectionConstraints(GetObservedObject()->GetBitmapListInfo());
}


// reimplemented (iqtgui::CGuiComponentBase)

void CMultiBitmapViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();
}


// reimplemented (icomp::CComponentBase)

void CMultiBitmapViewComp::OnComponentDestroyed()
{
	UnregisterAllModels();

	while (!m_views.IsEmpty()){
		m_views.PopAt(0);
	}

	BaseClass::OnComponentDestroyed();
}


// private methods

void CMultiBitmapViewComp::EnsureViewsCreated()
{
	iimg::IMultiBitmapProvider* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);
	if (objectPtr == NULL){
		return;
	}

	int totalViewsCount = objectPtr->GetBitmapsCount();
	if (m_views.GetCount() == totalViewsCount){
		return;
	}

	// recreate everything
	UnregisterAllModels();

	ResetAllViews();

	ConnectModels();

	m_columnCount = m_horizontalViewsAttrPtr.IsValid() ? qMax(0, *m_horizontalViewsAttrPtr) : 0;
	m_rowCount = m_verticalViewsAttrPtr.IsValid() ? qMax(0, *m_verticalViewsAttrPtr) : 0;

	if (m_rowCount <= 0 && m_columnCount <= 0){
		m_columnCount = qMax(1, (int)sqrt((double)totalViewsCount));
		m_rowCount = totalViewsCount / m_columnCount;
		if (totalViewsCount % m_columnCount){
			m_rowCount++;
		}
	}
	else if (m_rowCount <= 0){
		m_rowCount = totalViewsCount / m_columnCount;
		if (totalViewsCount % m_columnCount){
			m_rowCount++;
		}
	}
	else if (m_columnCount <= 0){
		m_columnCount = totalViewsCount / m_rowCount;
		if (totalViewsCount % m_rowCount){
			m_columnCount++;
		}
	}

	QColor backgroundColor = m_viewBackgroundColorAttrPtr.IsValid() ? 
				QColor(QString(*m_viewBackgroundColorAttrPtr)) : 
				Qt::transparent;

	QWidget* widgetPtr = GetQtWidget();
	QLayout* existingLayoutPtr = widgetPtr->layout();
	if (existingLayoutPtr != NULL){
		delete existingLayoutPtr;
	}

	QGridLayout* layoutPtr = new QGridLayout(widgetPtr);
	layoutPtr->setContentsMargins(0, 0, 0, 0);
	widgetPtr->setLayout(layoutPtr);

	const iprm::IOptionsList* bitmapListPtr = objectPtr->GetBitmapListInfo();

	int viewIndex = 0;
	for (int row = 0; row < m_rowCount && viewIndex < totalViewsCount; row++){
		for (int col = 0; col < m_columnCount && viewIndex < totalViewsCount; col++){
			QString title = GetTitleByIndex(viewIndex);

			QByteArray bitmapId;
			if (bitmapListPtr != NULL){
				bitmapId = bitmapListPtr->GetOptionId(viewIndex);
			}

			CSingleView* viewPtr = CreateView(widgetPtr, viewIndex, title, bitmapId);
			layoutPtr->addWidget(viewPtr, row, col);
			m_views.PushBack(viewPtr);

			if (m_viewBackgroundColorAttrPtr.IsValid()){
				viewPtr->SetBackgroundColor(backgroundColor);
			}

			OnViewCreated(viewIndex, viewPtr);

			viewIndex++;
		}
	}
}


QString CMultiBitmapViewComp::GetTitleByIndex(int viewIndex) const
{
	return QString("View - %1").arg(viewIndex + 1);
}


void CMultiBitmapViewComp::UpdateInspectionCategory(int index)
{
	if (!IsGuiCreated() || index < 0 || index >= m_views.GetCount()){
		return;
	}

	istd::IInformationProvider::InformationCategory viewResultCategory = istd::IInformationProvider::IC_NONE;
/*
	if (m_informationProvidersCompPtr.GetCount() == 1){
		viewResultCategory = m_informationProvidersCompPtr[0]->GetInformationCategory();
	}
	else if (index < m_informationProvidersCompPtr.GetCount()){
		viewResultCategory = m_informationProvidersCompPtr[index]->GetInformationCategory();
	}
*/
	CSingleView* viewPtr = m_views.GetAt(index);
	viewPtr->SetInspectionResult(viewResultCategory);
}


void CMultiBitmapViewComp::ResetAllViews()
{
	m_columnCount = 0;
	m_rowCount = 0;
	m_views.Reset();
}


void CMultiBitmapViewComp::ConnectModels()
{
	if (m_generalInformationModelCompPtr.IsValid() && m_generalInformationProviderCompPtr.IsValid()){
		RegisterModel(m_generalInformationModelCompPtr.GetPtr(), GeneralStatusModelId);
	}
}


void CMultiBitmapViewComp::OnViewCreated(int /*index*/, CSingleView* viewPtr)
{
	viewPtr->Init(false, false, *m_verticalViewLayoutAttrPtr);
}


// protected methods

CMultiBitmapViewComp::CSingleView* CMultiBitmapViewComp::CreateView(QWidget* parentPtr, int id, const QString& title, const QByteArray& bitmapId)
{
	return new CSingleView(parentPtr, id, title, bitmapId);
}


// embedded class CView

CMultiBitmapViewComp::CSingleView::CSingleView(QWidget* parentPtr, int id, const QString& title, const QByteArray& bitmapId)
:	BaseClass(parentPtr),
	m_console(this),
	m_id(id),
	m_showStatusLabel(true),
	m_showStatusBackground(true),
	m_bitmapId(bitmapId)
{
	setTitle(title);

	m_backgroundShape.AssignToLayer(iview::IViewLayer::LT_BACKGROUND);
	m_backgroundModel.AttachObserver(&m_backgroundShape);

	m_console.GetViewRef().ConnectShape(&m_backgroundShape);

	m_console.SetFitMode(iview::CConsoleBase::FM_BOTH);
	m_console.SetZoomToFit(true);
}


void CMultiBitmapViewComp::CSingleView::SetBackgroundColor(const QColor& color)
{
	iview::CColorSchema* colorShemaPtr = new iview::CColorSchema;
	colorShemaPtr->Assign(m_console.GetViewRef().GetDefaultColorSchema());

	colorShemaPtr->SetBrush(iview::IColorSchema::SB_BACKGROUND, color);

	m_console.GetViewRef().SetDefaultColorSchema(colorShemaPtr, true);
}


void CMultiBitmapViewComp::CSingleView::UpdateImage(const iimg::IBitmap* bitmapPtr)
{
	if (bitmapPtr == NULL){
		m_backgroundModel.ResetImage();
	}
	else{
		m_backgroundModel.CopyFrom(*bitmapPtr, istd::IChangeable::CM_CONVERT);

		istd::CIndex2d imageSize = bitmapPtr->GetImageSize();

		m_console.GetViewRef().SetFitArea(i2d::CRectangle(0, 0, imageSize.GetX(), imageSize.GetY()));
	}
}


void CMultiBitmapViewComp::CSingleView::Init(bool hasStatusLabel, bool hasStatusBackground, bool useVerticalLayout)
{
	m_showStatusLabel = hasStatusLabel;
	m_showStatusBackground = hasStatusBackground;

	// enable title info
	if (hasStatusLabel && title().isEmpty()){
		setTitle(" "); // a trick, otherwise if QGroupBox has no title text, then it will not be shown.
	}

	// create default layout
	QBoxLayout* viewLayoutPtr = useVerticalLayout ? (QBoxLayout*)new QVBoxLayout : (QBoxLayout*)new QHBoxLayout;
	setLayout(viewLayoutPtr);

	// add console
	QWidget* holderPtr = new QWidget(this);
	holderPtr->setLayout(new QVBoxLayout());
	holderPtr->layout()->setContentsMargins(0,0,0,0);
	holderPtr->layout()->addWidget(&m_console);

	viewLayoutPtr->addWidget(holderPtr);

	// add status label(s)
	AddStatusItems(viewLayoutPtr);

	// default status
	SetInspectionResult(istd::IInformationProvider::IC_NONE);
}


void CMultiBitmapViewComp::CSingleView::SetInspectionResult(int result)
{
	if (m_showStatusBackground){
		switch (result){
			case istd::IInformationProvider::IC_INFO:
				SetBackgroundColor(Qt::darkGreen);
				break;

			case istd::IInformationProvider::IC_ERROR:
			case istd::IInformationProvider::IC_CRITICAL:
				SetBackgroundColor(Qt::darkRed);
				break;

			case istd::IInformationProvider::IC_WARNING:
				SetBackgroundColor(Qt::darkYellow);
				break;

			default:
				SetBackgroundColor(Qt::lightGray);
		}
	}


	if (m_showStatusLabel){
		static QString s_defaultStyleSheet = 
			"QGroupBox::title{"
			"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.5, y2:1, stop:0 #d5eefc, stop:0.5 #d5eefc, stop:0.51 #b8e3f9, stop:1 #a5dbf7);"
			"}";

		static QString s_okStyleSheet = 
			"QGroupBox::title{"
			"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
			"stop: 0.0 rgba(120,250,145), stop: 0.49 rgba(70,212,145), stop: 0.52 rgba(70,200,105), stop: 1.0 rgba(70,250,105)); "
			"border: 1px solid #339933; "
			"color: #000000; font-size: 10pt; font-weight: bold;"
			"}";

		static QString s_errorStyleSheet = 
			"QGroupBox::title{"
			"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
			"stop: 0.0 rgba(250,120,145), stop: 0.49 rgba(212,120,145), stop: 0.52 rgba(200,70,105), stop: 1.0 rgba(250,120,145)); "
			"border: 1px solid #993333; "
			"color: #ffffff; font-size: 10pt; font-weight: bold;"
			"}";

		static QString s_warningStyleSheet = 
			"QGroupBox::title{"
			"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
			"stop: 0.0 rgba(250,250,145), stop: 0.49 rgba(212,212,145), stop: 0.52 rgba(200,200,105), stop: 1.0 rgba(250,250,105)); "
			"border: 1px solid #996633; "
			"color: #000000; font-size: 10pt; font-weight: bold;"
			"}";

		switch (result){
			case istd::IInformationProvider::IC_INFO:
				setStyleSheet(s_okStyleSheet);
				break;

			case istd::IInformationProvider::IC_ERROR:
			case istd::IInformationProvider::IC_CRITICAL:
				setStyleSheet(s_errorStyleSheet);
				break;

			case istd::IInformationProvider::IC_WARNING:
				setStyleSheet(s_warningStyleSheet);
				break;

			default:
				setStyleSheet(s_defaultStyleSheet);
		}
	}
}


// reimplemented (iqt2d::IViewProvider)

int CMultiBitmapViewComp::CSingleView::GetViewId() const
{
	return m_id;
}


iview::IShapeView* CMultiBitmapViewComp::CSingleView::GetView() const
{
	return &(m_console.GetViewRef());
}


// protected members

void CMultiBitmapViewComp::CSingleView::AddStatusItems(QBoxLayout* /*layoutPtr*/)
{
	// do nothing here, it is only for extension
}


} // namespace iqt2d


