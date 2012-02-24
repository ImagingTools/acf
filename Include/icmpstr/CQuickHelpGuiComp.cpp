#include "icmpstr/CQuickHelpGuiComp.h"


// Qt includes
#include <QProcess>
#include <QDir>
#include <QFileInfo>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>

#include "iqt/CSignalBlocker.h"
#include "iqt/CFileSystem.h"


namespace icmpstr
{


// reimplemented (idoc::IHelpInfoProvider)

double CQuickHelpGuiComp::GetHelpQuality(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) const
{
	if (m_descriptionFileProviderCompPtr.IsValid()){
		return m_descriptionFileProviderCompPtr->GetHelpQuality(contextText, contextObjectPtr);
	}

	return 0;
}


// reimplemented (idoc::IHelpViewer)

void CQuickHelpGuiComp::ShowHelp(const QString& contextText, const istd::IPolymorphic* contextObjectPtr)
{
	if (!IsGuiCreated()){
		return;
	}

	iqt::CSignalBlocker blocker(GetQtWidget());

	m_descrFilePath = "";

	bool hasDescrFile = false;
	if (m_descriptionFileProviderCompPtr.IsValid()){
		m_descrFilePath = m_descriptionFileProviderCompPtr->GetHelpFilePath(contextText, contextObjectPtr);

		hasDescrFile = (m_descriptionFileProviderCompPtr->GetHelpQuality(contextText, contextObjectPtr) > 0);
		if (hasDescrFile){
			QUrl url = QUrl::fromLocalFile(m_descrFilePath);

			DescriptionBrowser->setSource(url);
		}
	}

	bool isEditAvailable =
				!m_descrFilePath.isEmpty() &&
				m_docuEditorFileParamsCompPtr.IsValid() &&
				!m_docuEditorFileParamsCompPtr->GetPath().isEmpty();
	NewButton->setVisible(isEditAvailable);
	EditButton->setVisible(isEditAvailable);
	EditButton->setEnabled(!m_descrFilePath.isEmpty() && QFileInfo(m_descrFilePath).isWritable());

	DescriptionFrame->setVisible(hasDescrFile);
	NoDescriptionFrame->setVisible(!hasDescrFile);

	m_techFilePath = "";

	if (m_metaInfoManagerCompPtr.IsValid()){
		const icomp::CComponentAddress* addressPtr = dynamic_cast<const icomp::CComponentAddress*>(contextObjectPtr);
		if (addressPtr != NULL){
			const icomp::IComponentStaticInfo* infoPtr = m_metaInfoManagerCompPtr->GetComponentMetaInfo(*addressPtr);
			if (infoPtr != NULL){
				istd::CClassInfo classInfo(*infoPtr);

				while (classInfo.IsTemplateClass()){
					classInfo = classInfo.GetTemplateParam();
				}

				if (m_externalMetaInfoManagerCompPtr.IsValid()){
					QString infoPath = m_externalMetaInfoManagerCompPtr->GetPackageInfoPath(addressPtr->GetPackageId());
					if (!infoPath.isEmpty() && (classInfo.GetName() != "icomp::CCompositeComponentStaticInfo")){
						QDir infoDir(infoPath);
						QFile file(infoDir.absoluteFilePath("CompositorInfo.xml"));
						QDomDocument domDocument;
						if (file.open(QIODevice::ReadOnly) && domDocument.setContent(&file)){
							QDomElement root = domDocument.firstChildElement("CompositorInfo");

							if (root.isElement()){
								QString doxygenPath = iqt::CFileSystem::GetEnrolledPath(root.firstChildElement("DoxyGenPath").text().trimmed());
								if (!doxygenPath.isEmpty()){
									QDir doxygenDir(infoDir.absoluteFilePath(doxygenPath));
									QString doxyFileName = CalcDoxygenFileName(classInfo);
									QFileInfo fileInfo(doxygenDir.absoluteFilePath(doxyFileName));
									if (fileInfo.exists()){
										m_techFilePath = fileInfo.absoluteFilePath();
									}
								}
							}
						}
					}
				}
			}
		}
	}

	ShowTechButton->setVisible(!m_techFilePath.isEmpty());
}


// protected methods

QString CQuickHelpGuiComp::CalcDoxygenFileName(const istd::CClassInfo& classInfo) const
{
	QString retVal = "class";

	std::string className = classInfo.GetName();
	for (		std::string::const_iterator iter = className.begin();
				iter != className.end();
				++iter){
		char c = *iter;
		if (c == ':'){
			retVal += "_1";
		}
		else if ((c >= 'A') && (c <= 'Z')){
			retVal += "_";
			retVal.push_back(c - 'A' + 'a');
		}
		else{
			retVal.push_back(c);
		}
	}

	retVal += ".html";

	return retVal;
}


// reimplemented (CGuiComponentBase)

void CQuickHelpGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	DescriptionFrame->setVisible(false);
	NoDescriptionFrame->setVisible(false);
	ShowTechButton->setVisible(false);
}


// protected slots

void CQuickHelpGuiComp::on_EditButton_clicked()
{
	if (!m_docuEditorFileParamsCompPtr.IsValid()){
		return;
	}

	QString editorPath = m_docuEditorFileParamsCompPtr->GetPath();
	
	if (editorPath.isEmpty()){
		return;
	}

	QStringList parameters;
	parameters << m_descrFilePath;

	QProcess::startDetached(editorPath, parameters);
}


void CQuickHelpGuiComp::on_NewButton_clicked()
{
	on_EditButton_clicked();
}


void CQuickHelpGuiComp::on_ShowTechButton_clicked()
{
	if (!m_techDocuViewerFileParamsCompPtr.IsValid()){
		return;
	}

	QString editorPath = m_techDocuViewerFileParamsCompPtr->GetPath();
	
	if (editorPath.isEmpty()){
		return;
	}

	QStringList parameters;
	parameters << "file://" + m_techFilePath;

	QProcess::startDetached(editorPath, parameters);
}


} // namespace icmpstr


