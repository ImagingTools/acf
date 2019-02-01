#ifndef iser_CPrimitiveTypesSerializer_included
#define iser_CPrimitiveTypesSerializer_included


// STL includes
#include <typeinfo>

// Qt includes
#include <QtCore/QDateTime>
#include <QtCore/QPointF>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaEnum>

// ACF includes
#include <istd/TRange.h>
#include <istd/TRanges.h>
#include <istd/TIndex.h>
#include <iser/IArchive.h>


namespace iser
{


/**
	Implementation of serialize method for some common data types.
*/
class CPrimitiveTypesSerializer
{
public:
	/**
		Serialize a range object.
	*/
	static bool SerializeRange(iser::IArchive& archive, istd::CRange& range);

	/**
		Serialize an integer-based range object.
	*/
	static bool SerializeIntRange(iser::IArchive& archive, istd::CIntRange& range);

	/**
		Serialize list of range objects.
	*/
	static bool SerializeRanges(iser::IArchive& archive, istd::CRanges& ranges);

	/**
		Serialize list of integer-based ranges.
	*/
	static bool SerializeIntRanges(iser::IArchive& archive, istd::CIntRanges& ranges);

	/**
		Serialize QDateTime object.
	*/
	static bool SerializeDateTime(iser::IArchive& archive, QDateTime& dateTime);

	/**
		Serialize QDate object.
	*/
	static bool SerializeDate(iser::IArchive& archive, QDate& date);

	/**
		Serialize a generic index object.
	*/
	template <int Dimensions>
	static bool SerializeIndex(iser::IArchive& archive, istd::TIndex<Dimensions>& index);

	/**
		Serialize a QPointF object.
	*/
	static bool SerializeQPointF(iser::IArchive& archive, QPointF& point);

	/**
		Serialize a QStringList object.
	*/
	static bool SerializeQStringList(
				iser::IArchive& archive,
				QStringList& stringList,
				const QByteArray& containerTagName = "Elements",
				const QByteArray& elementTagName = "Element");

	template <typename ContainterType>
	static bool SerializeContainer(
				iser::IArchive& archive,
				ContainterType& container,
				const QByteArray& containerTagName,
				const QByteArray& elementTagName);

	/**
		Method for serialization of the enumerated value using ACF's meta information extensions for the C++ enums.
		This implementation supports both methods for serialization of the enumerator - as integer value or in textual form.
		The second variant is only possible if the enumeration was defined using I_DECLARE_ENUM macro.
		\sa I_DECLARE_ENUM I_DECLARE_FLAGS
	*/
	template <typename EnumType, QByteArray (*ToString)(EnumType), bool (*FromString)(const QByteArray&, EnumType&)>
	static bool SerializeEnum(
				iser::IArchive& archive,
				EnumType& enumValue);

