#include "icmpstr/CQuickHelpViewerComp.h"


// Qt includes
#include <QProcess>
#include <QDir>
#include <QFileInfo>
#include <QDomDocument>
#include <QDomNode>

#include "iqt/CSignalBlocker.h"
#include "iqt/CFileSystem.h"


namespace icmpstr
{


// reimplemented (idoc::IHelpViewer)

void CQuickHelpViewerComp::ShowHelp(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr)
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
			QUrl url = QUrl::fromLocalFile(iqt::GetQString(m_descrFilePath));

			DescriptionBrowser->setSource(url);
		}
	}

	bool isEditAvailable =
				!m_descrFilePath.IsEmpty() &&
				m_docuEditorFileParamsCompPtr.IsValid() &&
				!m_docuEditorFileParamsCompPtr->GetPath().IsEmpty();
	NewButton->setVisible(isEditAvailable);
	EditButton->setVisible(isEditAvailable);
	EditButton->setEnabled(!m_descrFilePath.IsEmpty() && QFileInfo(iqt::GetQString(m_descrFilePath)).isWritable());

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
					istd::CString infoPath = m_externalMetaInfoManagerCompPtr->GetPackageInfoPath(addressPtr->GetPackageId());
					if (!infoPath.IsEmpty() && (classInfo.GetName() != "icomp::CCompositeComponentStaticInfo")){
						QDir infoDir(iqt::GetQString(infoPath));
						QFile file(infoDir.absoluteFilePath("CompositorInfo.xml"));
						QDomDocument domDocument;
						if (file.open(QIODevice::ReadOnly) && domDocument.setContent(&file)){
							QDomElement root = domDocument.firstChildElement("CompositorInfo");

							if (root.isElement()){
								QString doxygenPath = iqt::CFileSystem::GetEnrolledPath(root.firstChildElement("DoxyGenPath").text().trimmed());
								if (!doxygenPath.isEmpty()){
									QDir doxygenDir(infoDir.absoluteFilePath(doxygenPath));
									istd::CString doxyFileName = CalcDoxygenFileName(classInfo);
									QFileInfo fileInfo(doxygenDir.absoluteFilePath(iqt::GetQString(doxyFileName)));
									if (fileInfo.exists()){
										m_techFilePath = iqt::GetCString(fileInfo.absoluteFilePath());
									}
								}
							}
						}
					}
				}
			}
		}
	}

	ShowTechButton->setVisible(!m_techFilePath.IsEmpty());
}


// protected methods

istd::CString CQuickHelpViewerComp::CalcDoxygenFileName(const istd::CClassInfo& classInfo) const
{
	istd::CString retVal = "class";

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

void CQuickHelpViewerComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	DescriptionFrame->setVisible(false);
	NoDescriptionFrame->setVisible(false);
	ShowTechButton->setVisible(false);
}


// protected slots

void CQuickHelpViewerComp::on_EditButton_clicked()
{
	if (!m_docuEditorFileParamsCompPtr.IsValid()){
		return;
	}

	istd::CString editorPath = m_docuEditorFileParamsCompPtr->GetPath();
	
	if (editorPath.IsEmpty()){
		return;
	}

	QStringList parameters;
	parameters << iqt::GetQString(m_descrFilePath);

	QProcess::startDetached(iqt::GetQString(editorPath), parameters);
}


void CQuickHelpViewerComp::on_NewButton_clicked()
{
	on_EditButton_clicked();
}


void CQuickHelpViewerComp::on_ShowTechButton_clicked()
{
	if (!m_techDocuViewerFileParamsCompPtr.IsValid()){
		return;
	}

	istd::CString editorPath = m_techDocuViewerFileParamsCompPtr->GetPath();
	
	if (editorPath.IsEmpty()){
		return;
	}

	QStringList parameters;
	parameters << "file://" + iqt::GetQString(m_techFilePath);

	QProcess::startDetached(iqt::GetQString(editorPath), parameters);
}


} // namespace icmpstr


