#ifndef istd_TChangeNotifier_included
#define istd_TChangeNotifier_included


#include "istd/IChangeable.h"
#include "istd/TPointer.h"


namespace istd
{


/**
	Help class which provides the automatic update mechanism of the model.
*/
template <class Changeable>
class TChangeNotifier: public istd::TPointer<Changeable>
{
public:
	typedef istd::TPointer<Changeable> BaseClass;

	explicit TChangeNotifier(Changeable* changeablePtr, int changeFlags = IChangeable::CF_MODEL, istd::IPolymorphic* updateParamsPtr = NULL);
	virtual  ~TChangeNotifier();

	void SetPtr(Changeable* changeablePtr);
	void Reset();
	const Changeable* operator->() const;
	Changeable* operator->();
	operator Changeable*() const;

private:
	int m_changeFlags;
	istd::IPolymorphic* m_updateParamsPtr;
};


template <class Changeable>
TChangeNotifier<Changeable>::TChangeNotifier(Changeable* changeablePtr, int changeFlags, istd::IPolymorphic* updateParamsPtr)
:	BaseClass(changeablePtr),
	m_changeFlags(changeFlags),
	m_updateParamsPtr(updateParamsPtr)
{
	if (changeablePtr != NULL){
		changeablePtr->BeginChanges(m_changeFlags, m_updateParamsPtr);
	}
}


template <class Changeable>
TChangeNotifier<Changeable>::~TChangeNotifier()
{
	Reset();
}


template <class Changeable>
void TChangeNotifier<Changeable>::SetPtr(Changeable* changeablePtr)
{
	if (changeablePtr != GetPtr()){
		Reset();

		BaseClass::SetPtr(changeablePtr);

		if (changeablePtr != NULL){
			changeablePtr->BeginChanges(m_changeFlags, m_updateParamsPtr);
		}
	}
}


template <class Changeable>
inline void TChangeNotifier<Changeable>::Reset()
{
	Changeable* changeablePtr = GetPtr();

	if (changeablePtr != NULL){
		changeablePtr->EndChanges(m_changeFlags, m_updateParamsPtr);

		BaseClass::Reset();
	}
}


template <class Changeable>
inline const Changeable* TChangeNotifier<Changeable>::operator->() const
{
	I_ASSERT(IsValid());

	return GetPtr();
}


template <class Changeable>
inline Changeable* TChangeNotifier<Changeable>::operator->()
{
	I_ASSERT(IsValid());

	return GetPtr();
}


template <class Changeable>
inline TChangeNotifier<Changeable>::operator Changeable*() const
{
	I_ASSERT(IsValid());

	return GetPtr();
}


typedef TChangeNotifier<IChangeable> CChangeNotifier;


} // namespace istd


#endif // !istd_TChangeNotifier_included

