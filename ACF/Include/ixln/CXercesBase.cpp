#include "ixln/CXercesBase.h"


#include <xercesc/util/PlatformUtils.hpp>


namespace ixln
{


CXercesBase::CXercesBase()
:	m_nodePtr(NULL), m_documentPtr(NULL)
{
	AddXercescRef();
}


CXercesBase::~CXercesBase()
{
	RemXercescRef();
}


// protected static methods

void CXercesBase::AddXercescRef()
{
	if (s_xercescInstanceCount++ == 0){
    	xercesc::XMLPlatformUtils::Initialize();
	}
}


void CXercesBase::RemXercescRef()
{
	if (--s_xercescInstanceCount == 0){
		xercesc::XMLPlatformUtils::Terminate();
	}
}


// static attributes

int CXercesBase::s_xercescInstanceCount = 0;
const std::wstring CXercesBase::s_text = L"#text";


} // namespace ixln


