#include "iview/CPolylineCalibrationShape.h"



// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include "i2d/CPolyline.h"

#include "iqt/iqt.h"

#include "iview/IColorSchema.h"
#include "iview/CScreenTransform.h"


namespace iview
{


CPolylineCalibrationShape::CPolylineCalibrationShape()
{
	m_isOrientationVisible = false;
}
	
	
// reimplemented (iview::IMouseActionObserver)

bool CPolylineCalibrationShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	imod::IModel* modelPtr = GetModelPtr();
	i2d::CPolyline* polylinePtr = dynamic_cast<i2d::CPolyline*>(modelPtr);
	if (IsDisplayConnected() && (polylinePtr != NULL)){
		if (downFlag){
			const iview::IColorSchema& colorSchema = GetColorSchema();
			const iview::CScreenTransform& transform = GetLogToScreenTransform();
			const i2d::ITransformation2d& calib = GetIsomorphCalib();

			i2d::CVector2d logMouse;
			calib.GetPositionAt(transform.GetClientPosition(position), logMouse);

			int nodesCount = polylinePtr->GetNodesCount();

			int editMode = GetEditMode();
			switch (editMode){
			case iview::ISelectable::EM_NONE:
				if (IsSelected() && CTransformableRectangleCalibrationShape::OnMouseButton(position, buttonType, downFlag)){
					return true;
				}
				break;

			case iview::ISelectable::EM_MOVE:
				{
					const i2d::CRect& tickerBox = colorSchema.GetTickerBox(iview::IColorSchema::TT_MOVE);

					for (int i = nodesCount - 1; i >= 0; --i){
						const i2d::CVector2d& logPos = polylinePtr->GetNode(i);
						i2d::CVector2d viewPos;
						calib.GetInvPositionAt(logPos, viewPos);
						istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);
						if (tickerBox.IsInside(position - screenPos)){
							m_referencePosition = logPos - logMouse;
							m_referenceIndex = i;

							BeginModelChanges();
							return true;
						}
					}
				}
				break;

			case iview::ISelectable::EM_ADD:
				{
					const i2d::CRect& tickerBox = colorSchema.GetTickerBox(iview::IColorSchema::TT_INSERT);

					i2d::CVector2d logLast, viewLast;
					if (!polylinePtr->IsClosed()){
						logLast = polylinePtr->GetNode(nodesCount - 1);
					}
					else{
						logLast = GetSegmentMiddle(nodesCount - 1);
					}
					calib.GetInvPositionAt(logLast, viewLast);
					istd::CIndex2d screenLast = transform.GetScreenPosition(viewLast);
					if (tickerBox.IsInside(position - screenLast)){
						BeginModelChanges();
						if (polylinePtr->InsertNode(logLast)){
							m_referencePosition = logLast - logMouse;
							m_referenceIndex = nodesCount;

							UpdateModelChanges();
							return true;
						}
					}
					for (int i = nodesCount - 2; i >= 0; --i){
						i2d::CVector2d logPos = GetSegmentMiddle(i);
						i2d::CVector2d viewPos;
						calib.GetInvPositionAt(logPos, viewPos);
						istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);
						if (tickerBox.IsInside(position - screenPos)){
							BeginModelChanges();
							if (polylinePtr->InsertNode(i + 1, logPos)){
								m_referencePosition = logPos - logMouse;
								m_referenceIndex = i + 1;

								UpdateModelChanges();
								return true;
							}
						}
					}
					if (!polylinePtr->IsClosed()){
						i2d::CVector2d logFirst = polylinePtr->GetNode(0);
						i2d::CVector2d viewFirst;
						calib.GetInvPositionAt(logFirst, viewFirst);
						istd::CIndex2d screenFirst = transform.GetScreenPosition(viewFirst);
						if (tickerBox.IsInside(position - screenFirst)){
							BeginModelChanges();
							if (polylinePtr->InsertNode(0, logFirst)){
								m_referencePosition = logFirst - logMouse;
								m_referenceIndex = 0;

								UpdateModelChanges();
								return true;
							}
						}
					}
				}
				break;

			case iview::ISelectable::EM_REMOVE:
				if (nodesCount > 2){
					const i2d::CRect& tickerBox = colorSchema.GetTickerBox(iview::IColorSchema::TT_DELETE);

					for (int i = nodesCount - 1; i >= 0; --i){
						const i2d::CVector2d& logPos = polylinePtr->GetNode(i);
						i2d::CVector2d viewPos;
						calib.GetInvPositionAt(logPos, viewPos);
						istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);
						if (tickerBox.IsInside(position - screenPos)){
							BeginModelChanges();
							polylinePtr->RemoveNode(i);

							EndModelChanges();
							return true;
						}
					}
				}
				break;
			}
		}
	}

	EndModelChanges();
	return false;
}


