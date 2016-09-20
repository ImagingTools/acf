#ifndef i2d_CDataNodePolylineBase_included
#define i2d_CDataNodePolylineBase_included


// ACF includes
#include <i2d/CPolyline.h>


namespace i2d
{


/**
	Base class for polylines with additional data stored in each node.
*/
class CDataNodePolylineBase: public CPolyline
{
public:
	typedef CPolyline BaseClass;

	/**
		Get user data from the given node.
	*/
	virtual const iser::ISerializable& GetNodeData(int nodeIndex) const = 0;

	/**
		\overload
	*/
	virtual iser::ISerializable& GetNodeDataRef(int nodeIndex) = 0;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
};


} // namespace i2d


#endif // !i2d_CDataNodePolylineBase_included

