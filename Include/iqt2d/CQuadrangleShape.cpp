#include "iqt2d/CQuadrangleShape.h"


// ACF includes
#include "istd/TChangeNotifier.h"


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
	connect(&m_firstDiagGrip1, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnFirstDiagGrip1Changed(const i2d::CVector2d&)));
	connect(&m_firstDiagGrip2, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnFirstDiagGrip2Changed(const i2d::CVector2d&)));
	connect(&m_secondDiagGrip1, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnSecondDiagGrip1Changed(const i2d::CVector2d&)));
	connect(&m_secondDiagGrip2, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnSecondDiagGrip2Changed(const i2d::CVector2d&)));
	
	m_firstDiagGrip2.SetLabelPosition(CGripShape::LabelBottom);
	m_secondDiagGrip2.SetLabelPosition(CGripShape::LabelBottom);

	if (!isEditable){
		m_firstDiagGrip1.setParentItem(NULL);
		m_firstDiagGrip2.setParentItem(NULL);
		m_secondDiagGrip1.setParentItem(NULL);
		m_secondDiagGrip2.setParentItem(NULL);
	}
}


// protected slots

void CQuadrangleShape::OnFirstDiagGrip1Changed(const i2d::CVector2d& point)
{
	i2d::CQuadrangle* quadranglePtr = GetObjectPtr();
	if (quadranglePtr != NULL){
		i2d::CQuadrangle orginalQuadrangle = *quadranglePtr;
		bool wasValid = quadranglePtr->IsQuadrangleValid();

		i2d::CLine2d firstDiag = quadranglePtr->GetFirstDiagonal();
		firstDiag.SetPoint1(point);

		quadranglePtr->SetFirstDiagonal(firstDiag);

		// restore valid quadrangle:
		if (wasValid  && !quadranglePtr->IsQuadrangleValid()){
			*quadranglePtr = orginalQuadrangle;
		}
	}
}


void CQuadrangleShape::OnFirstDiagGrip2Changed(const i2d::CVector2d& point)
{
	i2d::CQuadrangle* quadranglePtr = GetObjectPtr();
	if (quadranglePtr != NULL){
		i2d::CQuadrangle orginalQuadrangle = *quadranglePtr;
		bool wasValid = quadranglePtr->IsQuadrangleValid();

		i2d::CLine2d firstDiag = quadranglePtr->GetFirstDiagonal();
		firstDiag.SetPoint2(point);

		quadranglePtr->SetFirstDiagonal(firstDiag);

		// restore valid quadrangle:
		if (wasValid && !quadranglePtr->IsQuadrangleValid()){
			*quadranglePtr = orginalQuadrangle;
		}
	}
}


void CQuadrangleShape::OnSecondDiagGrip1Changed(const i2d::CVector2d& point)
{
	i2d::CQuadrangle* quadranglePtr = GetObjectPtr();
	if (quadranglePtr != NULL){
		i2d::CQuadrangle orginalQuadrangle = *quadranglePtr;
		bool wasValid = quadranglePtr->IsQuadrangleValid();

		i2d::CLine2d secondDiag = quadranglePtr->GetSecondDiagonal();
		secondDiag.SetPoint1(point);

		quadranglePtr->SetSecondDiagonal(secondDiag);

		// restore valid quadrangle:
		if (wasValid && !quadranglePtr->IsQuadrangleValid()){
			*quadranglePtr = orginalQuadrangle;
		}
	}
}


void CQuadrangleShape::OnSecondDiagGrip2Changed(const i2d::CVector2d& point)
{
	i2d::CQuadrangle* quadranglePtr = GetObjectPtr();
	if (quadranglePtr != NULL){
		i2d::CQuadrangle orginalQuadrangle = *quadranglePtr;
		bool wasValid = quadranglePtr->IsQuadrangleValid();

		i2d::CLine2d secondDiag = quadranglePtr->GetSecondDiagonal();
		secondDiag.SetPoint2(point);

		quadranglePtr->SetSecondDiagonal(secondDiag);

		// restore valid quadrangle:
		if (wasValid && !quadranglePtr->IsQuadrangleValid()){
			*quadranglePtr = orginalQuadrangle;
		}
	}
}


// protected methods

// reimplemented (iqt2d::TObjectShapeBase)

void CQuadrangleShape::UpdateGraphicsItem(const i2d::CQuadrangle& quadrangle)
{
	i2d::CLine2d  firstDiag = quadrangle.GetFirstDiagonal();
	i2d::CLine2d  secondDiag = quadrangle.GetSecondDiagonal();

	m_firstDiagGrip1.SetPosition(firstDiag.GetPoint1());
	m_firstDiagGrip2.SetPosition(firstDiag.GetPoint2());
	m_secondDiagGrip1.SetPosition(secondDiag.GetPoint1());
	m_secondDiagGrip2.SetPosition(secondDiag.GetPoint2());

	QPainterPath path;

	if (!quadrangle.IsQuadrangleValid()){
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

	path.moveTo(GetLocalFromPos(firstDiag.GetPoint1()));
	path.lineTo(GetLocalFromPos(secondDiag.GetPoint1()));
	path.lineTo(GetLocalFromPos(firstDiag.GetPoint2()));
	path.lineTo(GetLocalFromPos(secondDiag.GetPoint2()));
	path.lineTo(GetLocalFromPos(firstDiag.GetPoint1()));

	setPath(path);
}


} // namespace iqt2d


