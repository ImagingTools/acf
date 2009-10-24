#include "iqt2d/CQuadrangleShape.h"


#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"


namespace iqt2d
{


// public methods

CQuadrangleShape::CQuadrangleShape(bool isEditable, const ISceneProvider* providerPtr)
:	BaseClass(isEditable, providerPtr),
	m_firstDiagGrip1(this, providerPtr),
	m_firstDiagGrip2(this, providerPtr),
	m_secondDiagGrip1(this, providerPtr),
	m_secondDiagGrip2(this, providerPtr)
{
	connect(&m_firstDiagGrip1, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnFirstDiagGrip1Changed(const QPointF&)));
	connect(&m_firstDiagGrip2, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnFirstDiagGrip2Changed(const QPointF&)));
	connect(&m_secondDiagGrip1, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnSecondDiagGrip1Changed(const QPointF&)));
	connect(&m_secondDiagGrip2, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnSecondDiagGrip2Changed(const QPointF&)));
	
	m_firstDiagGrip2.SetLabelPosition(CGripShape::LabelBottom);
	m_secondDiagGrip2.SetLabelPosition(CGripShape::LabelBottom);

	if (!isEditable){
		m_firstDiagGrip1.setParentItem(NULL);
		m_firstDiagGrip2.setParentItem(NULL);
		m_secondDiagGrip1.setParentItem(NULL);
		m_secondDiagGrip2.setParentItem(NULL);
	}
}


// reimplemented (imod::IObserver)

void CQuadrangleShape::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	i2d::CQuadrangle* quadranglePtr = GetObjectPtr();
	if (quadranglePtr != NULL){
		i2d::CLine2d  firstDiag = quadranglePtr->GetFirstDiagonal();
		i2d::CLine2d  secondDiag = quadranglePtr->GetSecondDiagonal();

		QPainterPath path;

		if (!quadranglePtr->IsQuadrangleValid()){
			setPen(QPen(Qt::red, 0));
		}
		else{
			if (IsEditable()){
				setPen(GetPen(EditableColor));
			}
			else{
				setPen(GetPen(InactiveColor));
			}
		}

		path.moveTo(iqt::GetQPointF(firstDiag.GetPoint1()));
		path.lineTo(iqt::GetQPointF(secondDiag.GetPoint1()));
	
		path.moveTo(iqt::GetQPointF(secondDiag.GetPoint1()));
		path.lineTo(iqt::GetQPointF(firstDiag.GetPoint2()));

		path.moveTo(iqt::GetQPointF(firstDiag.GetPoint2()));
		path.lineTo(iqt::GetQPointF(secondDiag.GetPoint2()));

		path.moveTo(iqt::GetQPointF(secondDiag.GetPoint2()));
		path.lineTo(iqt::GetQPointF(firstDiag.GetPoint1()));

		setPath(path);

		UpdateGripPositions();
	}
}


// protected slots

void CQuadrangleShape::OnFirstDiagGrip1Changed(const QPointF& point)
{
	i2d::CQuadrangle* quadranglePtr = GetObjectPtr();
	if (quadranglePtr != NULL){
		i2d::CQuadrangle orginalQuadrangle = *quadranglePtr;
		bool wasValid = quadranglePtr->IsQuadrangleValid();

		i2d::CLine2d firstDiag = quadranglePtr->GetFirstDiagonal();
		firstDiag.SetPoint1(iqt::GetCVector2d(point));

		quadranglePtr->SetFirstDiagonal(firstDiag);

		// restore valid quadrangle:
		if (wasValid  && !quadranglePtr->IsQuadrangleValid()){
			*quadranglePtr = orginalQuadrangle;
		}
	}
}


void CQuadrangleShape::OnFirstDiagGrip2Changed(const QPointF& point)
{
	i2d::CQuadrangle* quadranglePtr = GetObjectPtr();
	if (quadranglePtr != NULL){
		i2d::CQuadrangle orginalQuadrangle = *quadranglePtr;
		bool wasValid = quadranglePtr->IsQuadrangleValid();

		i2d::CLine2d firstDiag = quadranglePtr->GetFirstDiagonal();
		firstDiag.SetPoint2(iqt::GetCVector2d(point));

		quadranglePtr->SetFirstDiagonal(firstDiag);

		// restore valid quadrangle:
		if (wasValid && !quadranglePtr->IsQuadrangleValid()){
			*quadranglePtr = orginalQuadrangle;
		}
	}
}


void CQuadrangleShape::OnSecondDiagGrip1Changed(const QPointF& point)
{
	i2d::CQuadrangle* quadranglePtr = GetObjectPtr();
	if (quadranglePtr != NULL){
		i2d::CQuadrangle orginalQuadrangle = *quadranglePtr;
		bool wasValid = quadranglePtr->IsQuadrangleValid();

		i2d::CLine2d secondDiag = quadranglePtr->GetSecondDiagonal();
		secondDiag.SetPoint1(iqt::GetCVector2d(point));

		quadranglePtr->SetSecondDiagonal(secondDiag);

		// restore valid quadrangle:
		if (wasValid && !quadranglePtr->IsQuadrangleValid()){
			*quadranglePtr = orginalQuadrangle;
		}
	}
}


void CQuadrangleShape::OnSecondDiagGrip2Changed(const QPointF& point)
{
	i2d::CQuadrangle* quadranglePtr = GetObjectPtr();
	if (quadranglePtr != NULL){
		i2d::CQuadrangle orginalQuadrangle = *quadranglePtr;
		bool wasValid = quadranglePtr->IsQuadrangleValid();

		i2d::CLine2d secondDiag = quadranglePtr->GetSecondDiagonal();
		secondDiag.SetPoint2(iqt::GetCVector2d(point));

		quadranglePtr->SetSecondDiagonal(secondDiag);

		// restore valid quadrangle:
		if (wasValid && !quadranglePtr->IsQuadrangleValid()){
			*quadranglePtr = orginalQuadrangle;
		}
	}
}


// private methods

void CQuadrangleShape::UpdateGripPositions()
{
	i2d::CQuadrangle* quadranglePtr = GetObjectPtr();
	if (quadranglePtr != NULL){
		iqt::CSignalBlocker block(&m_firstDiagGrip1);
		m_firstDiagGrip1.setPos(iqt::GetQPointF(quadranglePtr->GetFirstDiagonal().GetPoint1()));

		iqt::CSignalBlocker block2(&m_firstDiagGrip2);
		m_firstDiagGrip2.setPos(iqt::GetQPointF(quadranglePtr->GetFirstDiagonal().GetPoint2()));

		iqt::CSignalBlocker block3(&m_secondDiagGrip1);
		m_secondDiagGrip1.setPos(iqt::GetQPointF(quadranglePtr->GetSecondDiagonal().GetPoint1()));

		iqt::CSignalBlocker block4(&m_secondDiagGrip2);
		m_secondDiagGrip2.setPos(iqt::GetQPointF(quadranglePtr->GetSecondDiagonal().GetPoint2()));
	}
}


} // namespace iqt2d


