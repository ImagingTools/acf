#include "ixln/CXmlWriteArchiveBase.h"


#include <stdio.h>
#include <sstream>

#include "iser/CArchiveTag.h"


namespace ixln
{


CXmlWriteArchiveBase::~CXmlWriteArchiveBase()
{
	Flush();
	Reset();
}


bool CXmlWriteArchiveBase::IsValid()
{
	I_ASSERT((m_writerPtr != NULL) == (m_documentPtr != NULL));
	I_ASSERT(m_formTargetPtr.IsValid() == (m_documentPtr != NULL));

	return (m_documentPtr != NULL);
}


bool CXmlWriteArchiveBase::Flush()
{
	I_ASSERT(IsValid());

	bool retval = true;
	if (!m_isFlushed){
        try {
			m_writerPtr->writeNode(m_formTargetPtr.GetPtr(), *m_documentPtr);

			m_isFlushed = true;
        }
        catch (...) {
        	retval = false;
        }
    }

	return retval;
}


// reimplemented (iser/IArchive)

bool CXmlWriteArchiveBase::BeginTag(const iser::CArchiveTag& tag)
{
	I_ASSERT(IsValid());

	m_isFirstParamInTag = true;
	m_isFlushed = false;

	if (IsCommentEnabled()){
		std::string fullDescription = "Tag comment: " + tag.GetComment();

	    wchar_t* tagComment = xercesc::XMLString::transcode(fullDescription.c_str());
		xercesc::DOMComment* commentElement = m_documentPtr->createComment(tagComment);
		m_nodePtr->appendChild(commentElement);
		xercesc::XMLString::release(&tagComment);
	}

    wchar_t* tagName = xercesc::XMLString::transcode(tag.GetId().c_str());
    xercesc::DOMElement* element = m_documentPtr->createElement(tagName);
	m_nodePtr->appendChild(element);

	m_nodePtr = element;
	xercesc::XMLString::release(&tagName);

    return true;
}


bool CXmlWriteArchiveBase::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& /*subTag*/, int& /*count*/)
{
    return BeginTag(tag);
}


bool CXmlWriteArchiveBase::EndTag(const iser::CArchiveTag& /*tag*/)
{
	I_ASSERT(IsValid());

	m_isFlushed = false;
    m_isFirstParamInTag = true;
    
    xercesc::DOMNode* parentNode = m_nodePtr->getParentNode();
    I_ASSERT(parentNode != NULL);
    
    m_nodePtr = parentNode;
    return true;
}


bool CXmlWriteArchiveBase::Process(std::string& data)
{
	istd::CString str(data);

	return Process(str);
}


bool CXmlWriteArchiveBase::Process(istd::CString& data)
{
	I_ASSERT(m_documentPtr != NULL);

	m_isFlushed = false;

    if (m_isFirstParamInTag){
		m_isFirstParamInTag = false;
	}
	else{
    	xercesc::DOMElement* elementPtr = m_documentPtr->createElement(GetElementSeparator().c_str());
        m_nodePtr->appendChild(elementPtr);
    }
    xercesc::DOMText* textPtr = m_documentPtr->createTextNode(data.c_str());
    m_nodePtr->appendChild(textPtr);

    return true;
}


// protected methods

CXmlWriteArchiveBase::CXmlWriteArchiveBase(const iser::IVersionInfo* versionInfoPtr)
:	BaseClass(versionInfoPtr)
{
    m_writerPtr = NULL;
    m_isFlushed = false;
	m_isFirstParamInTag = true;
}


void CXmlWriteArchiveBase::Init(xercesc::XMLFormatTarget* formTarget)
{
	I_ASSERT(m_writerPtr == NULL);
	I_ASSERT(!m_formTargetPtr.IsValid());
	I_ASSERT(formTarget != NULL);

	xercesc::DOMImplementation* impl = xercesc::DOMImplementationRegistry::getDOMImplementation(L"LS");

	m_isFlushed = false;
	m_writerPtr = ((xercesc::DOMImplementationLS*)impl)->createDOMWriter();

	if (m_writerPtr->canSetFeature(xercesc::XMLUni::fgDOMWRTDiscardDefaultContent, true)){
		m_writerPtr->setFeature(xercesc::XMLUni::fgDOMWRTDiscardDefaultContent, true);
	}

	if (m_writerPtr->canSetFeature(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true)){
		m_writerPtr->setFeature(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true);
	}

	m_formTargetPtr.SetPtr(formTarget);
	m_documentPtr = impl->createDocument();

	if (m_documentPtr != NULL){
		xercesc::DOMElement* element = m_documentPtr->createElement(istd::CString(s_acfRootTag.GetId()).c_str());
		m_documentPtr->appendChild(element);
		m_nodePtr = element;
	}
}


void CXmlWriteArchiveBase::Reset()
{
	m_formTargetPtr.Reset();

	if (m_writerPtr != NULL){
		m_writerPtr->release();
		m_writerPtr = NULL;
	}
}


} // namespace ixln


