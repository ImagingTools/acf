#ifndef iqt_CTrace_included
#define iqt_CTrace_included


// STL includes
#include <set>


// ACF includes
#include "istd/istd.h"
#include "istd/ITrace.h"


namespace iqt
{


class CTrace: virtual public istd::ITrace
{
public:
	CTrace();

	// reimplemented (istd::ITrace)
	virtual void LogMessage(
				istd::TraceLevel level,
				const std::string& groupId,
				const QString& message,
				const std::string& fileName = std::string(),
				int line = -1) const;
	virtual bool IsTraceEnabled(istd::TraceLevel level, const std::string& groupId) const;
	virtual void RegisterGroupId(const std::string& groupId);

private:
	typedef std::set<std::string> GroupIds;

	GroupIds m_groupIds;
};


} // namespace iqt


#endif // !iqt_CTrace_included


