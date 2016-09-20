#include <iqtdoc/CStaticHelpFileProviderComp.h>


// Qt includes
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

// ACF includes
#include <iqt/iqt.h>


namespace iqtdoc
{


// reimplemented (idoc::IHelpFileProvider)

double CStaticHelpFileProviderComp::GetHelpQuality(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) const
{
	QString filePath = GetHelpFilePath(contextText, contextObjectPtr);
	if (filePath.isEmpty()){
		return 0;
	}

	QFileInfo fileInfo(filePath);
	if (fileInfo.isFile()){
		return 1;
	}
	else{
		return 0;
	}
}


QString CStaticHelpFileProviderComp::GetHelpFilePath(const QString& contextText, const istd::IPolymorphic* /*contextObjectPtr*/) const
{
	if (!m_helpFileDirCompPtr.IsValid()){
		return QString();
	}

	QDir helpDir(m_helpFileDirCompPtr->GetPath());
	QString fileName;
	if (contextText.isEmpty()){
		fileName = *m_defaultFileNameAttrPtr;
	}
	else{
		fileName = *m_contextPrefixAttrPtr + contextText + *m_contextSuffixAttrPtr;
	}

	return helpDir.absoluteFilePath(fileName);
}


} // namespace iqtdoc


