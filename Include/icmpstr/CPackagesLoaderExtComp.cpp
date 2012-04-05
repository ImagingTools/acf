#include "icmpstr/CPackagesLoaderExtComp.h"


namespace icmpstr
{


// reimplemented (iqt::CPackagesLoaderComp)

bool CPackagesLoaderExtComp::RegisterPackagesDir(const QString& path)
{
	bool retVal = BaseClass::RegisterPackagesDir(path);

	QDir packagesDir(path);

	QStringList infoFilters;
	infoFilters.append("*.arp.info");
	QStringList infoFilesInfo = packagesDir.entryList(infoFilters, QDir::Dirs | QDir::NoDotAndDotDot);
	for (		QStringList::iterator iter = infoFilesInfo.begin();
				iter != infoFilesInfo.end();
				++iter){
		QFileInfo fileInfo(*iter);

		std::string packageName(fileInfo.baseName().toStdString());

		QDir componentInfoDir = packagesDir;
		componentInfoDir.cd(*iter);

		m_packageInfosMap[packageName] = componentInfoDir;
	}

	return retVal;
}


// reimplemented (icmpstr::IExternalMetaInfoManager)

QString CPackagesLoaderExtComp::GetPackageInfoPath(const std::string& packageId) const
{
	PackageInfosMap::const_iterator foundPackageIter = m_packageInfosMap.find(packageId);
	if (foundPackageIter != m_packageInfosMap.end()){
		return foundPackageIter.value().absolutePath();
	}

	return "";
}


QString CPackagesLoaderExtComp::GetComponentInfoPath(const icomp::CComponentAddress& address) const
{
	PackageInfosMap::const_iterator foundPackageIter = m_packageInfosMap.find(address.GetPackageId());
	if (foundPackageIter != m_packageInfosMap.end()){
		return foundPackageIter.value().filePath(address.GetComponentId().c_str());
	}

	return "";
}


} // namespace icmpstr


