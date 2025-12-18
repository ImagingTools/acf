#ifndef istd_TComposedFactory_included
#define istd_TComposedFactory_included


// Qt includes
#include <QtCore/QList>

// ACF includes
#include <istd/TIFactory.h>
#include <istd/TSmartPtr.h>


namespace istd
{


/**
	Standard generic implementation of the composed factory.
*/
template <class InterfaceType>
class TComposedFactory: virtual public TIFactory<InterfaceType>
{
public:
	typedef TIFactory<InterfaceType> FactoryInterface;

	template <class FactoryImpl>
	bool RegisterFactory(const QByteArray& typeId)
	{
		istd::TSmartPtr<FactoryInterface> factoryPtr(new FactoryImpl(typeId));

		if (factoryPtr->GetFactoryKeys().isEmpty()){
			return false;
		}

		m_factoryList.push_back(factoryPtr);

		return true;
	}

	// reimplemented (istd::IFactoryInfo)
	virtual IFactoryInfo::KeyList GetFactoryKeys() const override;

	// reimplemented (istd::TIFactory)
	virtual InterfaceType* CreateInstance(const QByteArray& keyId = "") const override;

protected:
	typedef istd::TSmartPtr<FactoryInterface> FactoryPtr;
	typedef QList<FactoryPtr> FactoryList;

	FactoryList m_factoryList;
};


// reimplemented (istd::IFactoryInfo)

template <class InterfaceType>
IFactoryInfo::KeyList TComposedFactory<InterfaceType>::GetFactoryKeys() const
{
	IFactoryInfo::KeyList retVal;

	for (		typename FactoryList::const_iterator iter = m_factoryList.begin();
				iter != m_factoryList.end();
				++iter){
		const FactoryPtr& factoryPtr = *iter;

		IFactoryInfo::KeyList factoryKeys = factoryPtr->GetFactoryKeys();

		retVal += factoryKeys;
	}

	return retVal;
}


// reimplemented (istd::TIFactory)

template <class InterfaceType>
InterfaceType* TComposedFactory<InterfaceType>::CreateInstance(const QByteArray& keyId) const
{
	for (		typename FactoryList::const_iterator iter = m_factoryList.begin();
				iter != m_factoryList.end();
				++iter){
		const FactoryPtr& factoryPtr = *iter;

		InterfaceType* createdPtr = factoryPtr->CreateInstance(keyId);

		if (createdPtr != NULL){
			return createdPtr;
		}
	}

	return NULL;
}


} // namespace istd


#endif // !istd_TComposedFactory_included


