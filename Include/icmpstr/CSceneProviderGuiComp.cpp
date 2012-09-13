#include "icmpstr/CSceneProviderGuiComp.h"


// Qt includes
#include <QtCore/qmath.h>
#include <QtCore/QVarLengthArray>
#include <QtGui/QKeyEvent>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include <QtGui/QGraphicsItem>

// ACF includes
#include "iqtdoc/CMainWindowGuiComp.h"

#include "iqt/CSignalBlocker.h"


namespace icmpstr
{


// public methods

CSceneProviderGuiComp::CSceneProviderGuiComp()
:	m_fitMode(FM_NONE),
	m_isFullScreenMode(false),
	m_isZoomIgnored(false),
	m_savedParentWidgetPtr(NULL),
	m_isotropyFactor(0)
{
	m_printCommand.SetGroupId(iqtdoc::CMainWindowGuiComp::GI_DOCUMENT);
	m_printCommand.setShortcut(tr("Ctrl+P"));
	connect(&m_printCommand, SIGNAL(triggered()), this, SLOT(OnPrint()));
	m_fileMenu.InsertChild(&m_printCommand);

	m_selectAllCommand.setShortcut(Qt::CTRL + Qt::Key_A);
	connect(&m_selectAllCommand, SIGNAL(triggered()), this, SLOT(OnSelectAllShapes()));
	m_editMenu.InsertChild(&m_selectAllCommand);

	m_autoFitToViewCommand.SetStaticFlags(
				iqtgui::CHierarchicalCommand::CF_ONOFF | 
				iqtgui::CHierarchicalCommand::CF_GLOBAL_MENU);
	connect(&m_autoFitToViewCommand, SIGNAL(toggled(bool)), this, SLOT(OnAutoFit(bool)));
	m_viewMenu.InsertChild(&m_autoFitToViewCommand);

	connect(&m_fitToViewCommand, SIGNAL(triggered()), this, SLOT(OnFitToView()));
	m_viewMenu.InsertChild(&m_fitToViewCommand);

	connect(&m_fitToImageCommand, SIGNAL(triggered()), this, SLOT(OnFitToShapes()));
	m_viewMenu.InsertChild(&m_fitToImageCommand);

	connect(&m_resetZoomCommand, SIGNAL(triggered()), this, SLOT(OnResetScale()));
	m_viewMenu.InsertChild(&m_resetZoomCommand);

	m_commands.InsertChild(&m_fileMenu);
	m_commands.InsertChild(&m_editMenu);
	m_commands.InsertChild(&m_viewMenu);
}


double CSceneProviderGuiComp::GetIsotropyFactor() const
{
	return m_isotropyFactor;
}


void CSceneProviderGuiComp::SetIsotropyFactor(double factor)
{
	if (factor != m_isotropyFactor){
		m_isotropyFactor = factor;

		if ((m_fitMode == FM_ISOTROPIC_REDUCTION) || (m_fitMode == FM_ISOTROPIC)){
			SetFittedScale(m_fitMode);
		}
	}
}

int CSceneProviderGuiComp::GetEmbeddedListSize() const
{
	return LocalCompositionSelectorList->count();
}


void CSceneProviderGuiComp::ClearEmbeddedList()
{
	LocalCompositionSelectorGroupBox->setVisible(false);
	iqt::CSignalBlocker blocker(LocalCompositionSelectorList);
	LocalCompositionSelectorList->clear();
	LocalCompositionSelectorList->addItem("<< root >>");
	LocalCompositionSelectorList->setCurrentItem(0);
}


void CSceneProviderGuiComp::InsertEmbeddedIntoList(QByteArray id)
{
	if (LocalCompositionSelectorList->count() == 0){
		ClearEmbeddedList();
	}
	QList<QListWidgetItem *> items = LocalCompositionSelectorList->findItems(id, Qt::MatchExactly);
	if (items.empty()){
		LocalCompositionSelectorList->addItem(id);
	}
	LocalCompositionSelectorGroupBox->setVisible(true);
}


void CSceneProviderGuiComp::RemoveEmbeddedFromList(QByteArray id)
{
	QList<QListWidgetItem *> items = LocalCompositionSelectorList->findItems(id, Qt::MatchExactly);
	if (items.empty()){
		return;
	}
	int row = LocalCompositionSelectorList->row(items[0]);
	LocalCompositionSelectorList->model()->removeRow(row);
	if (LocalCompositionSelectorList->count() == 1){ // just root
		LocalCompositionSelectorGroupBox->setVisible(false);
	}
}


void CSceneProviderGuiComp::SelectEmbeddedInList(QByteArray id, bool propagateEvent /* = true */)
{
	QList<QListWidgetItem *> items = LocalCompositionSelectorList->findItems(id, Qt::MatchExactly);

	if (items.empty()){
		return;
	}

	LocalCompositionSelectorList->blockSignals(!propagateEvent);
	LocalCompositionSelectorList->setCurrentItem(items[0]);
	LocalCompositionSelectorList->blockSignals(false);
}


const ibase::IHierarchicalCommand* CSceneProviderGuiComp::GetCommands() const
{
	return &m_commands;
}


// reimplemented (icmpstr::ISceneProvider)

int CSceneProviderGuiComp::GetSceneId() const
{
	I_ASSERT(m_sceneIdAttrPtr.IsValid());

	return *m_sceneIdAttrPtr;
}


QGraphicsScene* CSceneProviderGuiComp::GetScene() const
{
	return m_scenePtr.GetPtr();
}


bool CSceneProviderGuiComp::GetSceneAlignment(double& distance) const
{
	if (*m_isAlignmentEnabledAttrPtr){
		distance = *m_gridSizeAttrPtr;

		return true;
	}

	return false;
}



int CSceneProviderGuiComp::GetSceneFlags() const
{
	int sceneFlags = 0;

	if (m_sceneWidthAttrPtr.IsValid() && m_sceneHeightAttrPtr.IsValid()){
		sceneFlags = sceneFlags | SF_FIXED_SIZE;
	}

	if (*m_isAlignmentEnabledAttrPtr){
		sceneFlags = sceneFlags | SF_ALIGNED;
	}

	return sceneFlags;
}


// reimplemented (i2d::ISceneController)

int CSceneProviderGuiComp::GetSceneRestrictionFlags() const
{
	if (m_fitMode != FM_NONE){
		return SR_ALL;
	}

	return SR_NONE;
}


CSceneProviderGuiComp::FitMode CSceneProviderGuiComp::GetFitMode() const
{
	return m_fitMode;
}


bool CSceneProviderGuiComp::SetFitMode(FitMode mode)
{
	if (mode != m_fitMode){
		m_fitMode = mode;

		if (m_fitMode != FM_NONE){
			SceneView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
			SceneView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

			SetFittedScale(m_fitMode);
		}
		else{
			SceneView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
			SceneView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		}
	}

	return true;
}


bool CSceneProviderGuiComp::IsFullScreenMode() const
{
	return m_isFullScreenMode;
}


bool CSceneProviderGuiComp::SetFullScreenMode(bool fullScreenMode)
{
	if (fullScreenMode != m_isFullScreenMode){
		m_isFullScreenMode = fullScreenMode;

		if (m_isFullScreenMode != CompleteFrame->isFullScreen()){
			if (m_isFullScreenMode){
				m_savedParentWidgetPtr = CompleteFrame->parentWidget();
				m_savedViewTransform = SceneView->matrix();
				CompleteFrame->setParent(NULL);

				BottomFrame->setVisible(false);
				CompleteFrame->showFullScreen();

				SetFittedScale(m_fitMode != FM_NONE? m_fitMode: FM_ISOTROPIC);
			}
			else{
				if (m_savedParentWidgetPtr != NULL){
					CompleteFrame->setParent(m_savedParentWidgetPtr);
					m_savedParentWidgetPtr->layout()->addWidget(CompleteFrame);
				}
				
				m_savedParentWidgetPtr = NULL;

				BottomFrame->setVisible(m_sceneControllerGuiCompPtr.IsValid());

				CompleteFrame->showNormal();

				SceneView->setMatrix(m_savedViewTransform);
			}
		}
	}

	return true;
}


double CSceneProviderGuiComp::GetScale() const
{
	QMatrix currentMatrix = SceneView->matrix();
	double currentScale = sqrt(currentMatrix.det());

	return currentScale;
}


bool CSceneProviderGuiComp::SetScale(int scaleMode, double scaleFactor)
{
	if (scaleMode != SM_SET){
		return false;
	}

	if (m_isZoomIgnored || (m_fitMode != FM_NONE)){
		return false;
	}

	double currentScale = GetScale();

	QMatrix newMatrix;
	newMatrix.scale(scaleFactor / currentScale, scaleFactor/ currentScale);

	QMatrix currentMatrix = SceneView->matrix();
	currentMatrix *= newMatrix;

	SceneView->setMatrix(currentMatrix);

	m_isZoomIgnored = true;

	Q_EMIT zoomChanged(scaleFactor);

	m_isZoomIgnored = false;

	return true;
}


// protected methods

bool CSceneProviderGuiComp::OnResize(QResizeEvent* /*eventPtr*/)
{
	if (m_fitMode != FM_NONE){
		SetFittedScale(m_fitMode);

		return true;
	}

	return false;
}


bool CSceneProviderGuiComp::OnWheelEvent(QGraphicsSceneWheelEvent* eventPtr)
{
	if (m_fitMode == FM_NONE){
		ScaleView(qPow((double)2, eventPtr->delta() / 240.0));

		return true;
	}

	return false;
}


bool CSceneProviderGuiComp::OnMouseDoubleClickEvent(QEvent* eventPtr)
{
	QGraphicsSceneMouseEvent* sceneEventPtr = dynamic_cast<QGraphicsSceneMouseEvent*>(eventPtr);
	if (sceneEventPtr != NULL){
		QGraphicsItem* mouseItemPtr = m_scenePtr->itemAt(sceneEventPtr->scenePos());
		if (mouseItemPtr != NULL){
			m_scenePtr->sendEvent(mouseItemPtr, eventPtr);
		}
	}

	if (!eventPtr->isAccepted()){
		SetFullScreenMode(!IsFullScreenMode());
	}

	return true;
}


bool CSceneProviderGuiComp::OnKeyReleaseEvent(QKeyEvent* eventPtr)
{
	switch(eventPtr->key()){
	case Qt::Key_Plus:
		OnZoomIncrement();
		return true;

	case Qt::Key_Minus:
		OnZoomDecrement();
		return true;

	case Qt::Key_Escape:
		if (IsFullScreenMode()){
			SetFullScreenMode(false);

			return true;
		}
		break;
	}

	return false;
}


void CSceneProviderGuiComp::ScaleView(double scaleFactor)
{
	double currentScale = GetScale();

	SetScale(SM_SET, scaleFactor * currentScale);
}


void CSceneProviderGuiComp::SetFittedScale(FitMode mode)
{
	if ((mode < FM_ISOTROPIC_REDUCTION) || (mode > FM_ANISOTROPIC)){
		return;
	}

	QRectF sceneRect = SceneView->sceneRect();
	if ((sceneRect.width() < I_BIG_EPSILON) || (sceneRect.height() < I_BIG_EPSILON)){
		return;
	}

	double scaleX = SceneView->width() / sceneRect.width();
	double scaleY = SceneView->height() / sceneRect.height();

	if (mode <= FM_ISOTROPIC){
		double newScale = qMin(scaleX, scaleY) * (1 - m_isotropyFactor) + qMax(scaleX, scaleY) * m_isotropyFactor;

		if ((mode == FM_ISOTROPIC_REDUCTION) && newScale >= 1){
			newScale = 1;
		}

		scaleX = newScale;
		scaleY = newScale;
	}

	QMatrix fitMatrix;
	fitMatrix.scale(scaleX, scaleY);
	fitMatrix.translate(-sceneRect.left(), -sceneRect.top());

	SceneView->setMatrix(fitMatrix);
}


bool CSceneProviderGuiComp::HasDropConsumerForFormat(const QStringList& formats) const
{
	if (!m_dropConsumersCompPtr.IsValid() || m_dropConsumersCompPtr.GetCount() == 0){
		return false;
	}

	for (int formatIndex = 0; formatIndex < formats.size(); formatIndex++){
		for (int consumerIndex = 0; consumerIndex < m_dropConsumersCompPtr.GetCount(); consumerIndex++){
			iqtgui::IDropConsumer* dropConsumerPtr = m_dropConsumersCompPtr[consumerIndex];
			if (dropConsumerPtr != NULL){
				QStringList supportedIds = dropConsumerPtr->GetAcceptedMimeIds();

				if (supportedIds.contains(formats[formatIndex])){
					return true;
				}
			}
		}
	}
	
	return false;
}


void CSceneProviderGuiComp::DelegateDropEvent(const QMimeData& mimeData, QGraphicsSceneDragDropEvent* eventPtr)
{
	if (!m_dropConsumersCompPtr.IsValid() || m_dropConsumersCompPtr.GetCount() == 0){
		return;
	}

	QStringList formats = mimeData.formats();

	for (int consumerIndex = 0; consumerIndex < m_dropConsumersCompPtr.GetCount(); consumerIndex++){
		iqtgui::IDropConsumer* dropConsumerPtr = m_dropConsumersCompPtr[consumerIndex];
		if (dropConsumerPtr == NULL){
			continue;
		}

		QStringList supportedIds = dropConsumerPtr->GetAcceptedMimeIds();
		
		bool triggerEvent = false;
		for (int idIndex = 0; idIndex < supportedIds.size(); idIndex++){
			if (formats.contains(supportedIds[idIndex])){
				triggerEvent = true;
				
				break;
			}
		}

		if (triggerEvent){
			dropConsumerPtr->OnDropFinished(mimeData, eventPtr);
		}
	}
}


void CSceneProviderGuiComp::ResetScene()
{
	QList<QGraphicsItem*> sceneItems = m_scenePtr->items();
	foreach(QGraphicsItem* itemPtr, sceneItems){
		m_scenePtr->removeItem(itemPtr);
	}
	
	m_scenePtr.Reset();
}


// reimplemented (iqtgui::CGuiComponentBase)

void CSceneProviderGuiComp::OnGuiCreated()
{
	ClearEmbeddedList(); // hide

	BaseClass::OnGuiCreated();

	if (m_dropConsumersCompPtr.IsValid() && m_dropConsumersCompPtr.GetCount() > 0){
		SceneView->setAcceptDrops(true);
	}

	SceneView->setScene(m_scenePtr.GetPtr());
	SceneView->setMouseTracking(true);
	SceneView->setDragMode(QGraphicsView::ScrollHandDrag);
	
	switch (*m_backgroundModeAttrPtr){
		case BM_NORMAL:
			SceneView->setBackgroundBrush(qApp->palette().button());
			break;

		case BM_SOLID:
			SceneView->setBackgroundBrush(qApp->palette().window());
			break;

		case BM_CHECKERBOARD:{
			QPixmap backgroundPixmap(16, 16);

			QPainter p(&backgroundPixmap);
			p.fillRect(0, 0, 8, 8, QBrush(qRgb(200,200,200)));
			p.fillRect(0, 8, 8, 8, QBrush(Qt::white));
			p.fillRect(8, 0, 8, 8, QBrush(Qt::white));
			p.fillRect(8, 8, 8, 8, QBrush(qRgb(200,200,200)));
			SceneView->setBackgroundBrush(QBrush(backgroundPixmap));
			break;
		}

		case BM_TRANSPARENT:
			SceneView->setBackgroundBrush(Qt::transparent);
			break;
	}

	SceneView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	SceneView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	SceneView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	SceneView->installEventFilter(this);

	if (m_useAntialiasingAttrPtr.IsValid() && *m_useAntialiasingAttrPtr){
		SceneView->setRenderHints(QPainter::Antialiasing);
	}

	m_isotropyFactor = *m_isotropyFactorAttrPtr;


	if (m_fitModeAttrPtr.IsValid()){
		SetFitMode(FitMode(*m_fitModeAttrPtr));
	}

	m_scenePtr->installEventFilter(this);

	// try to place the scene controller:
	if (m_sceneControllerGuiCompPtr.IsValid()){
		m_sceneControllerGuiCompPtr->CreateGui(ControllerFrame);

		if (*m_backgroundModeAttrPtr == BM_SOLID){
			QPalette viewPalette = SceneView->palette();
			viewPalette.setColor(QPalette::Window, SceneView->backgroundBrush().color());

			QColor backgroundColor = SceneView->backgroundBrush().color();
			QString backgroundColorString = QString("background-color: rgb(%1,%2,%3)").arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue());
			BottomFrame->setStyleSheet(backgroundColorString);
			ControllerFrame->setStyleSheet(backgroundColorString);
			GetWidget()->setStyleSheet(backgroundColorString);
			m_sceneControllerGuiCompPtr->GetWidget()->setStyleSheet(backgroundColorString);
		}

		BottomFrame->setVisible(true);
	}
	else{
		BottomFrame->setVisible(false);
	}
} 


