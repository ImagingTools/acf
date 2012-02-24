#include "isys/CSystemEnvironmentBase.h"


// STD includes
#include <cstdlib>


namespace isys
{


// reimplemented (isys::ISystemEnvironment)

QString CSystemEnvironmentBase::GetSystemVariableValue(const QString& variableName) const
{
	return QString(::getenv(variableName.toStdString().c_str()));
}


} // namespace isys


