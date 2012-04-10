#ifndef iqtproc_CDocumentProcessingManagerComp_included
#define iqtproc_CDocumentProcessingManagerComp_included


// ACF includes
#include "iqtproc/CDocumentProcessingManagerCompBase.h"


namespace iqtproc
{


/**
	Component to trigger document-to-document processing action.

	You can use \c m_inPlaceProcessingAttrPtr to define, 
	weither the new document is created for the processing result or the document is processed "in-place".
*/
class CDocumentProcessingManagerComp: public iqtproc::CDocumentProcessingManagerCompBase
{
public:
	typedef iqtproc::CDocumentProcessingManagerCompBase BaseClass;
	
	I_BEGIN_COMPONENT(CDocumentProcessingManagerComp);
		I_ASSIGN(m_inPlaceProcessingAttrPtr, "InPlaceProcessing", "If enabled, the input document will be the result of processing", false, false);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtproc::CDocumentProcessingManagerCompBase)
	void DoDocumentProcessing(const istd::IChangeable* inputDocumentPtr, const QByteArray& documentTypeId);

private:
	void DoProcessingToOutput(const istd::IChangeable* inputDocumentPtr, const QByteArray& documentTypeId);
	void DoInPlaceProcessing(istd::IChangeable* inputDocumentPtr);

private:
	I_ATTR(bool, m_inPlaceProcessingAttrPtr);
};


} // namespace iproc


#endif // !iqtproc_CDocumentProcessingManagerComp_included
