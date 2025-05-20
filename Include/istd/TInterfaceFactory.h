#pragma once


// ACF includes
#include <istd/TInterfacePtr.h>
#include <istd/IFactoryInfo.h>


namespace istd
{


template <class InterfaceType>
class TInterfaceFactory: virtual public istd::IFactoryInfo
{
public:
	virtual istd::TUniqueInterfacePtr<InterfaceType> CreateInstance(const QByteArray& typeId = QByteArray()) const = 0;
};


} // namespace istd