// reimplemented (imod::IObserver)

bool CPolylineCalibrationShape::OnAttached(imod::IModel* modelPtr)
{
	Q_ASSERT(dynamic_cast<i2d::CPolyline*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


// protected methods

void CPolylineCalibrationShape::DrawCurve(QPainter& drawContext) const
{
	const i2d::CPolyline* polylinePtr = dynamic_cast<const i2d::CPolyline*>(GetModelPtr());
	if (IsDisplayConnected() && (polylinePtr != NULL)){
		const iview::IColorSchema& colorSchema = GetColorSchema();
		int nodesCount = polylinePtr->GetNodesCount();
		if (nodesCount > 0){
			int secondPointIndex;
			int pointIndex;
			QPoint firstPoint;
			QPoint point1;

			const iview::CScreenTransform& transform = GetLogToScreenTransform();
			const i2d::ITransformation2d& calib = GetIsomorphCalib();

			if (polylinePtr->IsClosed()){
				i2d::CVector2d viewPos;
				calib.GetInvPositionAt(polylinePtr->GetNode(nodesCount - 1), viewPos);
				firstPoint = iqt::GetQPoint(transform.GetScreenPosition(viewPos));
				secondPointIndex = 0;
			}
			else{
				i2d::CVector2d viewPos;
				calib.GetInvPositionAt(polylinePtr->GetNode(0), viewPos);
				firstPoint = iqt::GetQPoint(transform.GetScreenPosition(viewPos));
				secondPointIndex = 1;
			}
			if (m_isOrientationVisible && IsSelected()){
				const QPen& darkPen = colorSchema.GetPen(iview::IColorSchema::SP_ORIENT_DARK);
				const QPen& brightPen = colorSchema.GetPen(iview::IColorSchema::SP_ORIENT_BRIGHT);

				point1 = firstPoint;
				for (pointIndex = secondPointIndex; pointIndex < nodesCount; ++pointIndex){
					i2d::CVector2d viewPos;
					calib.GetInvPositionAt(polylinePtr->GetNode(pointIndex), viewPos);
					QPoint point2 = iqt::GetQPoint(transform.GetScreenPosition(viewPos));
					istd::CIndex2d delta(point2.x() - point1.x(), point2.y() - point1.y());
					if (qAbs(delta.GetY()) > qAbs(delta.GetX())){
						if (delta.GetY() > 0){
							drawContext.save();
							drawContext.setPen(darkPen);
							drawContext.drawLine(QPoint(point1.x() - 1, point1.y()), QPoint(point2.x() - 1, point2.y()));
							drawContext.restore();

							drawContext.save();
							drawContext.setPen(brightPen);
							drawContext.drawLine(QPoint(point1.x() + 1, point1.y()), QPoint(point2.x() + 1, point2.y()));
							drawContext.restore();
						}
						else{
							drawContext.save();
							drawContext.setPen(darkPen);
							drawContext.drawLine(QPoint(point1.x() + 1, point1.y()), QPoint(point2.x() + 1, point2.y()));
							drawContext.restore();

							drawContext.save();
							drawContext.setPen(brightPen);
							drawContext.drawLine(QPoint(point1.x() - 1, point1.y()), QPoint(point2.x() - 1, point2.y()));
							drawContext.restore();
						}
					}
					else{
						if (delta.GetX() > 0){
							drawContext.save();
							drawContext.setPen(darkPen);
							drawContext.drawLine(QPoint(point1.x(), point1.y() + 1), QPoint(point2.x(), point2.y() + 1));
							drawContext.restore();

							drawContext.save();
							drawContext.setPen(brightPen);
							drawContext.drawLine(QPoint(point1.x(), point1.y() - 1), QPoint(point2.x(), point2.y() - 1));
							drawContext.restore();
						}
						else{
							drawContext.save();
							drawContext.setPen(darkPen);
							drawContext.drawLine(QPoint(point1.x(), point1.y() - 1), QPoint(point2.x(), point2.y() - 1));
							drawContext.restore();

							drawContext.save();
							drawContext.setPen(brightPen);
							drawContext.drawLine(QPoint(point1.x(), point1.y() + 1), QPoint(point2.x(), point2.y() + 1));
							drawContext.restore();
						}
					}
					point1 = point2;
				}
			}

			if (IsSelected()){
				drawContext.save();

				drawContext.setPen(colorSchema.GetPen(iview::IColorSchema::SP_SELECTED));
			}
			else{
				drawContext.save();

				drawContext.setPen(colorSchema.GetPen(iview::IColorSchema::SP_NORMAL));
			}

			point1 = firstPoint;
			for (pointIndex = secondPointIndex; pointIndex < nodesCount; ++pointIndex){
				i2d::CVector2d viewPos;
				calib.GetInvPositionAt(polylinePtr->GetNode(pointIndex), viewPos);
				QPoint point2 = iqt::GetQPoint(transform.GetScreenPosition(viewPos));
				
				drawContext.drawLine(point1, point2);

				point1 = point2;
			}
		}

		drawContext.restore();
	}
}


void CPolylineCalibrationShape::DrawArea(QPainter&) const
{
}


void CPolylineCalibrationShape::DrawSelectionElements(QPainter& drawContext) const
{
	const i2d::CPolyline* polylinePtr = dynamic_cast<const i2d::CPolyline*>(GetModelPtr());
	if (IsDisplayConnected() && (polylinePtr != NULL)){
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const i2d::ITransformation2d& calib = GetIsomorphCalib();

		const iview::IColorSchema& colorSchema = GetColorSchema();
		int nodesCount = polylinePtr->GetNodesCount();
		int editMode = GetEditMode();

		switch (editMode){
			int i;

		case iview::ISelectable::EM_NONE:
			DrawFigure(drawContext);
			DrawTickers(drawContext);
			break;

		case iview::ISelectable::EM_MOVE:
			for (i = 0; i < nodesCount; i++){
				i2d::CVector2d viewPos;
				calib.GetInvPositionAt(polylinePtr->GetNode(i), viewPos);
				istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);

				colorSchema.DrawTicker(drawContext, screenPos, iview::IColorSchema::TT_MOVE);
			}
			break;

		case iview::ISelectable::EM_REMOVE:
			if (nodesCount > 2){
				for (i = 0; i < nodesCount; i++){
					i2d::CVector2d viewPos;
					calib.GetInvPositionAt(polylinePtr->GetNode(i), viewPos);
					istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);

					colorSchema.DrawTicker(drawContext, screenPos, iview::IColorSchema::TT_DELETE);
				}
			}
			break;

		case iview::ISelectable::EM_ADD:
			for (i = 1; i < nodesCount - 1; i++){
				i2d::CVector2d viewPos;
				calib.GetInvPositionAt(polylinePtr->GetNode(i), viewPos);
				istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);

				colorSchema.DrawTicker(drawContext, screenPos, iview::IColorSchema::TT_SELECTED_INACTIVE);
			}
			break;
		}

		if ((nodesCount > 0) && (editMode == iview::ISelectable::EM_ADD)){
			bool isOpened = !polylinePtr->IsClosed();
			if (isOpened){
				i2d::CVector2d viewPos;
				calib.GetInvPositionAt(polylinePtr->GetNode(0), viewPos);
				istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);

				colorSchema.DrawTicker(drawContext, screenPos, iview::IColorSchema::TT_INSERT);
			}

			int segmentsCount = polylinePtr->GetSegmentsCount();
			for (int i = 0; i < segmentsCount; i++){
				i2d::CVector2d viewPos;
				calib.GetInvPositionAt(GetSegmentMiddle(i), viewPos);
				istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);

				colorSchema.DrawTicker(drawContext, screenPos, iview::IColorSchema::TT_INSERT);
			}

			if (isOpened){
				i2d::CVector2d viewPos;
				calib.GetInvPositionAt(polylinePtr->GetNode(nodesCount - 1), viewPos);
				istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);

				colorSchema.DrawTicker(drawContext, screenPos, iview::IColorSchema::TT_INSERT);
			}
		}
	}
}


