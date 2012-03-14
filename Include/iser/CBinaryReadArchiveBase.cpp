#include "iser/CBinaryReadArchiveBase.h"


// STL includes
#include <vector>

// Qt includes
#include <QtCore/QObject>
#include <QtCore/QString>

// ACF includes
#include "iser/CArchiveTag.h"


namespace iser
{


bool CBinaryReadArchiveBase::BeginTag(const CArchiveTag& tag)
{
	I_DWORD readId;
	bool retVal = Process(readId);

	if (!retVal){
		return false;
	}

	if (readId != tag.GetBinaryId()){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformation::IC_ERROR,
						MI_TAG_ERROR,
						QObject::tr("Bad tag begin code, is %1, should be %2 (tag '%3')").arg(readId).arg(tag.GetBinaryId()).arg(QString::fromStdString(tag.GetId())),
						"iser::CBinaryReadArchiveBase",
						istd::IInformation::ITF_SYSTEM);
		}

		return false;
	}

	return true;
}


bool CBinaryReadArchiveBase::EndTag(const CArchiveTag& tag)
{
	I_DWORD readId;
	bool retVal = Process(readId);

	if (!retVal){
		return false;
	}

	if (~readId != tag.GetBinaryId()){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformation::IC_ERROR,
						MI_TAG_ERROR,
						QObject::tr("Bad tag begin code, is %1, should be %2 (tag '%3')").arg(~readId).arg(tag.GetBinaryId()).arg(QString::fromStdString(tag.GetId())),
						"iser::CBinaryReadArchiveBase",
						istd::IInformation::ITF_SYSTEM);
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


bool CBinaryReadArchiveBase::Process(I_BYTE& value)
{
	return ProcessData(&value, int(sizeof(I_BYTE)));
}


bool CBinaryReadArchiveBase::Process(I_SBYTE& value)
{
	return ProcessData(&value, int(sizeof(I_SBYTE)));
}


bool CBinaryReadArchiveBase::Process(I_WORD& value)
{
	return ProcessData(&value, int(sizeof(I_WORD)));
}


bool CBinaryReadArchiveBase::Process(I_SWORD& value)
{
	return ProcessData(&value, int(sizeof(I_SWORD)));
}


bool CBinaryReadArchiveBase::Process(I_DWORD& value)
{
	return ProcessData(&value, int(sizeof(I_DWORD)));
}


bool CBinaryReadArchiveBase::Process(I_SDWORD& value)
{
	return ProcessData(&value, int(sizeof(I_SDWORD)));
}


bool CBinaryReadArchiveBase::Process(I_QWORD& value)
{
	return ProcessData(&value, int(sizeof(I_QWORD)));
}


bool CBinaryReadArchiveBase::Process(I_SQWORD& value)
{
	return ProcessData(&value, int(sizeof(I_SQWORD)));
}


bool CBinaryReadArchiveBase::Process(float& value)
{
	return ProcessData(&value, int(sizeof(float)));
}


bool CBinaryReadArchiveBase::Process(double& value)
{
	return ProcessData(&value, int(sizeof(double)));
}


bool CBinaryReadArchiveBase::Process(std::string& value)
{			
	int stringLength;

	bool retVal = Process(stringLength);

	if (retVal){
		if (stringLength > 0) {
			if (stringLength > MaxStringLength){
				if (IsLogConsumed()){
					SendLogMessage(
								istd::IInformation::IC_ERROR,
								MI_STRING_TOO_LONG,
								QString("Read string size is ") + QString("%1").arg(stringLength) + " and it is longer than maximum size",
								"iser::CBinaryReadArchiveBase",
								istd::IInformation::ITF_SYSTEM);
				}

				return false;
			}

			std::vector<char> buffer(stringLength + 1, 0);

			retVal = ProcessData(&buffer[0], stringLength * int(sizeof(char)));	

			if (retVal){
				value = std::string(&buffer[0]);
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
								istd::IInformation::IC_ERROR,
								MI_STRING_TOO_LONG,
								QString("Read string size is ") + QString("%1").arg(stringLength) + " and it is longer than maximum size",
								"iser::CBinaryReadArchiveBase",
								istd::IInformation::ITF_SYSTEM);
				}

				return false;
			}

			std::vector<wchar_t> buffer(stringLength + 1, 0);

			retVal = ProcessData(&buffer[0], stringLength * int(sizeof(wchar_t)));	

			if (retVal){
				value = QString::fromStdWString(std::wstring(&buffer[0]));
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

