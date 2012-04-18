#include "iser/CBinaryReadArchiveBase.h"


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVarLengthArray>

// ACF includes
#include "iser/CArchiveTag.h"


namespace iser
{


bool CBinaryReadArchiveBase::BeginTag(const CArchiveTag& tag)
{
	quint32 readId;
	bool retVal = Process(readId);

	if (!retVal){
		return false;
	}

	if (readId != tag.GetBinaryId()){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QObject::tr("Bad tag begin code, is %1, should be %2 (tag '%3')").arg(readId).arg(tag.GetBinaryId()).arg(QString(tag.GetId())),
						"iser::CBinaryReadArchiveBase",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		return false;
	}

	return true;
}


bool CBinaryReadArchiveBase::EndTag(const CArchiveTag& tag)
{
	quint32 readId;
	bool retVal = Process(readId);

	if (!retVal){
		return false;
	}

	if (~readId != tag.GetBinaryId()){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QObject::tr("Bad tag begin code, is %1, should be %2 (tag '%3')").arg(~readId).arg(tag.GetBinaryId()).arg(QString(tag.GetId())),
						"iser::CBinaryReadArchiveBase",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		return false;
	}

	return true;
}


bool CBinaryReadArchiveBase::Process(bool& value)
{
	return ProcessData(&value, int(sizeof(bool)));
}


bool CBinaryReadArchiveBase::Process(char& value)
{
	return ProcessData(&value, int(sizeof(char)));
}


bool CBinaryReadArchiveBase::Process(quint8& value)
{
	return ProcessData(&value, int(sizeof(quint8)));
}


bool CBinaryReadArchiveBase::Process(qint8& value)
{
	return ProcessData(&value, int(sizeof(qint8)));
}


bool CBinaryReadArchiveBase::Process(quint16& value)
{
	return ProcessData(&value, int(sizeof(quint16)));
}


bool CBinaryReadArchiveBase::Process(qint16& value)
{
	return ProcessData(&value, int(sizeof(qint16)));
}


bool CBinaryReadArchiveBase::Process(quint32& value)
{
	return ProcessData(&value, int(sizeof(quint32)));
}


bool CBinaryReadArchiveBase::Process(qint32& value)
{
	return ProcessData(&value, int(sizeof(qint32)));
}


bool CBinaryReadArchiveBase::Process(quint64& value)
{
	return ProcessData(&value, int(sizeof(quint64)));
}


bool CBinaryReadArchiveBase::Process(qint64& value)
{
	return ProcessData(&value, int(sizeof(qint64)));
}


bool CBinaryReadArchiveBase::Process(float& value)
{
	return ProcessData(&value, int(sizeof(float)));
}


bool CBinaryReadArchiveBase::Process(double& value)
{
	return ProcessData(&value, int(sizeof(double)));
}


bool CBinaryReadArchiveBase::Process(QByteArray& value)
{			
	int stringLength;

	bool retVal = Process(stringLength);

	if (retVal){
		if (stringLength > 0) {
			if (stringLength > MaxStringLength){
				if (IsLogConsumed()){
					SendLogMessage(
								istd::IInformationProvider::IC_ERROR,
								MI_STRING_TOO_LONG,
								QString("Read string size is ") + QString("%1").arg(stringLength) + " and it is longer than maximum size",
								"iser::CBinaryReadArchiveBase",
								istd::IInformationProvider::ITF_SYSTEM);
				}

				return false;
			}

			QVarLengthArray<char> buffer(stringLength);

			retVal = ProcessData(buffer.data(), stringLength * int(sizeof(char)));	

			if (retVal){
				value = QByteArray(buffer.constData(), stringLength);
			}
		}
		else {
			// just clear the string
			value.clear();
		}
	}
	
	return retVal;
}


bool CBinaryReadArchiveBase::Process(QString& value)
{
	int stringLength;

	bool retVal = Process(stringLength);

	if (retVal){
		if (stringLength > 0) {
			if (stringLength > MaxStringLength){
				if (IsLogConsumed()){
					SendLogMessage(
								istd::IInformationProvider::IC_ERROR,
								MI_STRING_TOO_LONG,
								QString("Read string size is ") + QString("%1").arg(stringLength) + " and it is longer than maximum size",
								"iser::CBinaryReadArchiveBase",
								istd::IInformationProvider::ITF_SYSTEM);
				}

				return false;
			}

			QVarLengthArray<QChar> buffer(stringLength);

			retVal = ProcessData(buffer.data(), stringLength * int(sizeof(QChar)));	

			if (retVal){
				value = QString(buffer.constData(), stringLength);
			}
		}
		else {
			// just clear the string
			value.clear();
		}
	}

	return retVal;
}


} // namespace iser

