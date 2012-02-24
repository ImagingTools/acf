#include "iqt/CSystemEnvironment.h"


// Qt includes
#include <QUuid>
#include <QDir>


namespace iqt
{


// reimplemented (isys::ISystemEnvironment)

QString CSystemEnvironment::GetUser() const
{
	return GetSystemVariableValue("USER");
}


QString CSystemEnvironment::GetTempDirPath() const
{
	return QDir::tempPath();
}


QString CSystemEnvironment::GetUniqueIdentifier() const
{
	return QUuid::createUuid().toString();
}


} // namespace isys


