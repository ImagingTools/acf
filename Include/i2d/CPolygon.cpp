#include <i2d/CPolygon.h>


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include <istd/TDelPtr.h>
#include <istd/CChangeNotifier.h>
#include <i2d/CLine2d.h>
#include <i2d/CAffineTransformation2d.h>


namespace i2d
{


static const istd::IChangeable::ChangeSet s_flipPolygonChange(CPolygon::CF_OBJECT_POSITION, CPolygon::CF_ALL_DATA, QObject::tr("Flip"));
static const istd::IChangeable::ChangeSet s_rotatePolygonChange(CPolygon::CF_OBJECT_POSITION, CPolygon::CF_ALL_DATA, QObject::tr("Rotate"));
static const istd::IChangeable::ChangeSet s_reversePolygonChange(CPolygon::CF_OBJECT_POSITION, CPolygon::CF_ALL_DATA, QObject::tr("Reverse nodes"));


// public methods

double CPolygon::GetOutlineLength() const
{
	double length = 0;
	int nodesCount = GetNodesCount();
	if (nodesCount > 0){
		i2d::CLine2d segmentLine;
		segmentLine.SetPoint2(GetNodePos(nodesCount - 1));
		for (int nodeIndex = 0; nodeIndex < nodesCount; ++nodeIndex){
			segmentLine.PushEndPoint(GetNodePos(nodeIndex));
			length += segmentLine.GetLength();
		}
	}

	return length;
}


void CPolygon::FlipByX()
{
	int count = GetNodesCount();
	if (count){
		istd::CChangeNotifier changeNotifier(this, &s_flipPolygonChange);
		Q_UNUSED(changeNotifier);

		i2d::CVector2d center = GetCenter();

		for (int i = 0; i < count; i++){
			i2d::CVector2d& position = GetNodePosRef(i);

			position.SetX(center.GetX() + (center.GetX() - position.GetX()));
		}
	}
}


void CPolygon::FlipByY()
{
	int count = GetNodesCount();
	if (count){
		istd::CChangeNotifier changeNotifier(this, &s_flipPolygonChange);
		Q_UNUSED(changeNotifier);

		i2d::CVector2d center = GetCenter();

		for (int i = 0; i < count; i++){
			i2d::CVector2d& position = GetNodePosRef(i);

			position.SetY(center.GetY() + (center.GetY() - position.GetY()));
		}
	}
}


void CPolygon::Rotate(double radians)
{
	int count = GetNodesCount();
	if (count){
		istd::CChangeNotifier changeNotifier(this, &s_rotatePolygonChange);
		Q_UNUSED(changeNotifier);

		i2d::CVector2d center = GetCenter();

		i2d::CAffineTransformation2d translateTo00;
		translateTo00.Reset(-center);
		i2d::CAffineTransformation2d rotate;
		rotate.Reset(i2d::CVector2d(0, 0), radians);
		i2d::CAffineTransformation2d translateBackToCenter;
		translateBackToCenter.Reset(center);

		Transform(translateTo00);
		Transform(rotate);
		Transform(translateBackToCenter);
	}
}


void CPolygon::ReverseNodes()
{
	int count = GetNodesCount();
	if (count){
		istd::CChangeNotifier changeNotifier(this, &s_reversePolygonChange);
		Q_UNUSED(changeNotifier);

		for (int i = 0; i < count / 2; i++){
			i2d::CVector2d node1 = GetNodePos(i);
			i2d::CVector2d node2 = GetNodePos(count - 1 - i);

			SetNodePos(i, node2);
			SetNodePos(count - 1 - i, node1);
		}
	}
}


} // namespace i2d


