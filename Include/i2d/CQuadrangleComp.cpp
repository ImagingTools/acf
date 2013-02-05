#include "i2d/CQuadrangleComp.h"


namespace i2d
{

// protected methods

// reimplemented (icomp::CComponentBase)

void CQuadrangleComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	Q_ASSERT(m_diag1X1AttrPtr.IsValid());
	Q_ASSERT(m_diag1Y1AttrPtr.IsValid());
	Q_ASSERT(m_diag1X2AttrPtr.IsValid());
	Q_ASSERT(m_diag1Y2AttrPtr.IsValid());
	Q_ASSERT(m_diag2X1AttrPtr.IsValid());
	Q_ASSERT(m_diag2Y1AttrPtr.IsValid());
	Q_ASSERT(m_diag2X2AttrPtr.IsValid());
	Q_ASSERT(m_diag2Y2AttrPtr.IsValid());

	i2d::CLine2d firstDiagonal(
				i2d::CVector2d(*m_diag1X1AttrPtr, *m_diag1Y1AttrPtr),
				i2d::CVector2d(*m_diag1X2AttrPtr, *m_diag1Y2AttrPtr));

	i2d::CLine2d secondDiagonal(
				i2d::CVector2d(*m_diag2X1AttrPtr, *m_diag2Y1AttrPtr),
				i2d::CVector2d(*m_diag2X2AttrPtr, *m_diag2Y2AttrPtr));

	SetFirstDiagonal(firstDiagonal);
	SetSecondDiagonal(firstDiagonal);
}


} // namespace i2d


