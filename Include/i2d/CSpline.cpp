#include <i2d/CSpline.h>


// ACF includes
#include <i2d/CSplineSegment.h>
#include <istd/TDelPtr.h>
#include <istd/CChangeNotifier.h>

namespace i2d
{


CSpline::CSpline()
{
	m_isInvalidated = false;
}


int CSpline::GetSegmentCount() const
{
	return GetNodesCount() - 1;
}


const CSplineSegment& CSpline::GetSplineSegment(int index) const
{
	if (m_isInvalidated){
		CalcAllSegments();
	}

	return m_segments[index];
}


bool CSpline::InsertSplineSegment(const CSplineSegment& segment)
{
	istd::CChangeNotifier changeNotifier(this);
	Q_UNUSED(changeNotifier);

	m_segments.push_back(segment);

	return true;
}


// reimplemented (istd::IChangeable)

int CSpline::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE;
}


bool CSpline::CopyFrom(const IChangeable& object, CompatibilityMode /*mode*/)
{
	const CSpline* splinePtr = dynamic_cast<const CSpline*>(&object);

	if (splinePtr != NULL){	
		istd::CChangeNotifier changeNotifier(this);
		Q_UNUSED(changeNotifier);

		BaseClass::CopyFrom(object);

		m_segments = splinePtr->m_segments;

		return true;
	}

	return false;
}


istd::IChangeable* CSpline::CloneMe(CompatibilityMode mode) const 
{
	istd::TDelPtr<CSpline> clonePtr(new CSpline);

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


void CSpline::CalcAllSegments() const
{
	double* coeffX;
	double* coeffY;
	int i;
	int nodesCount = GetNodesCount();
	m_segments.clear();
	m_segments.resize(nodesCount);

	coeffX = new double[nodesCount];
	coeffY = new double[nodesCount];
	if ((coeffX != NULL) && (coeffY != NULL)){
		for (i = 1; i < nodesCount - 1; i++){
			coeffX[i] = (GetNodePos(i + 1).GetX() + GetNodePos(i - 1).GetX() - 2 * GetNodePos(i).GetX()) / 2;
			coeffY[i] = (GetNodePos(i + 1).GetY() + GetNodePos(i - 1).GetY() - 2 * GetNodePos(i).GetY()) / 2;
		}
		coeffX[nodesCount - 1] = coeffX[0] = 0;
		coeffY[nodesCount - 1] = coeffY[0] = 0;
		
		SolveOpen(coeffX, nodesCount);
		SolveOpen(coeffY, nodesCount);

		for (i = 0; i < nodesCount - 1; i++){
			CSplineSegment& segment = m_segments[i];
			segment.D(GetNodePos(i));
			segment.C(GetNodePos(i + 1) - GetNodePos(i) - i2d::CVector2d(coeffX[i + 1] + 2 * coeffX[i], coeffY[i + 1] + 2 * coeffY[i]));
			segment.B(i2d::CVector2d(3 * coeffX[i], 3 * coeffY[i]));
			segment.A(i2d::CVector2d(coeffX[i + 1] - coeffX[i], coeffY[i + 1] - coeffY[i]));
		}
	}
	delete[] coeffX;
	delete[] coeffY;

	m_isInvalidated = false;
}


// private methods

void CSpline::SolveOpen(double* d,int count)
{
	int i;

	for (i = 2; i < count - 1; i++){
		d[i] -=d [i-1] / 4;
	}

	d[count - 2] = (d[count - 2] * 8) / 15;

	for (i = count - 3; i > 1; i--){
		d[i] = ((d[i] - d[i + 1] * 0.5) * 8) / 15;
	}

	d[1] = (d[1] - d[2] * 0.5) / 2;

	return;
}


} // namespace i2d


