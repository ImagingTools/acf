#ifndef istd_CClassInfo_included
#define istd_CClassInfo_included


// STL includes
#include <typeinfo>

// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include "istd/IPolymorphic.h"


namespace istd
{


/**
	Represents platform independent type info and provide set of static class manipulation functions.

	\ingroup Main
*/
class CClassInfo: virtual public istd::IPolymorphic
{
public:
	CClassInfo();
	explicit CClassInfo(const std::type_info& info);
	explicit CClassInfo(const QByteArray& name);
	explicit CClassInfo(const istd::IPolymorphic& object);

	/**
		Check if this class info object is valid.
		It is valid if stored RTTI type info is set.
	*/
	bool IsValid() const;
	/**
		Get undecorated and platform undependent class name.
		This name has format "namespace::class_name", for example "iser::ISerializable".
	*/
	const QByteArray& GetName() const;

	/**
		Check if this class information represents void type.
	*/
	bool IsVoid() const;

	/**
		Check if this class information represents const class.
	*/
	bool IsConst() const;

	/**
		Get class with enabled or disabled const attribute.
	*/
	CClassInfo GetConstCasted(bool enableConst = true) const;

	/**
		Enable or disable const attribute.
	*/
	bool ConstCast(bool enableConst = true);

	/**
		Check if this class information represents template class.
	*/
	bool IsTemplateClass() const;
	/**
		Get number of template parameters if this class is template class.
		\return	number of parameters of negative value, if this class is not a template.
	*/
	int GetTemplateParamsCount() const;
	/**
		Get class info of some template parameter if this class is template.
	*/
	CClassInfo GetTemplateParam(int paramIndex = 0) const;

	template <class C>
	bool IsType() const;

	CClassInfo& operator=(const std::type_info& info);
	CClassInfo& operator=(const CClassInfo& info);
	bool operator==(const CClassInfo& info) const;
	bool operator!=(const CClassInfo& info) const;
	bool operator<(const CClassInfo& info) const;
	bool operator>(const CClassInfo& info) const;

	// static methods
	/**
		Get class name defined as template parameter.
	*/
	template <class C>
	static CClassInfo GetInfo();
	/**
		Get class name defined as template parameter.
	*/
	template <class C>
	static QByteArray GetName();
	/**
		Get class name from RTTI.
	*/
	static QByteArray GetName(const std::type_info& info);
	/**
		Get class name of polymorphic object.
	*/
	static QByteArray GetName(const istd::IPolymorphic& object);

	/**
		Get undecorated class name from raw name.
	*/
	static QByteArray GetUndecoratedName(const QByteArray& rawName);

private:
	QByteArray m_name;
};


// inline methods

inline CClassInfo::CClassInfo()
{
}


inline CClassInfo::CClassInfo(const std::type_info& info)
:	m_name(GetName(info))
{
}


inline CClassInfo::CClassInfo(const QByteArray& name)
:	m_name(GetUndecoratedName(name))
{
}


inline CClassInfo::CClassInfo(const istd::IPolymorphic& object)
:	m_name(GetUndecoratedName(typeid(object).name()))
{
}


inline bool CClassInfo::IsValid() const
{
	return !m_name.isEmpty();
}


inline const QByteArray& CClassInfo::GetName() const
{
	return m_name;
}


inline bool CClassInfo::IsVoid() const
{
	return *this == istd::CClassInfo::GetInfo<void>();
}


template <class C>
inline bool CClassInfo::IsType() const
{
	return m_name == GetName(typeid(C));
}


inline CClassInfo& CClassInfo::operator=(const std::type_info& info)
{
	m_name = GetName(info);

	return *this;
}


inline CClassInfo& CClassInfo::operator=(const CClassInfo& info)
{
	m_name = info.m_name;

	return *this;
}


inline bool CClassInfo::operator==(const CClassInfo& info) const
{
	return m_name == info.m_name;
}


inline bool CClassInfo::operator!=(const CClassInfo& info) const
{
	return m_name != info.m_name;
}


inline bool CClassInfo::operator<(const CClassInfo& info) const
{
	return m_name < info.m_name;
}


inline bool CClassInfo::operator>(const CClassInfo& info) const
{
	return m_name > info.m_name;
}


// public static methods

template <class C>
CClassInfo CClassInfo::GetInfo()
{
	CClassInfo retVal(typeid(C));

	return retVal;
}


template <class C>
QByteArray CClassInfo::GetName()
{
	return GetUndecoratedName(typeid(C).name());
}


} // namespace istd


#endif // !istd_CClassInfo_included


