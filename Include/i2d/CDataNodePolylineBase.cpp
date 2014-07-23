#include "i2d/CDataNodePolylineBase.h"


// ACF includes
#include "istd/CChangeNotifier.h"

#include "iser/IArchive.h"


namespace i2d
{


// reimplemented (iser::ISerializable)

bool CDataNodePolylineBase::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag nodesDataTag("UserData", "User data stored in the nodes of polyline", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag nodeDataTag("Node", "Data stored in the polyline node", iser::CArchiveTag::TT_GROUP, &nodesDataTag);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = BaseClass::Serialize(archive);

	// static attributes
	if (retVal){
		int nodesCount = GetNodesCount();
		retVal = retVal && archive.BeginMultiTag(nodesDataTag, nodeDataTag, nodesCount);

		if (!archive.IsStoring()){
			SetNodesCount(nodesCount);
		}

		for (int nodeIndex = 0; nodeIndex < nodesCount; ++nodeIndex){
			iser::ISerializable& nodeData = GetNodeDataRef(nodeIndex);

			retVal = retVal && archive.BeginTag(nodeDataTag);
			retVal = retVal && nodeData.Serialize(archive);
			retVal = retVal && archive.EndTag(nodeDataTag);
		}
		retVal = retVal && archive.EndTag(nodesDataTag);
	}

	return retVal;
}


} // namespace i2d


