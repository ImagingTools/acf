#ifndef CRegistryLoaderComp_included
#define CRegistryLoaderComp_included


#include "ibase/TMessageProducerWrap.h"

#include "BasePck/BasePck.h"


/**
	Special registry loader supporting of loading layout data.
*/
class CRegistryLoaderComp: public ibase::TMessageProducerWrap<BasePck::XmlFileSerializer>
{
public:
	typedef ibase::TMessageProducerWrap<BasePck::XmlFileSerializer> BaseClass;

	enum MessageId
	{
		MI_LOAD_ERROR = 680,
		MI_CANNOT_READ_LAYOUT
	};

	// reimplemented (iser::IFileLoader)
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const;

protected:
	istd::CString GetLayoutPath(const istd::CString& registryPath) const;
};


#endif // !CRegistryLoaderComp_included


