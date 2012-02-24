#ifndef icmpstr_CQuickHelpGuiComp_included
#define icmpstr_CQuickHelpGuiComp_included


// Qt includes
#include <QTextBrowser>


#include "icomp/IMetaInfoManager.h"

#include "iprm/IFileNameParam.h"

#include "idoc/IHelpViewer.h"
#include "idoc/IHelpFileProvider.h"

#include "iqtgui/TDesignerGuiCompBase.h"

#include "icmpstr/IExternalMetaInfoManager.h"

#include "Generated/ui_CQuickHelpGuiComp.h"


namespace icmpstr
{


class CQuickHelpGuiComp:
			public iqtgui::TDesignerGuiCompBase<Ui::CQuickHelpGuiComp>,
			virtual public idoc::IHelpViewer
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CQuickHelpGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CQuickHelpGuiComp);
		I_REGISTER_INTERFACE(idoc::IHelpInfoProvider);
		I_REGISTER_INTERFACE(idoc::IHelpViewer);
		I_ASSIGN(m_descriptionFileProviderCompPtr, "HelpFileProvider", "Calculate path of html document for short description", true, "HelpFileProvider");
		I_ASSIGN(m_docuEditorFileParamsCompPtr, "DocuEditorFileParam", "Object storing path to html editor used to edit the description", true, "DocuEditorFileParam");
		I_ASSIGN(m_techDocuViewerFileParamsCompPtr, "TechDocuViewerParam", "Object storing path to html viewer used to show technical documentation", true, "TechDocuViewerParam");
		I_ASSIGN(m_metaInfoManagerCompPtr, "MetaInfoManager", "Allows access to component meta information", true, "MetaInfoManager");
		I_ASSIGN_TO(m_externalMetaInfoManagerCompPtr, m_metaInfoManagerCompPtr, false);
	I_END_COMPONENT;

	// reimplemented (idoc::IHelpInfoProvider)
	virtual double GetHelpQuality(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) const;

	// reimplemented (idoc::IHelpViewer)
	virtual void ShowHelp(const QString& contextText, const istd::IPolymorphic* contextObjectPtr);

protected:
	QString CalcDoxygenFileName(const istd::CClassInfo& classInfo) const;

	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

protected Q_SLOTS:
	void on_EditButton_clicked();
	void on_NewButton_clicked();
	void on_ShowTechButton_clicked();

private:
	I_REF(idoc::IHelpFileProvider, m_descriptionFileProviderCompPtr);
	I_REF(iprm::IFileNameParam, m_docuEditorFileParamsCompPtr);
	I_REF(iprm::IFileNameParam, m_techDocuViewerFileParamsCompPtr);
	I_REF(icomp::IMetaInfoManager, m_metaInfoManagerCompPtr);
	I_REF(IExternalMetaInfoManager, m_externalMetaInfoManagerCompPtr);

	QString m_descrFilePath;
	QString m_techFilePath;
};


} // namespace icmpstr


#endif // !icmpstr_CQuickHelpGuiComp_included


