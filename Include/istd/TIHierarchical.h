#ifndef istd_TIHierarchical_included
#define istd_TIHierarchical_included


// ACF includes
#include <istd/IPolymorphic.h>


namespace istd
{		


/**
	Common interface to define the hierarchical graph structures.
*/
template <class Interface>
class TIHierarchical: virtual public Interface
{
public:
	typedef Interface InterfaceType;

	/**
		Flags describing supported features of this hierarchical graph.
	*/
	enum HierarchicalFlags
	{
		/**
			If active child information is supported.
		*/
		HF_CHILDS_SUPPORTED = 0x01,
		/**
			If active parent information is supported.
			It will be inactive for single direction hierarchical graphs.
		*/
		HF_PARENT_SUPPORTED = 0x02,
	};

	/**
		Get flags describing supported features of this hierarchical graph.
		\sa	HierarchicalFlags.
	*/
	virtual int GetHierarchicalFlags() const = 0;
	/**
		Get number of childs.
		For single direction graph when only parent connection is stored it will be always 0.
		To check if this feature is supported check HF_CHILDS_SUPPORTED in flags.
	*/
	virtual int GetChildsCount() const = 0;
	/**
		Get child value.
	*/
	virtual Interface* GetChild(int index) const = 0;
	/**
		Get access to parent object.
		For single direction graph when only child connection is stored it will be always NULL.
		To check if this feature is supported check HF_PARENT_SUPPORTED in flags.
	*/
	virtual Interface* GetParent() const = 0;
};


typedef TIHierarchical<IPolymorphic> IHierarchical;


} // namespace istd


#endif // !istd_TIHierarchical_included

