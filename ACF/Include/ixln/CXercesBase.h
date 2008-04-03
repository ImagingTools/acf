#ifndef ixln_CXercesBase_included
#define ixln_CXercesBase_included


#include <xercesc/dom/DOM.hpp>

#include "iser/CArchiveTag.h"


namespace ixln
{


/**
	Base class for implementations using XERCES library.
*/
class CXercesBase
{
public:
	explicit CXercesBase();

    virtual ~CXercesBase();

protected:
	static void AddXercescRef();
	static void RemXercescRef();

	xercesc::DOMDocument* m_documentPtr;
	xercesc::DOMNode* m_nodePtr;

	static const std::wstring s_text;

private:
	static int s_xercescInstanceCount;
};


} // namespace ixln


#endif // !ixln_CXercesBase_included


