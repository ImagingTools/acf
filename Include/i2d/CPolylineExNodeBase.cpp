#include "i2d/CPolylineExNodeBase.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"


namespace i2d
{


// reimplemented (iser::ISerializable)

bool CPolylineExNodeBase::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag nodesDataTag("NodesExtraData", "Nodes Extra Data");
	static iser::CArchiveTag nodeDataTag("Node", "Node Data");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

	bool retVal = BaseClass::Serialize(archive);

	// static attributes
	if (retVal){
		int nodesCount = GetNodesCount();
		retVal = retVal && archive.BeginTag(nodesDataTag);
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

