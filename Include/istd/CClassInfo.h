#ifndef istd_CClassInfo_included
#define istd_CClassInfo_included


// STL includes
#include <string>
#include <typeinfo>

#include "istd/IPolymorphic.h"


namespace istd
{


/**
	Represents platform independent type info and provide set of static class manipulation functions.
*/
class CClassInfo: virtual public istd::IPolymorphic
{
public:
	CClassInfo();
	explicit CClassInfo(const std::type_info& info);
	explicit CClassInfo(const std::string& name);
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
	const std::string& GetName() const;

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
	static std::string GetName();
	/**
		Get class name from RTTI.
	*/
	static std::string GetName(const std::type_info& info);
	/**
		Get class name of polymorphic object.
	*/
	static std::string GetName(const istd::IPolymorphic& object);

	/**
		Get undecorated class name from raw name.
	*/
	static std::string GetUndecoratedName(const std::string& rawName);

private:
	std::string m_name;
};


// inline methods

inline CClassInfo::CClassInfo()
{
}


inline CClassInfo::CClassInfo(const std::type_info& info)
:	m_name(GetName(info))
{
}


inline CClassInfo::CClassInfo(const std::string& name)
:	m_name(GetUndecoratedName(name))
{
}


inline CClassInfo::CClassInfo(const istd::IPolymorphic& object)
:	m_name(GetUndecoratedName(typeid(object).name()))
{
}


inline bool CClassInfo::IsValid() const
{
	return !m_name.empty();
}


inline const std::string& CClassInfo::GetName() const
{
	return m_name;
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
	return CClassInfo(typeid(C));
}


template <class C>
std::string CClassInfo::GetName()
{
	return GetUndecoratedName(typeid(C).name());
}


} // namespace istd


#endif // !istd_CClassInfo_included


