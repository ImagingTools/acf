#ifndef iqtdoc_CExtendedDocumentTemplateComp_included
#define iqtdoc_CExtendedDocumentTemplateComp_included


// ACF includes
#include <idoc/CSingleDocumentTemplateComp.h>

#include <iqtgui/IGuiObject.h>


namespace iqtdoc
{


/**
	Extension of \c idoc::CSingleDocumentTemplateComp providing additional Qt-specified functionality.
*/
class CExtendedDocumentTemplateComp: public idoc::CSingleDocumentTemplateComp
{
public:
	typedef idoc::CSingleDocumentTemplateComp BaseClass;

	I_BEGIN_COMPONENT(CExtendedDocumentTemplateComp);
		I_ASSIGN(m_viewGuiCompFact, "ViewFactory", "Create of document GUI", true, "ViewFactory");
	I_END_COMPONENT;

	// reimplemented (idoc::IDocumentTemplate)
	virtual Ids GetDocumentTypeIdsForFile(const QString& filePath) const;

protected:
	// reimplemented (idoc::CSingleDocumentTemplateComp)
	virtual istd::IPolymorphic* ExtractViewInterface(icomp::IComponent* componentPtr) const;

private:
	I_FACT(iqtgui::IGuiObject, m_viewGuiCompFact);
};


} // namespace iqtdoc


#endif // !iqtdoc_CExtendedDocumentTemplateComp_included


