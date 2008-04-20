#ifndef idoc_CSerializedDocumentTemplateComp_included
#define idoc_CSerializedDocumentTemplateComp_included


#include "istd/IFileLoader.h"

#include "idoc/CDocumentTemplateCompBase.h"


namespace idoc
{		


class CSerializedDocumentTemplateComp: public idoc::CDocumentTemplateCompBase
{
public:
	typedef idoc::CDocumentTemplateCompBase BaseClass;
	
	I_BEGIN_COMPONENT(CSerializedDocumentTemplateComp)
		I_ASSIGN(m_fileLoaderCompPtr, "DocumentSerializer", "Provide document loading and saving", true, "DocumentSerializer");
	I_END_COMPONENT

	// reimplemented (idoc::IDocumentTemplate)
	virtual bool LoadDocumentFromFile(const istd::CString& filePath, imod::IModel& result) const;
	virtual bool SaveDocumentToFile(const imod::IModel& document, const istd::CString& filePath) const;

private:
	I_REF(istd::IFileLoader, m_fileLoaderCompPtr);
};


} // namespace idoc


#endif // !idoc_CSerializedDocumentTemplateComp_included