bool CPolylineCalibrationShape::IsCurveTouched(istd::CIndex2d position) const
{
	const i2d::CPolyline* polylinePtr = dynamic_cast<const i2d::CPolyline*>(GetModelPtr());
	if (IsDisplayConnected() && (polylinePtr != NULL)){
		int editMode = GetEditMode();
		if (IsSelected() && (editMode == iview::ISelectable::EM_NONE)){
			if (BaseClass::IsParallTouched(m_castTransform, position)){
				return true;
			}
		}

		const iview::IColorSchema& colorSchema = GetColorSchema();
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const i2d::ITransformation2d& calib = GetIsomorphCalib();

		double proportions = GetViewToScreenTransform().GetDeformMatrix().GetApproxScale();

		int nodesCount = polylinePtr->GetNodesCount();

		i2d::CVector2d node1;
		int i;
		if (polylinePtr->IsClosed()){
			calib.GetInvPositionAt(polylinePtr->GetNode(nodesCount - 1), node1);
			i = 0;
		}
		else{
			calib.GetInvPositionAt(polylinePtr->GetNode(0), node1);
			i = 1;
		}

		double logicalLineWidth = colorSchema.GetLogicalLineWidth();

		i2d::CVector2d mousePos = transform.GetClientPosition(position);

		for (; i < nodesCount; i++){
			i2d::CVector2d node2;
			calib.GetInvPositionAt(polylinePtr->GetNode(i), node2);

			i2d::CVector2d delta = node2 - node1;

			if ((delta.GetDotProduct(mousePos - node1) >= 0) && (delta.GetDotProduct(mousePos - node2) <= 0)){
				i2d::CVector2d ortonormal = delta.GetOrthogonal().GetNormalized();
				double distance = qAbs(ortonormal.GetDotProduct(mousePos - node1));
				if (proportions * distance < logicalLineWidth){
					return true;
				}
			}

			node1 = node2;
		}
	}

	return false;
}


