#include <ifile/CFileNameParamComp.h>


#if QT_VERSION >= 0x050000
#include <QtCore/QStandardPaths>
#else
#include <QtGui/QDesktopServices>
#endif

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace ifile
{



// reimplemented (ifile::IFileNameParam)

int CFileNameParamComp::GetPathType() const
{
	Q_ASSERT(m_pathTypeAttrPtr.IsValid());

	return *m_pathTypeAttrPtr;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CFileNameParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultDirAttrPtr.IsValid()){
		m_path = *m_defaultDirAttrPtr;
	}

	if (m_path.contains(s_tempPathVariable)){
		QString tempPath;

#if QT_VERSION < 0x050000
		tempPath = QDesktopServices::storageLocation(QDesktopServices::TempLocation);
#else
		tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
#endif
		m_path.replace(s_tempPathVariable, tempPath);
	}

	if (m_path.contains(s_appNameVariable)){
		QString applicationName = QCoreApplication::applicationName();
	
		m_path.replace(s_appNameVariable, applicationName);
	}

	if (m_path.contains(s_companyNameVariable)){
		QString organizationName = QCoreApplication::organizationName();
	
		m_path.replace(s_companyNameVariable, organizationName);
	}

	if (m_path.contains(s_publicSharedPathVariable)){
#ifdef Q_OS_WIN
		QString publicSharedFolder("C:/Users/Public");
#else
		QString publicSharedFolder("/Users/Shared");
#endif

		m_path.replace(s_publicSharedPathVariable, publicSharedFolder);
	}

	if (m_path.contains(s_userHomePathVariable)){
		QString userHomePath;

#if QT_VERSION < 0x050000
		userHomePath = QDesktopServices::storageLocation(QDesktopServices::HomeLocation);
#else
		userHomePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
#endif
		m_path.replace(s_userHomePathVariable, userHomePath);
	}

	if (m_initialPathCompPtr.IsValid()){
		if (GetPathType() == m_initialPathCompPtr->GetPathType()){
			m_path = m_initialPathCompPtr->GetPath();
		}
	}
}


// private static members

QString CFileNameParamComp::s_tempPathVariable = "$(TempPath)";
QString CFileNameParamComp::s_appNameVariable = "$(AppName)";
QString CFileNameParamComp::s_companyNameVariable = "$(CompanyName)";
QString CFileNameParamComp::s_publicSharedPathVariable = "$(PublicSharedPath)";
QString CFileNameParamComp::s_userHomePathVariable = "$(UserHome)";


} // namespace ifile


