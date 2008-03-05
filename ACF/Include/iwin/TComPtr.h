#ifndef iwin_TComPtr_included
#define iwin_TComPtr_included


#include "objbase.h"

#include "istd/TPointer.h"


namespace iwin
{


/**
	Pointer wrapper providing automatic deleting pointed object during destruction.
*/
template <class Type>
class TComPtr: public istd::TPointer<Type>
{
public:
	typedef istd::TPointer<Type> BaseClass;

	/**
		Set new value of internal pointer.
	 */
	TComPtr(Type* ptr = NULL, bool addRef = true);

	/**
		Copy constructor.
		It increase COM reference pointer.
	*/
	TComPtr(const TComPtr& ptr);

	/**
		Destructor.
		It decreses COM reference pointer.
	 */
	~TComPtr();

	/**
		Create instance of COM object using its CLSID.
	*/
	bool CoCreateInstance(REFCLSID rclsid, IUnknown* outerPtr = NULL, I_DWORD clsContext = CLSCTX_ALL);

	/**
		It decreses COM reference pointer and set this pointer to NULL.
	*/
	void Reset();

	/**
		Set new value of internal pointer.
	 */
	void SetPtr(Type* ptr, bool addRef = true);

	/**
		Reset internal pointer value without decreasing of reference counter.
	 */
	Type* PopPtr();
	/**
		Take internal pointer over.
		It set pointer from other object and detach it from them.
	*/
	void TakeOver(TComPtr& sourcePtr);

	/**
		Assign operator.
	*/
	TComPtr& operator=(const TComPtr& ptr);
	/**
		Address operator.
	*/
	Type** operator&();

protected:
	/**
		Increase reference counter.
	*/
	void IncRef();
	/**
		Decrease reference counter.
	*/
	void DecRef();
};


// inline methods

template <class Type>
inline TComPtr<Type>::TComPtr(Type* ptr, bool addRef)
:	BaseClass(ptr)
{
	if (addRef){
		IncRef();
	}
}


template <class Type>
inline TComPtr<Type>::TComPtr(const TComPtr<Type>& ptr)
:	BaseClass(ptr)
{
	IncRef();
}


template <class Type>
inline TComPtr<Type>::~TComPtr()
{
	DecRef();
}


template <class Type>
inline void TComPtr<Type>::Reset()
{
	SetPtr(NULL);
}


template <class Type>
inline void TComPtr<Type>::SetPtr(Type* ptr, bool addRef)
{
	DecRef();

	BaseClass::SetPtr(ptr);

	if (addRef){
		IncRef();
	}
}


template <class Type>
inline typename Type* TComPtr<Type>::PopPtr()
{
	Type* slavePtr = BaseClass::GetPtr();
	BaseClass::SetPtr(NULL);

	return slavePtr;
}


template <class Type>
void TComPtr<Type>::TakeOver(TComPtr& sourcePtr)
{
	SetPtr(sourcePtr.PopPtr(), false);
}


template <class Type>
inline typename Type** TComPtr<Type>::operator&()
{
	return &BaseClass::GetPtrRef();
}


// public methods

template <class Type>
bool TComPtr<Type>::CoCreateInstance(REFCLSID rclsid, IUnknown* outerPtr, I_DWORD clsContext)
{
	Reset();

	return !FAILED(::CoCreateInstance(rclsid, outerPtr, clsContext, __uuidof(Type), (void**)operator&()));
}


template <class Type>
TComPtr<Type>& TComPtr<Type>::operator=(const TComPtr& ptr)
{
	SetPtr(ptr.GetPtr(), true);

	return *this;
}


// protected methods

template <class Type>
void TComPtr<Type>::IncRef()
{
	Type* ptr = BaseClass::GetPtr();
	if (ptr == NULL){
		return;
	}

	ptr->AddRef();
}


template <class Type>
void TComPtr<Type>::DecRef()
{
	Type* ptr = BaseClass::GetPtr();
	if (ptr == NULL){
		return;
	}

	ptr->Release();
}


} // namespace iwin


#endif // !iwin_TComPtr_included


