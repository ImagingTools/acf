#ifndef icmpstr_IElementSelectionInfo_included
#define icmpstr_IElementSelectionInfo_included


#include "istd/IChangeable.h"

#include "iser/ISerializable.h"

#include "icomp/IRegistry.h"
#include "icomp/CComponentAddress.h"

#include "icmpstr/icmpstr.h"


namespace icmpstr
{


/**
	Interface containing all information about registry selected element.
*/
class IElementSelectionInfo: virtual public istd::IChangeable
{
public:
	/**
		Get registry containing selected element.
		\return	pointer to registry or NULL if no registry is selected.
	*/
	virtual icomp::IRegistry* GetSelectedRegistry() const = 0;
	/**
		Get selected element.
		\return	pointer to selected element or NULL if no element is selected.
	*/
	virtual iser::ISerializable* GetSelectedElement() const = 0;
	/**
		Get name of selected element.
	*/
	virtual const std::string& GetSelectedElementName() const = 0;
	/**
		Get icon of selected element.
	*/
	virtual const QIcon* GetSelectedElementIcon() const = 0;
	/**
		Get address of selected element.
		\return		pointer to address of selected element or NULL
					if no element is selected or selected element doesn't support this functionality.
	*/
	virtual const icomp::CComponentAddress* GetSelectedElementAddress() const = 0;
};


} // namespace icmpstr


#endif // !icmpstr_IElementSelectionInfo_included


