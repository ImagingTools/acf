#pragma once


// ACF includes
#include <icomp/CComponentBase.h>
#include <ifile/IFileNameParam.h>
#include <idoc/IHelpFileProvider.h>


namespace iqtdoc
{


class CStaticHelpFileProviderComp:
			public icomp::CComponentBase,
			virtual public idoc::IHelpFileProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CStaticHelpFileProviderComp);
		I_REGISTER_INTERFACE(idoc::IHelpFileProvider);
		I_ASSIGN(m_helpFileDirCompPtr, "HelpFileDir", "Directory where help files are stored", true, "HelpFileDir");
		I_ASSIGN(m_defaultFileNameAttrPtr, "DefaultHelpFileName", "Help file name used if help without context is asked", true, "GeneralHelp.html");
		I_ASSIGN(m_contextPrefixAttrPtr, "ContextPrefix", "File name prefix used for context based help file name", true, "");
		I_ASSIGN(m_contextSuffixAttrPtr, "ContextSuffix", "File name suffix added to context based help file name", true, ".html");
	I_END_COMPONENT;

	// reimplemented (idoc::IHelpFileProvider)
	virtual double GetHelpQuality(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) const override;
	virtual QString GetHelpFilePath(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) const override;

private:
	I_REF(ifile::IFileNameParam, m_helpFileDirCompPtr);
	I_ATTR(QString, m_defaultFileNameAttrPtr);
	I_ATTR(QString, m_contextPrefixAttrPtr);
	I_ATTR(QString, m_contextSuffixAttrPtr);
};


} // namespace iqtdoc


