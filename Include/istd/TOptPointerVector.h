#ifndef istd_TOptPointerVector_included
#define istd_TOptPointerVector_included


// ACF includes
#include <istd/TPointerVector.h>


namespace istd
{


/**
	Optional delete adapter.
*/
template<typename Pointer>
class TOptDeleteAdapter
{
public:
	struct ElementType
	{
		Pointer* pointer;
		bool releaseFlag;
	};

	static Pointer* GetPtr(const ElementType& element)
	{
		return element.pointer;
	}

	static Pointer* PopPtr(const ElementType& element)
	{
		return GetPtr(element);
	}

	static void Delete(const ElementType& element)
	{
		if (element.releaseFlag){
			delete element.pointer;
		}
	}
};


/**
	Implementation of a special pointer container, 
	which controls the live cycle of the pointer object, but allows optional pointer removing.
*/
template <typename Pointer>
class TOptPointerVector: public TPointerVector<Pointer, TOptDeleteAdapter<Pointer> >
{
public:
	typedef TPointerVector<Pointer, TOptDeleteAdapter<Pointer> > BaseClass;

	void SetAt(int index, Pointer* pointer, bool releaseFlag = false);
	void PushBack(Pointer* pointer, bool releaseFlag = false);
	void InsertAt(int index, Pointer* pointer, bool releaseFlag = false);
};


// public methods

template <typename Pointer>
void TOptPointerVector<Pointer>::SetAt(int index, Pointer* pointer, bool releaseFlag)
{
	typename TOptDeleteAdapter<Pointer>::ElementType element;
	element.pointer = pointer;
	element.releaseFlag = releaseFlag;

	BaseClass::SetElementAt(index, element);
}


template <typename Pointer>
void TOptPointerVector<Pointer>::PushBack(Pointer* pointer, bool releaseFlag)
{
	typename TOptDeleteAdapter<Pointer>::ElementType element;
	element.pointer = pointer;
	element.releaseFlag = releaseFlag;

	BaseClass::PushBack(element);
}


template <typename Pointer>
void TOptPointerVector<Pointer>::InsertAt(int index, Pointer* pointer, bool releaseFlag)
{
	typename TOptDeleteAdapter<Pointer>::ElementType element;
	element.pointer = pointer;
	element.releaseFlag = releaseFlag;

	BaseClass::InsertElementAt(index, element);
}


} // namespace istd


#endif // !istd_TOptPointerVector_included

