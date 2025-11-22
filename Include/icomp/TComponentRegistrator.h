#pragma once


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <icomp/TComponentStaticInfo.h>
#include <icomp/CPackageStaticInfo.h>


namespace icomp
{


/**
	Simple helper class providing registration of template-specified component in package.
*/
template <class Component>
class TComponentRegistrator: public icomp::TComponentStaticInfo<Component>
{
public:
	typedef icomp::TComponentStaticInfo<Component> BaseClass;

	TComponentRegistrator(
				const QByteArray& componentId,
				icomp::CPackageStaticInfo& packageStaticInfo,
				const QString& description,
				const QString& keywords);

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual const QString& GetDescription() const override;
	virtual const QString& GetKeywords() const override;

private:
	QByteArray m_logicalName;
	QString m_description;
	QString m_keywords;
};


// public methods

template <class Component>
TComponentRegistrator<Component>::TComponentRegistrator(
			const QByteArray& componentId,
			icomp::CPackageStaticInfo& packageStaticInfo,
			const QString& description,
			const QString& keywords)
:	BaseClass(&Component::InitStaticInfo(NULL)),
	m_description(description),
	m_keywords(keywords)
{
	packageStaticInfo.RegisterEmbeddedComponentInfo(componentId, this);

	m_keywords += " ";

	m_keywords += packageStaticInfo.GetKeywords();
}


//	reimplemented (icomp::IComponentStaticInfo)

template <class Component>
const QString& TComponentRegistrator<Component>::GetDescription() const
{
	return m_description;
}


template <class Component>
const QString& TComponentRegistrator<Component>::GetKeywords() const
{
	return m_keywords;
}


} // namespace icomp