bool CPolylineCalibrationShape::IsTickerTouched(istd::CIndex2d position) const
{
	const i2d::CPolyline* polylinePtr = dynamic_cast<const i2d::CPolyline*>(GetModelPtr());
	if (IsDisplayConnected() && (polylinePtr != NULL)){
		const iview::IColorSchema& colorSchema = GetColorSchema();
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const i2d::ITransformation2d& calib = GetIsomorphCalib();

		int nodesCount = polylinePtr->GetNodesCount();

		int editMode = GetEditMode();

		switch (editMode){
		case iview::ISelectable::EM_NONE:
			if (IsSelected()){
				EnsureValidNodes();
				if (CTransformableRectangleCalibrationShape::IsTickerTouched(position)){
					return true;
				}
			}
			break;

		case iview::ISelectable::EM_MOVE:
			{
				const i2d::CRect& tickerBox = colorSchema.GetTickerBox(iview::IColorSchema::TT_MOVE);
				for (int i = 0; i < nodesCount; i++){
					i2d::CVector2d viewPos;
					calib.GetInvPositionAt(polylinePtr->GetNode(i), viewPos);
					istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);
					if (tickerBox.IsInside(position - screenPos)){
						return true;
					}
				}
			}
			break;

		case iview::ISelectable::EM_REMOVE:
			{
				const i2d::CRect& tickerBox = colorSchema.GetTickerBox(iview::IColorSchema::TT_DELETE);
				for (int i = 0; i < nodesCount; i++){
					i2d::CVector2d viewPos;
					calib.GetInvPositionAt(polylinePtr->GetNode(i), viewPos);
					istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);
					if (tickerBox.IsInside(position - screenPos)){
						return true;
					}
				}
			}
			break;

		case iview::ISelectable::EM_ADD:
			{
				const i2d::CRect& tickerBox = colorSchema.GetTickerBox(iview::IColorSchema::TT_INACTIVE);
				int lastIndex;

				bool isOpened = !polylinePtr->IsClosed();

				if (isOpened){
					lastIndex = nodesCount - 2;

					i2d::CVector2d viewPos;
					calib.GetInvPositionAt(polylinePtr->GetNode(0), viewPos);
					istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);
					if (tickerBox.IsInside(position - screenPos)){
						return true;
					}

					calib.GetInvPositionAt(polylinePtr->GetNode(nodesCount - 1), viewPos);
					screenPos = transform.GetScreenPosition(viewPos);
					if (tickerBox.IsInside(position - screenPos)){
						return true;
					}
				}
				else{
					lastIndex = nodesCount - 1;
				}

				for (int i = 0; i <= lastIndex; i++){
					i2d::CVector2d viewPos;
					calib.GetInvPositionAt(GetSegmentMiddle(i), viewPos);
					istd::CIndex2d screenPos = transform.GetScreenPosition(viewPos);
					if (tickerBox.IsInside(position - screenPos)){
						return true;
					}
				}
			}
		}
	}

	return false;
}


} // namespace iview

