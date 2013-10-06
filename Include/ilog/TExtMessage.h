#ifndef ilog_TExtMessage_included
#define ilog_TExtMessage_included


// ACF includes
#include "imod/TModelWrap.h"
#include "ilog/CMessage.h"


namespace ilog
{


/**
	Extended message with attached some object.
*/
template<class Element>
class TExtMessage:
			public CMessage,
			public imod::TModelWrap<Element>
{
public:
	typedef CMessage BaseClass;
	typedef imod::TModelWrap<Element> BaseClass2;

	TExtMessage();
	TExtMessage(istd::IInformationProvider::InformationCategory category,
				int id,
				const QString& text, 
				const QString& source,
				int flags = 0,
				const QDateTime* timeStampPtr = NULL);

	// reimplemented (iser::IObject)
	virtual QByteArray GetFactoryId() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (iser::IChangeable)
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_STRICT);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

	// static methods
	static QByteArray GetTypeName();
};


// public methods

template<class Element> 
TExtMessage<Element>::TExtMessage()
	:Element(),
	BaseClass()
{
}


template<class Element> 
TExtMessage<Element>::TExtMessage(
			istd::IInformationProvider::InformationCategory category,
			int id,
			const QString& text, 
			const QString& source,
			int flags,
			const QDateTime* timeStampPtr)
:	Element(),
	CMessage(category, id, text, source, flags, timeStampPtr)
{
}


// reimplemented (iser::IObject)

template<class Element> 
QByteArray TExtMessage<Element>::GetFactoryId() const
{
	return GetTypeName();
}


// reimplemented (iser::ISerializable)

template<class Element> 
bool TExtMessage<Element>::Serialize(iser::IArchive& archive)
{
	return BaseClass::Serialize(archive) && BaseClass2::Serialize(archive);
}


// reimplemented (iser::IChangeable)

template<class Element> 
bool TExtMessage<Element>::CopyFrom(const istd::IChangeable& object, CompatibilityMode mode)
{
	 return BaseClass::CopyFrom(object, mode) && BaseClass2::CopyFrom(object, mode);
}


template<class Element> 
istd::IChangeable* TExtMessage<Element>::CloneMe(CompatibilityMode mode) const
{
	istd::TDelPtr<TExtMessage<Element> > clonedPtr(new TExtMessage<Element>);
	if (clonedPtr->CopyFrom(*this, mode)){
		return clonedPtr.PopPtr();
	}

	return NULL;
}


// static methods

template<class Element> 
QByteArray TExtMessage<Element>::GetTypeName()
{
	return istd::CClassInfo::GetName<TExtMessage<Element> >();
}


} // namespace ilog


#endif // ilog_TExtMessage_included