void CSceneProviderGuiComp::OnGuiDestroyed()
{
	if (m_sceneControllerGuiCompPtr.IsValid()){
		m_sceneControllerGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


void CSceneProviderGuiComp::OnRetranslate()
{
	BaseClass::OnRetranslate();

	m_fileMenu.SetVisuals(tr("&File"), tr("File"), tr("File menu"));
	m_printCommand.SetVisuals(tr("&Print..."), tr("Print"), tr("Prints current document"), QIcon(":/Icons/Print"));
	m_editMenu.SetName(tr("&Edit"));
	m_viewMenu.SetVisuals(tr("&View"), tr("View"), tr("View menu"));
	m_autoFitToViewCommand.SetVisuals(tr("&Auto Fit"), tr("Auto Fit"), tr("Automatical fit contents to view area"));
	m_fitToViewCommand.SetVisuals(tr("&Fit Contents To View"), tr("Fit contents To View"), tr("Fit contents to view area"));
	m_fitToImageCommand.SetVisuals(tr("&Fit View To Contents"), tr("Fit View To Contents"), tr("Fit view area to contents"));
	m_resetZoomCommand.SetVisuals(tr("&Reset Zoom"), tr("Reset Zoom"), tr("Reset scene zoom"));
	m_selectAllCommand.SetVisuals(tr("&Select All"), tr("Select All"), tr("Select all shapes"));
}


// reimplemented (icomp::CComponentBase)

void CSceneProviderGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_scenePtr.SetPtr(new CScene(this));
	m_scenePtr->setFocus();

	if (m_sceneWidthAttrPtr.IsValid() && m_sceneHeightAttrPtr.IsValid()){
		m_scenePtr->setSceneRect(0, 0, *m_sceneWidthAttrPtr, *m_sceneHeightAttrPtr);
	}

	I_ASSERT(m_allowWidgetResizeAttrPtr.IsValid());	// this attribute is obligatory
	m_fitToImageCommand.setVisible(*m_allowWidgetResizeAttrPtr);
	I_ASSERT(m_allowSelectAllAttrPtr.IsValid());	// this attribute is obligatory
	m_selectAllCommand.setVisible(*m_allowSelectAllAttrPtr);
}


void CSceneProviderGuiComp::OnComponentDestroyed()
{
	ResetScene();

	BaseClass::OnComponentDestroyed();
}


// reimplemented (QObject)

bool CSceneProviderGuiComp::eventFilter(QObject* sourcePtr, QEvent* eventPtr)
{
	if ((sourcePtr != SceneView) && (sourcePtr != m_scenePtr.GetPtr())){
		return BaseClass::eventFilter(sourcePtr, eventPtr);
	}

	if (IsGuiCreated()){
		switch(eventPtr->type()){
//		case QEvent::MouseButtonDblClick:
		case QEvent::GraphicsSceneMouseDoubleClick:
			if (OnMouseDoubleClickEvent(eventPtr)){
				return true;
			}
			break;

		case QEvent::KeyRelease:
			if (OnKeyReleaseEvent(dynamic_cast<QKeyEvent*>(eventPtr))){
				return true;
			}
			break;

		case QEvent::Resize:
			OnResize(dynamic_cast<QResizeEvent*>(eventPtr));
			break;

		case QEvent::GraphicsSceneWheel:
			if (OnWheelEvent(dynamic_cast<QGraphicsSceneWheelEvent*>(eventPtr))){
				return true;
			}
			break;

		default:
			break;
		}
	}

	return BaseClass::eventFilter(sourcePtr, eventPtr);
}


// protected slots

void CSceneProviderGuiComp::OnPrint()
{
	if (!m_scenePtr.IsValid()){
		return;
	}

	QPrinter printer;

	QRectF sceneRect = m_scenePtr->sceneRect();
	if (sceneRect.width() <= sceneRect.height()){
		printer.setOrientation(QPrinter::Portrait);
	}
	else{
		printer.setOrientation(QPrinter::Landscape);
	}

	QPrintDialog printerDialog(&printer, GetQtWidget());
	if (printerDialog.exec() != QDialog::Accepted){
		return;
	}

	QPainter painter(&printer);

	 // print, fitting the viewport contents into a full page
	m_scenePtr->render(&painter);
}


void CSceneProviderGuiComp::OnZoomIncrement()
{
	 ScaleView(qPow((double)2, 0.5));  
}


void CSceneProviderGuiComp::OnZoomDecrement()
{
	ScaleView(qPow((double)2, -0.5));  
}


void CSceneProviderGuiComp::OnFitToView()
{
	SetFittedScale(FM_ISOTROPIC);
}


void CSceneProviderGuiComp::OnFitToShapes()
{
	SceneView->showNormal();

	double r = SceneView->matrix().m11();
	QRectF sceneRect = SceneView->sceneRect();
	SceneView->resize(int(sceneRect.width() * r), int(sceneRect.height() * r));
}


void CSceneProviderGuiComp::OnResetScale()
{
	SetScale(SM_SET, 1.0);
}


void CSceneProviderGuiComp::OnAutoFit(bool isAutoScale)
{
	if (isAutoScale){
		SetFitMode(FM_ISOTROPIC);
	}
	else{
		SetFitMode(FM_NONE);
	}

	m_fitToViewCommand.setEnabled(!isAutoScale);
	m_resetZoomCommand.setEnabled(!isAutoScale);
}


void CSceneProviderGuiComp::OnSelectAllShapes()
{
	if (m_scenePtr != NULL){
		QPainterPath sceneSelection;
		sceneSelection.addRect(m_scenePtr->sceneRect());

		QList<QGraphicsItem*> itemList = m_scenePtr->items();
		for (int itemIndex = 0; itemIndex < itemList.count(); itemIndex++){
			QGraphicsItem* itemPtr = itemList.at(itemIndex);
			I_ASSERT(itemPtr != NULL);

			if ((itemPtr->flags() & QGraphicsItem::ItemIsSelectable) != 0){
				itemPtr->setSelected(true);
			}
		}
	}
}


void CSceneProviderGuiComp::on_LocalCompositionSelectorList_itemSelectionChanged()
{
	QByteArray itemLabel = LocalCompositionSelectorList->currentItem()->data(0).toByteArray();
	embeddedRegistrySelected(itemLabel);
}


// public methods of embedded class CScene

CSceneProviderGuiComp::CScene::CScene(CSceneProviderGuiComp* parentPtr)
:	m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class CScene

void CSceneProviderGuiComp::CScene::DrawGrid(QPainter& painter, const QRectF& rect, bool useDot)  
{
	QRectF gridRect = rect;
	if (!sceneRect().isEmpty()){
		gridRect = sceneRect().intersected(rect);

		painter.fillRect(rect, qApp->palette().button());
		painter.fillRect(gridRect, qApp->palette().base());
	}
	else{
		painter.fillRect(gridRect, qApp->palette().base());
	}

	int gridSize = *m_parent.m_gridSizeAttrPtr;

	QRect realRect = gridRect.toAlignedRect();

	QColor gridColor = qApp->palette().highlight().color();

	int firstLeftGridLine = realRect.left() - (realRect.left() % gridSize);
	int firstTopGridLine = realRect.top() - (realRect.top() % gridSize);

	if (!useDot){
		QVarLengthArray<QLine> lines;
		for (int x = firstLeftGridLine; x <= realRect.right(); x += gridSize){
			lines.append(QLine(x, realRect.top(), x, realRect.bottom()));
		}

		for (int y = firstTopGridLine; y <= realRect.bottom(); y += gridSize){
			lines.append(QLine(realRect.left(), y, realRect.right(), y));           
		}

		gridColor.setAlpha(128);
		QPen gridPen(gridColor);
		gridPen.setWidthF(0.5);
		painter.setPen(gridPen);

		painter.save();
		painter.setRenderHint(QPainter::Antialiasing);
		painter.drawLines(lines.data(), lines.size());
		painter.restore();
	}
	else{
		QPolygon points;
		for (int x = firstLeftGridLine; x <= realRect.right(); x += gridSize){
			for (int y = firstTopGridLine; y <= realRect.bottom(); y += gridSize){
				points.append(QPoint(x, y));
			}
		}

		painter.setPen(gridColor);
		painter.drawPoints(points);
	}

	if (!sceneRect().isEmpty()){
		painter.setPen(qApp->palette().shadow().color());

		painter.drawRect(sceneRect());
	}
}


// reimplemented (QGraphicsScene)

void CSceneProviderGuiComp::CScene::drawBackground(QPainter* painter, const QRectF& rect)
{
	if (m_parent.IsGuiCreated() && m_parent.m_backgroundModeAttrPtr.IsValid()){
		switch (*m_parent.m_backgroundModeAttrPtr){
		case BM_GRID:
			DrawGrid(*painter, rect, false);
			break;

		case BM_DOT_GRID:
			DrawGrid(*painter, rect, true);
			break;

		default:
			QGraphicsScene::drawBackground(painter, rect);
		}
	}
	else{
		QGraphicsScene::drawBackground(painter, rect);
	}
}


void CSceneProviderGuiComp::CScene::dragEnterEvent(QGraphicsSceneDragDropEvent* eventPtr)
{
	const QStringList& formats = eventPtr->mimeData()->formats();

	if (m_parent.HasDropConsumerForFormat(formats)){
		eventPtr->acceptProposedAction();
	}
}


void CSceneProviderGuiComp::CScene::dropEvent(QGraphicsSceneDragDropEvent* eventPtr)
{
	const QMimeData* dataPtr = eventPtr->mimeData();
	if ((dataPtr != NULL)){
		m_parent.DelegateDropEvent(*dataPtr, eventPtr);

		eventPtr->acceptProposedAction();
	}
	else{
		eventPtr->ignore();
	}
}


void CSceneProviderGuiComp::CScene::dragMoveEvent(QGraphicsSceneDragDropEvent* /*eventPtr*/)
{
}


} // namespace icmpstr




