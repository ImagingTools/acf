#include <i2d/CTubeNode.h>


// ACF includes
#include <istd/CChangeNotifier.h>

#include <iser/CPrimitiveTypesSerializer.h>


namespace i2d
{


// public methods

CTubeNode::CTubeNode()
	:m_tubeRange(-10, 10)
{
}


CTubeNode::CTubeNode(const CTubeNode& node)
:	m_tubeRange(node.m_tubeRange)
{
}


const istd::CRange& CTubeNode::GetTubeRange() const
{
	return m_tubeRange;
}


void CTubeNode::SetTubeRange(const istd::CRange& tubeRange)
{
	if (m_tubeRange != tubeRange){
		istd::CChangeNotifier changePtr(this);

		m_tubeRange = tubeRange;
	}
}


void CTubeNode::SetInterpolated(
			const CTubeNode& first,
			const CTubeNode& second,
			double alpha)
{
	istd::CChangeNotifier changePtr(this);

	m_tubeRange.SetInterpolated(first.m_tubeRange, second.m_tubeRange, alpha);
}


// reimplemented (qcom::ISerializable)

bool CTubeNode::Serialize(iser::IArchive& archive)
{
	return iser::CPrimitiveTypesSerializer::SerializeRange(archive, m_tubeRange);
}


} // namespace i2d


