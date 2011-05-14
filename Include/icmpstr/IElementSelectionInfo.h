#ifndef icmpstr_IElementSelectionInfo_included
#define icmpstr_IElementSelectionInfo_included


// STL includes
#include <set>

// Qt includes
#include <QIcon>


// ACF includes
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
	enum ChangeFlags
	{
		CF_SELECTION = 1 << 31
	};

	typedef std::map<std::string, const icomp::IRegistry::ElementInfo*> Elements;

	/**
		Get registry containing selected element.
		\return	pointer to registry or NULL if no registry is selected.
	*/
	virtual icomp::IRegistry* GetSelectedRegistry() const = 0;
	/**
		Get set of selected elements.
		\return	map from element name to element information object.
				This map doesn't include pointers to NULL element info.
	*/
	virtual Elements GetSelectedElements() const = 0;
};


} // namespace icmpstr


#endif // !icmpstr_IElementSelectionInfo_included


