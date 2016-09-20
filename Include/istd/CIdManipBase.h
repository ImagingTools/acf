#ifndef istd_CIdManipBase_included
#define istd_CIdManipBase_included


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <istd/istd.h>


namespace istd
{


/**
	Base implementation for operations on composed ID objects.
*/
class CIdManipBase
{
public:
	/**
		Split component ID into separated ID's.
	*/
	static bool SplitId(const QByteArray& complexId, QByteArray& baseId, QByteArray& subId);
	/**
		Join base component ID and sub ID into composed component ID.
	*/
	static QByteArray JoinId(const QByteArray& baseId, const QByteArray& subId);
};


}//namespace istd


#endif // !istd_CIdManipBase_included


