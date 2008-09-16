#ifndef icomp_TInterfaceManipBase_included
#define icomp_TInterfaceManipBase_included


namespace icomp
{


template <class Interface>
class TInterfaceManipBase
{
protected:
	/**
		Extract interface from component.
	*/
	Interface* ExtractInterface(IComponent* componentPtr, const std::string& subId = "") const;

	/**
		Split component ID into separated ID's.
	*/
	bool SplitId(const std::string& complexId, std::string& baseId, std::string& subId) const;
};


// protected methods

template <class Interface>
typename Interface* TInterfaceManipBase<Interface>::ExtractInterface(IComponent* componentPtr, const std::string& subId = "") const
{
	if (componentPtr != NULL){
		return static_cast<Interface*>(componentPtr->GetInterface(typeid(Interface), subId));
	}
	else{
		return NULL;
	}
}


template <class Interface>
bool TInterfaceManipBase<Interface>::SplitId(const std::string& complexId, std::string& baseId, std::string& subId) const
{
	std::string::size_type separatorPos = complexId.find('/');
	if (separatorPos != std::string::npos){
		baseId = complexId.substr(0, separatorPos);
		subId = complexId.substr(separatorPos + 1);

		return true;
	}
	else{
		baseId = complexId;
		subId = "";

		return false;
	}
}


}//namespace icomp


#endif // !icomp_TInterfaceManipBase_included


