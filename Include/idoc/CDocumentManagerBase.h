#ifndef idoc_CDocumentManagerBase_included
#define idoc_CDocumentManagerBase_included


#include "idoc/IDocumentManager.h"


namespace idoc
{


class IDocumentTemplate;


class CDocumentManagerBase: virtual public idoc::IDocumentManager
{
public:
	CDocumentManagerBase();

	void SetDocumentTemplate(const idoc::IDocumentTemplate* documentTemplatePtr);

	// reimplemented (idoc::IDocumentManager)
	virtual int GetAllowedOperationFlags(const istd::IPolymorphic* viewPtr = NULL) const;
	virtual const idoc::IDocumentTemplate* GetDocumentTemplate() const;

private:
	const IDocumentTemplate* m_documentTemplatePtr;

	int m_baseAllowedFlags;
};


} // namespace idoc


#endif // idoc_CDocumentManagerBase_included


