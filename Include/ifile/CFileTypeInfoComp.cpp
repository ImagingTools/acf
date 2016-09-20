#include <ifile/CFileTypeInfoComp.h>


// Qt includes
#include <QtCore/QStringList>


namespace ifile
{


// reimplemented (ifile::IFileTypeInfo)

bool CFileTypeInfoComp::GetFileExtensions(QStringList& result, const istd::IChangeable* /*dataObjectPtr*/, int /*flags*/, bool doAppend) const
{
	if (m_fileExtensionsAttrPtr.IsValid()){
		if (!doAppend){
			result.clear();
		}

		int extensionsCount = m_fileExtensionsAttrPtr.GetCount();
		for (int i = 0; i < extensionsCount; ++i){
			const QString& extension = m_fileExtensionsAttrPtr[i];

			result.push_back(extension);
		}

#ifdef Q_OS_WIN
		extensionsCount = m_fileExtensionsWinAttrPtr.GetCount();
		for (int i = 0; i < extensionsCount; ++i){
			const QString& extension = m_fileExtensionsWinAttrPtr[i];

			result.push_back(extension);
		}
#endif // Q_OS_WIN

#ifdef Q_OS_MAC
		extensionsCount = m_fileExtensionsMacAttrPtr.GetCount();
		for (int i = 0; i < extensionsCount; ++i){
			const QString& extension = m_fileExtensionsMacAttrPtr[i];

			result.push_back(extension);
		}
#else
#ifdef Q_OS_UNIX
		extensionsCount = m_fileExtensionsUnxAttrPtr.GetCount();
		for (int i = 0; i < extensionsCount; ++i){
			const QString& extension = m_fileExtensionsUnxAttrPtr[i];

			result.push_back(extension);
		}
#endif // Q_OS_UNIX
#endif // Q_OS_MAC

		return true;
	}
	else{
		return false;
	}
}


QString CFileTypeInfoComp::GetTypeDescription(const QString* extensionPtr) const
{
	if (extensionPtr != NULL){
		int extensionsCount = qMin(m_fileExtensionsAttrPtr.GetCount(), m_typeDescriptionsAttrPtr.GetCount());
		for (int extIndex = 0; extIndex < extensionsCount; extIndex++){
			if (m_fileExtensionsAttrPtr[extIndex] == *extensionPtr){
				return m_typeDescriptionsAttrPtr[extIndex];
			}
		}

#ifdef Q_OS_WIN
		extensionsCount = qMin(m_fileExtensionsWinAttrPtr.GetCount(), m_typeDescriptionsWinAttrPtr.GetCount());
		for (int extIndex = 0; extIndex < extensionsCount; extIndex++){
			if (m_fileExtensionsWinAttrPtr[extIndex] == *extensionPtr){
				return m_typeDescriptionsWinAttrPtr[extIndex];
			}
		}
#endif // Q_OS_WIN

#ifdef Q_OS_MAC
		extensionsCount = qMin(m_fileExtensionsMacAttrPtr.GetCount(), m_typeDescriptionsMacAttrPtr.GetCount());
		for (int extIndex = 0; extIndex < extensionsCount; extIndex++){
			if (m_fileExtensionsMacAttrPtr[extIndex] == *extensionPtr){
				return m_typeDescriptionsMacAttrPtr[extIndex];
			}
		}
#else
#ifdef Q_OS_UNIX
		extensionsCount = qMin(m_fileExtensionsUnxAttrPtr.GetCount(), m_typeDescriptionsUnxAttrPtr.GetCount());
		for (int extIndex = 0; extIndex < extensionsCount; extIndex++){
			if (m_fileExtensionsUnxAttrPtr[extIndex] == *extensionPtr){
				return m_typeDescriptionsUnxAttrPtr[extIndex];
			}
		}
#endif // Q_OS_UNIX
#endif // Q_OS_MAC
	}

	if (m_typeDescriptionsAttrPtr.GetCount() > 0){
		return m_typeDescriptionsAttrPtr[0];
	}

#ifdef Q_OS_WIN
	if (m_typeDescriptionsWinAttrPtr.GetCount() > 0){
		return m_typeDescriptionsWinAttrPtr[0];
	}
#endif // Q_OS_WIN

#ifdef Q_OS_MAC
	if (m_typeDescriptionsMacAttrPtr.GetCount() > 0){
		return m_typeDescriptionsMacAttrPtr[0];
	}
#else
#ifdef Q_OS_UNIX
	if (m_typeDescriptionsUnxAttrPtr.GetCount() > 0){
		return m_typeDescriptionsUnxAttrPtr[0];
	}
#endif // Q_OS_UNIX
#endif // Q_OS_MAC
	return "";
}


} // namespace ifile


