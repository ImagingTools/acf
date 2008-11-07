#ifndef CRegistryLoaderComp_included
#define CRegistryLoaderComp_included


#include "ibase/TLoggerCompWrap.h"

#include "BasePck/BasePck.h"


/**
	Special registry loader supporting of loading layout data.
*/
class CRegistryLoaderComp: public ibase::TLoggerCompWrap<BasePck::XmlFileSerializer>
{
public:
	typedef ibase::TLoggerCompWrap<BasePck::XmlFileSerializer> BaseClass;

	enum MessageId
	{
		MI_LOAD_ERROR = 680,
		MI_CANNOT_READ_LAYOUT
	};

	// reimplemented (iser::IFileLoader)
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const;
	virtual bool GetFileExtensions(istd::CStringList& result, bool doAppend = false) const;

protected:
	istd::CString GetLayoutPath(const istd::CString& registryPath) const;
};


#endif // !CRegistryLoaderComp_included


