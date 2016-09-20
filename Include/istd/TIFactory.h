#ifndef istd_TIFactory_included
#define istd_TIFactory_included


// ACF includes
#include <istd/IFactoryInfo.h>


namespace istd
{


/**
	Generic interface for a factory.
*/
template <class Interface>
class TIFactory: virtual public IFactoryInfo  
{
public:
	typedef Interface InterfaceType;

	/**
		Create an instance of the object, mapped to the keyId \c keyId.
		\param	keyId		
		\return			pointer to created object or NULL if it was not possible to create it or keyId does not exist.
	*/
	virtual Interface* CreateInstance(const QByteArray& keyId = "") const = 0;
};


typedef TIFactory<void> IVoidFactory;
typedef TIFactory<istd::IPolymorphic> IFactory;


} // namespace istd


#endif // !istd_TIFactory_included

