#include "ixln/CXmlReadArchiveBase.h"


#include <stdio.h>
#include <sstream>

#include <xercesc/framework/Wrapper4InputSource.hpp>

#include "iser/CArchiveTag.h"


namespace ixln
{


CXmlReadArchiveBase::~CXmlReadArchiveBase()
{
	Reset();
}


bool CXmlReadArchiveBase::IsValid() const
{
	return m_isValid;
}


// reimplemented (iser::IArchive)

bool CXmlReadArchiveBase::BeginTag(const iser::CArchiveTag& tag)
{
	if (m_documentPtr == NULL || m_nodePtr == NULL){
		return false;
	}

    xercesc::DOMNode* foundNode = NULL;
    wchar_t* tagName = xercesc::XMLString::transcode(tag.GetId().c_str());
    std::wstring strTagName = tagName;

    xercesc::DOMElement* currentElement = NULL;
	if (m_nodePtr->getNodeType() == xercesc::DOMNode::ELEMENT_NODE) {
         currentElement = (xercesc::DOMElement*) m_nodePtr;
	}

    if(currentElement != NULL){
        xercesc::DOMNodeList* nodeList = currentElement->getElementsByTagName(tagName);
        foundNode = nodeList->item(0);
    }
    else{
        foundNode = m_nodePtr->getFirstChild();
		if(foundNode != NULL && strTagName != foundNode->getNodeName()){
			foundNode = NULL;
		}
    }
	if(foundNode){
        m_nodePtr = foundNode;
	}

	xercesc::XMLString::release(&tagName);
   
    return true;
}


bool CXmlReadArchiveBase::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count)
{
	if (!BeginTag(tag)){
		return false;
	}

	if (m_documentPtr == NULL){
		return false;
	}

    count = 0;

	wchar_t* tagName = xercesc::XMLString::transcode(subTag.GetId().c_str());
    
	xercesc::DOMElement* currentElement = NULL;
	if (m_nodePtr->getNodeType() == xercesc::DOMNode::ELEMENT_NODE){
         currentElement = (xercesc::DOMElement*) m_nodePtr;
	}

    if (currentElement != NULL){
        xercesc::DOMNodeList* nodeList = m_nodePtr->getChildNodes();
 
		int nodesCount = nodeList->getLength();
		for (int i = 0; i < nodesCount; i++){
			xercesc::DOMNode* childPtr = nodeList->item(i);
			if (			(childPtr != NULL) &&
							(childPtr->getNodeType() == xercesc::DOMNode::ELEMENT_NODE) &&
							(istd::CString(childPtr->getNodeName()) == tagName)){
				++count;
			}
		}
    }
	xercesc::XMLString::release(&tagName);

    return true;
}


bool CXmlReadArchiveBase::EndTag(const iser::CArchiveTag& /*tag*/)
{
	if (m_documentPtr == NULL){
		return false;
	}

	I_ASSERT(m_nodePtr != NULL);
	
    xercesc::DOMNode* parentNode = m_nodePtr->getParentNode();
    //I_ASSERT(parentNode != NULL);

	if (parentNode == NULL){
		return false;
	}

    parentNode->removeChild(m_nodePtr);
    m_nodePtr = parentNode;
    return true;
}


bool CXmlReadArchiveBase::Process(std::string& data)
{
    istd::CString text;
    bool retVal = Process(text);

	if (!retVal){
		return false;
	}

    char* transcodedText = xercesc::XMLString::transcode(text.c_str());

	data = text.ToString();

	xercesc::XMLString::release(&transcodedText);
        
    return true;
}


bool CXmlReadArchiveBase::Process(istd::CString& data)
{
	if (m_documentPtr == NULL){
		return false;
	}

	data = L"";

	xercesc::DOMNode* dataNode = m_nodePtr->getFirstChild();
	if (dataNode != NULL){
		if (dataNode->getNodeName() == s_text){
			data = dataNode->getNodeValue();

			m_nodePtr->removeChild(dataNode);

			dataNode = m_nodePtr->getFirstChild();
		}
	}
	else{
		return true;
	}

	if ((dataNode != NULL) && (dataNode->getNodeName() == GetElementSeparator().c_str())){
		m_nodePtr->removeChild(dataNode);
	}

	return true;
}


// protected methods

CXmlReadArchiveBase::CXmlReadArchiveBase()
{
	m_builder = NULL;
	m_isValid = true;
}


void CXmlReadArchiveBase::Init(xercesc::InputSource& inputSource)
{
	I_ASSERT(m_builder == NULL);
	
	XMLCh tempStr[100];
	xercesc::XMLString::transcode("LS", tempStr, 99);
	xercesc::DOMImplementation *impl = xercesc::DOMImplementationRegistry::getDOMImplementation(tempStr);
	m_builder = ((xercesc::DOMImplementationLS*)impl)->createDOMBuilder(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	
	// optionally you can set some features on this builder
	if (m_builder->canSetFeature(xercesc::XMLUni::fgDOMValidation, true))
		m_builder->setFeature(xercesc::XMLUni::fgDOMValidation, true);
	if (m_builder->canSetFeature(xercesc::XMLUni::fgDOMNamespaces, true))
		m_builder->setFeature(xercesc::XMLUni::fgDOMNamespaces, true);
	if (m_builder->canSetFeature(xercesc::XMLUni::fgDOMDatatypeNormalization, true))
		m_builder->setFeature(xercesc::XMLUni::fgDOMDatatypeNormalization, true);
	
	xercesc::Wrapper4InputSource wrapperSource(&inputSource, false);
	m_documentPtr = m_builder->parse(wrapperSource);

	if (m_documentPtr != NULL){
		m_nodePtr = m_documentPtr->getFirstChild();
		if (m_nodePtr == NULL || xercesc::XMLString::compareIString(m_nodePtr->getNodeName(), istd::CString(s_acfRootTag.GetId()).c_str()) != 0){
			m_documentPtr->release();
			m_documentPtr = NULL;
			m_nodePtr = NULL;
		}
	}
}


void CXmlReadArchiveBase::Reset()
{
	if (m_builder != NULL){
        m_builder->release();
		m_builder = NULL;
	}
}


} // namespace ixln


