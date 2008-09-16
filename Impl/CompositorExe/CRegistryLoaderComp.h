#ifndef CRegistryLoaderComp_included
#define CRegistryLoaderComp_included


#include "BasePck/BasePck.h"


/**
	Special registry loader supporting of loading layout data.
*/
class CRegistryLoaderComp: public BasePck::XmlFileSerializer
{
public:
	typedef BasePck::XmlFileSerializer BaseClass;

	// reimplemented (iser::IFileLoader)
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const;

protected:
	istd::CString GetLayoutPath(const istd::CString& registryPath) const;
};


#endif // !CRegistryLoaderComp_included