	/**
		Method for serialization of the enumerated value using Qt's meta information system.
		This implementation supports both methods for serialization of the enumerator - as integer value or in textual form.
		The second variant is only possible if the owner of the enumeration is a class which is directly or indirectly derived from QObject
	*/
	template <typename EnumType>
	static bool SerializeEnum(
				iser::IArchive& archive,
				EnumType& enumValue,
				const QObject* objectPtr);
};


// public template methods

template <int Dimensions>
bool CPrimitiveTypesSerializer::SerializeIndex(iser::IArchive& archive, istd::TIndex<Dimensions>& index)
{
	bool retVal = true;

	for (int i = 0; i < Dimensions; ++i){
		retVal = retVal && archive.Process(index[i]);
	}

	return retVal;
}


template <typename ContainerType>
bool CPrimitiveTypesSerializer::SerializeContainer(
	iser::IArchive& archive,
	ContainerType& container,
	const QByteArray& containerTagName,
	const QByteArray& elementTagName)
{
	iser::CArchiveTag elementsTag(containerTagName, "List of elements", iser::CArchiveTag::TT_MULTIPLE);
	iser::CArchiveTag elementTag(elementTagName, "Single element", iser::CArchiveTag::TT_LEAF, &elementsTag);

	bool retVal = true;

	bool isStoring = archive.IsStoring();
	int elementsCount = container.count();

	retVal = retVal && archive.BeginMultiTag(elementsTag, elementTag, elementsCount);
	if (!retVal){
		return false;
	}

	if (isStoring){
		for (int i = 0; i < elementsCount; ++i){
			ContainerType::value_type element = container[i];

			retVal = retVal && archive.BeginTag(elementTag);
			retVal = retVal && archive.Process(element);
			retVal = retVal && archive.EndTag(elementTag);
		}
	}
	else{
		container.clear();

		for (int i = 0; i < elementsCount; ++i){
			ContainerType::value_type element;
			retVal = retVal && archive.BeginTag(elementTag);
			retVal = retVal && archive.Process(element);
			retVal = retVal && archive.EndTag(elementTag);

			if (retVal){
				container.push_back(element);
			}
		}
	}

	retVal = retVal && archive.EndTag(elementsTag);

	return retVal;
}


template <typename EnumType, QByteArray (*ToString)(EnumType), bool (*FromString)(const QByteArray&, EnumType&)>
bool CPrimitiveTypesSerializer::SerializeEnum(
			iser::IArchive& archive,
			EnumType& enumValue)
{
	QByteArray enumValueAsText;

	bool retVal = true;

	// Try to get enumeration text generated by I_DECLARE_ENUM macro:
	if (enumValueAsText.isEmpty() && (ToString != NULL)){
		enumValueAsText = ToString(enumValue);
	}

	// Enum value is defined in textual form:
	if (!enumValueAsText.isEmpty()){
		retVal = retVal && archive.Process(enumValueAsText);
		
		if (retVal && !archive.IsStoring()){
			if (FromString != NULL){
				retVal = FromString(enumValueAsText, enumValue);
			}
			else{
				retVal = false;
			}
		}
	}
	else{
		int value = enumValue;

		retVal = retVal && archive.Process(value);

		if (!archive.IsStoring()){
			// TODO: check if the readed value is in range of the enum values!
			enumValue = EnumType(value);
		}
	}

	return retVal;
}


template <typename EnumType>
bool CPrimitiveTypesSerializer::SerializeEnum(
			iser::IArchive& archive,
			EnumType& enumValue,
			const QObject* objectPtr)
{
	QByteArray enumValueAsText;
	QMetaEnum foundEnumMeta;

	QString enumTypeName = typeid(EnumType).name();
	enumTypeName = enumTypeName.mid(enumTypeName.lastIndexOf(":") + 1);

	if (objectPtr != NULL){
		const QMetaObject* metaObjectPtr = objectPtr->metaObject();

		// Iterate over all enums of the class:
		int enumeratorsCount = metaObjectPtr->enumeratorCount();
		for (int enumeratorIndex = 0; enumeratorIndex < enumeratorsCount; ++enumeratorIndex){
			QMetaEnum enumMeta = metaObjectPtr->enumerator(enumeratorIndex);
			QString enumName = enumMeta.name();

			if (enumTypeName == enumName){
				foundEnumMeta = enumMeta;

				// Find the enum value:
				int keysCount = enumMeta.keyCount();
				for (int keyIndex = 0; keyIndex < keysCount; ++keyIndex){
				
					if (enumMeta.value(keyIndex) == enumValue){
						enumValueAsText = enumMeta.valueToKey(keyIndex);
						break;
					}
				}
			}
		}
	}

	bool retVal = true;

	// Enum value is defined in textual form:
	if (!enumValueAsText.isEmpty()){
		retVal = retVal && archive.Process(enumValueAsText);
		
		if (retVal && !archive.IsStoring()){
			Q_ASSERT(foundEnumMeta.isValid());
#if QT_VERSION >= 0x050000
			enumValue = EnumType(foundEnumMeta.keyToValue(enumValueAsText.constData(), &retVal));
#else
			int value = foundEnumMeta.keyToValue(enumValueAsText.constData());
			if (value >= 0){
				enumValue = EnumType(value);
			}
			else{
				return false;
			}
#endif
		}
	}
	else{
		int value = enumValue;

		retVal = retVal && archive.Process(value);

		if (!archive.IsStoring()){
			// TODO: check if the readed value is in range of the enum values!
			enumValue = EnumType(value);
		}
	}

	return retVal;
}


#define I_SERIALIZE_FLAG(Enum, archive, flag) iser::CPrimitiveTypesSerializer::SerializeEnum<int, Enum##ToString, Enum##FromString>(archive, flag);
#define I_SERIALIZE_ENUM(Enum, archive, enumValue) iser::CPrimitiveTypesSerializer::SerializeEnum<Enum, ToString, FromString>(archive, enumValue);


} // namespace iser


#endif // !iser_CPrimitiveTypesSerializer_included

