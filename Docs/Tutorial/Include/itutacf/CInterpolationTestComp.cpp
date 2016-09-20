#include <itutacf/CInterpolationTestComp.h>


// ACF inlcudes
#include <istd/CChangeNotifier.h>
#include <istd/TRange.h>
#include <imod/IModel.h>
#include <imath/CLinearInterpolator.h>
#include <imath/CAkimaInterpolator.h>


namespace itutacf
{


// protected methods

// reimplemented (imod::CSingleModelObserverBase)

void CInterpolationTestComp::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	if (!m_slaveObjectCompPtr.IsValid()){
		return;
	}

	istd::CChangeNotifier notifier(m_slaveObjectCompPtr.GetPtr());

	m_slaveObjectCompPtr->Clear();

	const i2d::CPolygon* objectPtr = GetObservedObject();

	if (objectPtr != NULL){
		QMap<double, double> positionMap;
		int nodesCount = objectPtr->GetNodesCount();

		for (int nodeIndex = 0; nodeIndex < nodesCount; ++nodeIndex){
			const i2d::CVector2d& node = objectPtr->GetNode(nodeIndex);
			positionMap[node.GetX()] = node.GetY();
		}

		QVector<double> positions = positionMap.keys().toVector();
		QVector<double> values = positionMap.values().toVector();
		Q_ASSERT(positions.size() == values.size());

		if (positions.size() >= 2){
			istd::TDelPtr<imath::IDoubleFunction> interpolationFunctionPtr;

			switch (*m_interpolatorTypeAttrPtr){
			case 0:
				interpolationFunctionPtr = new imath::CLinearInterpolator(&positions[0], &values[0], positions.size());
				break;

			case 1:
				interpolationFunctionPtr = new imath::CAkimaInterpolator(&positions[0], &values[0], positions.size());
				break;

			default:
				break;
			}

			if (interpolationFunctionPtr.IsValid()){
				istd::CRange positionRange(positions.first(), positions.last());

				int interpolatedNodesCount = *m_interpolatedNodesCountAttrPtr;

				for (int interIndex = 0; interIndex < interpolatedNodesCount; ++interIndex){
					double position = positionRange.GetValueFromAlpha(double(interIndex) / (interpolatedNodesCount - 1));
					double interpolatedValue = interpolationFunctionPtr->GetValueAt(position);

					m_slaveObjectCompPtr->InsertNode(i2d::CVector2d(position, interpolatedValue));
				}
			}
		}
	}
}


} // namespace itutacf


