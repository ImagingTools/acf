#include "iser/CBinaryWriteArchiveBase.h"


// Qt includes
#include <QtCore/QString>


// ACF includes
#include "iser/CArchiveTag.h"


namespace iser
{


// reimplemented (iser::IArchive)

bool CBinaryWriteArchiveBase::BeginTag(const CArchiveTag& tag)
{
	quint32 binaryId = tag.GetBinaryId();

	return Process(binaryId);
}


bool CBinaryWriteArchiveBase::EndTag(const CArchiveTag& tag)
{
	quint32 binaryId = ~tag.GetBinaryId();

	return Process(binaryId);
}


bool CBinaryWriteArchiveBase::Process(bool& value)
{
	return ProcessData(&value, int(sizeof(bool)));
}


bool CBinaryWriteArchiveBase::Process(char& value)
{
	return ProcessData(&value, int(sizeof(char)));
}


bool CBinaryWriteArchiveBase::Process(quint8& value)
{
	return ProcessData(&value, int(sizeof(quint8)));
}


bool CBinaryWriteArchiveBase::Process(qint8& value)
{
	return ProcessData(&value, int(sizeof(qint8)));
}


bool CBinaryWriteArchiveBase::Process(quint16& value)
{
	return ProcessData(&value, int(sizeof(quint16)));
}


bool CBinaryWriteArchiveBase::Process(qint16& value)
{
	return ProcessData(&value, int(sizeof(qint16)));
}


bool CBinaryWriteArchiveBase::Process(quint32& value)
{
	return ProcessData(&value, int(sizeof(quint32)));
}


bool CBinaryWriteArchiveBase::Process(qint32& value)
{
	return ProcessData(&value, int(sizeof(qint32)));
}


bool CBinaryWriteArchiveBase::Process(quint64& value)
{
	return ProcessData(&value, int(sizeof(quint64)));
}


bool CBinaryWriteArchiveBase::Process(qint64& value)
{
	return ProcessData(&value, int(sizeof(qint64)));
}


bool CBinaryWriteArchiveBase::Process(float& value)
{
	return ProcessData(&value, int(sizeof(float)));
}


bool CBinaryWriteArchiveBase::Process(double& value)
{
	return ProcessData(&value, int(sizeof(double)));
}


bool CBinaryWriteArchiveBase::Process(std::string& value)
{			
	int stringLength = value.size();

	bool retVal = Process(stringLength);

	retVal = retVal && ProcessData((void*)value.c_str(), stringLength * int(sizeof(char)));

	return retVal;
}


bool CBinaryWriteArchiveBase::Process(QString& value)
{
	int stringLength = value.size();

	bool retVal = Process(stringLength);

	retVal = retVal && ProcessData((void*)value.toStdWString().c_str(), stringLength * int(sizeof(wchar_t)));	

	return retVal;
}


// protected methods

CBinaryWriteArchiveBase::CBinaryWriteArchiveBase(const IVersionInfo* versionInfoPtr)
:	BaseClass(versionInfoPtr)
{
}


} // namespace iser


