#ifndef istd_TPointerVector_included
#define istd_TPointerVector_included


#include <vector>
#include <algorithm>


namespace istd
{


/**
	Default delete adapter.
*/
template<typename PointerType>
class TDeleteAdapter: public ::std::unary_function<PointerType, void>
{
public:
	void operator()(PointerType& pointer)
	{
		I_ASSERT(pointer != NULL);

		delete pointer;
	}
};


/**
	Implementation of a pointer container, 
	which controls the live cycle of the pointer object.
*/
template <typename PointerType, class DeleteAdapter = TDeleteAdapter<PointerType*> >
class TPointerVector
{
public:
	~TPointerVector();

	void Reset();
	PointerType* GetAt(int index);
	const PointerType* GetAt(int index) const;
	void RemoveAt(int index);
	PointerType* PopAt(int index);
	void PushBack(PointerType* elementPtr);

private:
	typedef std::vector<PointerType*> Elements;

	Elements m_elements;
};


template <typename PointerType, class DeleteAdapter>
TPointerVector<PointerType, DeleteAdapter>::~TPointerVector()
{
	Reset();
}


template <typename PointerType, class DeleteAdapter>
void TPointerVector<PointerType, DeleteAdapter>::Reset()
{
	std::for_each(m_elements.begin(), m_elements.end(), DeleteAdapter());

	m_elements.clear();
}


template <typename PointerType, class DeleteAdapter>
PointerType* TPointerVector<PointerType, DeleteAdapter>::GetAt(int index)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_elements.size()));

	return *(m_elements.begin() + index);
}


template <typename PointerType, class DeleteAdapter>
const PointerType* TPointerVector<PointerType, DeleteAdapter>::GetAt(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_elements.size()));

	return *(m_elements.begin() + index);
}


template <typename PointerType, class DeleteAdapter>
void TPointerVector<PointerType, DeleteAdapter>::RemoveAt(int index)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_elements.size()));

	Elements::iterator delIter = (m_elements.begin() + index);

	DeleteAdapter()(*delIter);

	m_elements.erase(delIter);
}


template <typename PointerType, class DeleteAdapter>
PointerType* TPointerVector<PointerType, DeleteAdapter>::PopAt(int index)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_elements.size()));

	PointerType* popPtr = *(m_elements.begin() + index);

	m_elements.erase(m_elements.begin() + index);

	return popPtr;
}


template <typename PointerType, class DeleteAdapter>
void TPointerVector<PointerType, DeleteAdapter>::PushBack(PointerType* elementPtr)
{
	m_elements.push_back(elementPtr);
}


} // namespace istd


#endif // !istd_TPointerVector_included

