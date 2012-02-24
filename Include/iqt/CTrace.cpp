#include "iqt/CTrace.h"


// Qt includes
#include <QString>


// ACF includes
#include "iqt/iqt.h"


namespace iqt
{


// public methods

CTrace::CTrace()
{
	RegisterGroupId("assert");
	RegisterGroupId("info");
}


// reimplemented (istd::ITrace)

void CTrace::LogMessage(
			istd::TraceLevel level,
			const std::string& groupId,
			const QString& message,
			const std::string& fileName,
			int line) const
{
	if (IsTraceEnabled(level, groupId)){
		QString traceMessage = message;
		if (!fileName.empty() && line >= 0){
			QString fileMessage = QString ("%1, Line: %2: ").arg(fileName.c_str()).arg(line);

			traceMessage = fileMessage + traceMessage;
		}

		qDebug(traceMessage.toLatin1().constData());
	}
}


bool CTrace::IsTraceEnabled(istd::TraceLevel /*level*/, const std::string& groupId) const
{
	if (m_groupIds.find(groupId) != m_groupIds.end()){
		return true;
	}

	return false;
}


void CTrace::RegisterGroupId(const std::string& groupId)
{
	m_groupIds.insert(groupId);
}


} // namespace iqt


