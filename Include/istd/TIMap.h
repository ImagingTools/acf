#ifndef istd_TIMap_included
#define istd_TIMap_included


// Qt includes
#include <QtCore/QSet>

// ACF includes
#include <istd/IPolymorphic.h>


namespace istd
{


/**
	Generic interface for a key/value mapping.
*/
template <typename Key, typename Value>
class TIMap: virtual public IPolymorphic
{
public:
	typedef Key KeyType;
	typedef Value ValueType;
	typedef QSet<KeyType> Keys;

	/**
		Get number of elements.
	*/
	virtual int GetElementsCount() const = 0;

	/**
		Element access operator.
		If key object not exists, it will be automatically created.
	*/
	virtual ValueType& operator[](const KeyType& key) = 0;

	/**
		Element access operator.
		If key object not exists, it will be automatically created.
	*/
	virtual const ValueType& operator[](const KeyType& key) const = 0;

	/**
		Find index index of specified key.
	*/
	virtual int FindIndex(const KeyType& key) const = 0;

	/**
		Find value element associated with specified key.
		\return	pointer to associated value, or NULL if key not exists.
	*/
	virtual const ValueType* FindElement(const KeyType& key) const = 0;

	/**
		Get list of keys stored in this map.
		\param	doAppend	if it is true, list of keys will be appended to existing key list.
	*/
	virtual void GetKeys(Keys& result, bool doAppend = false) const = 0;

	/**
		Get key value at specified index.
	*/
	virtual const KeyType& GetKeyAt(int index) const = 0;

	/**
		Get mapped value at specified index.
	*/
	virtual const ValueType& GetValueAt(int index) const = 0;
};


};


#endif // !istd_TIMap_included


