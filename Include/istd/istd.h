#ifndef istd_included
#define istd_included


// STL includes
#include <limits>

// Qt includes
#include <QtCore/QtGlobal>
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QStringList>


/**
	Standard library.
	Contains the system indenendent primitives and basic implementations.

	\ingroup Main
*/
namespace istd
{
} // namespace istd


static const double I_BIG_EPSILON = 1.0e-8;

/**
	Some very small number.
*/
#define I_EPSILON std::numeric_limits<double>::epsilon()


/**
	Some very big number.
*/
#define I_HUGE_NUMBER 10e20


/**
	Infinite value.
*/
#define I_INFINITY std::numeric_limits<double>::infinity()


/**
	Commonly used symbols.
*/
static const QChar QCHAR_POW2 = QChar(0x00B2);
static const QChar QCHAR_POW3 = QChar(0x00B3);
static const QChar QCHAR_DEGREE = QChar(0x00B0);
static const QChar QCHAR_PERMILLE = QChar(0x2030);



#ifndef QT_NO_DEBUG


#define I_IF_DEBUG(instructions) instructions
#define I_CRITICAL() Q_ASSERT(false)


#else // !QT_NO_DEBUG


#define I_CRITICAL()
#define I_IF_DEBUG(instructions)


#endif // !QT_NO_DEBUG


#ifndef NULL
#define NULL 0
#endif // !NULL


/**
	Get all values of an enumeration as a list of intergers.
*/
#define I_ENUM_GET_VALUES(Enum, ...)\
	static QList<int> Enum##GetValues(){\
		QList<int> values;\
		int vars[] = {0, __VA_ARGS__};\
		int count = (sizeof(vars) / sizeof(int));\
		for(int i = 0; i < count; ++i){\
			if (i > 0){\
				values << vars[i];\
			}\
		}\
		return values;\
	}

/**
	Get all values of an enumeration as strings.
*/
#define I_ENUM_GET_STRINGS(Enum, ...)\
	static QStringList Enum##GetStrings(){\
		QString enumValuesString = #__VA_ARGS__;\
		QStringList values = enumValuesString.split(",");\
		for (int i = 0; i < values.count(); ++i){\
			QString rawValue = values[i].simplified();\
			QStringList splitNames = rawValue.split("_");\
			if (splitNames.isEmpty()){\
				values[i] = rawValue;\
			}\
			else{\
				QString formattedValue = rawValue;\
				for (int partIndex = 1; partIndex < splitNames.count(); ++partIndex){\
					QString partValue = splitNames[partIndex].toLower();\
					if (partIndex == 1){\
						formattedValue = partValue;\
					}\
					else{\
						formattedValue += partValue.at(0).toUpper() + partValue.mid(1);\
					}\
				}\
				values[i] = formattedValue;\
			}\
		}\
		return values;\
	}

/**
	Create meta information for an existing enumeration.
*/
#define I_DECLARE_ENUM(Enum, ...)\
	I_ENUM_GET_VALUES(Enum, __VA_ARGS__)\
	I_ENUM_GET_STRINGS(Enum, __VA_ARGS__)\
	static QByteArray ToString(Enum enumValue){\
		static QByteArray emptyString;\
		QStringList values = Enum##GetStrings();\
		QList<int> enumValues = Enum##GetValues();\
		Q_ASSERT(enumValues.count() == values.count());\
		for (int i = 0; i < enumValues.count(); ++i){\
			if (enumValues[i] == enumValue){\
				return values[i].toUtf8();\
			}\
		}\
		return emptyString;\
	}\
	static bool FromString(const QByteArray& enumString, Enum& enumValue){\
		QStringList values = Enum##GetStrings();\
		QList<int> enumValues = Enum##GetValues();\
		Q_ASSERT(enumValues.count() == values.count());\
		for (int i = 0; i < enumValues.count(); ++i){\
			if (values[i].toUtf8() == enumString){\
				enumValue = Enum(enumValues[i]);\
				return true;\
			}\
		}\
		return false;\
	}

#define I_DECLARE_FLAGS(Enum, ...)\
	I_ENUM_GET_VALUES(Enum, __VA_ARGS__)\
	I_ENUM_GET_STRINGS(Enum, __VA_ARGS__)\
	static QByteArray Enum##ToString(int flags){\
		QByteArray retVal;\
		QStringList values = Enum##GetStrings();\
		QList<int> enumValues = Enum##GetValues();\
		Q_ASSERT(enumValues.count() == values.count());\
		for (int i = 0; i < enumValues.count(); ++i){\
			if (enumValues[i] & flags){\
				if (!retVal.isEmpty()){\
					retVal += "|";\
				}\
				retVal += values[i].toUtf8();\
			}\
		}\
		return retVal;\
	}\
	static bool Enum##FromString(const QByteArray& enumString, int& flags){\
		QStringList values = Enum##GetStrings();\
		QList<int> enumValues = Enum##GetValues();\
		flags = 0;\
		Q_ASSERT(enumValues.count() == values.count());\
		QList<QByteArray> separatedValues = enumString.split('|');\
		for (int valueIndex = 0; valueIndex < separatedValues.count(); ++valueIndex){\
			QByteArray value = separatedValues[valueIndex];\
			for (int i = 0; i < enumValues.count(); ++i){\
				if (values[i].toUtf8() == value){\
					flags |= enumValues[i];\
				}\
			}\
		}\
		return true;\
	}


#endif // !istd_included

