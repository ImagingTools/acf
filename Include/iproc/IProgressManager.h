#ifndef iproc_IProgressManager_included
#define iproc_IProgressManager_included


// STL includes
#include <string>

#include "istd/IPolymorphic.h"
#include <QString>


namespace iproc
{


/**
	Consume information about progress of some process.
*/
class IProgressManager: virtual public istd::IPolymorphic
{
public:
	/**
		Begin progress report session.
		\return	session ID or negative value if failed.
	*/
	virtual int BeginProgressSession(
				const std::string& progressId,
				const QString& description,
				bool isCancelable = false) = 0;
	/**
		Close progress report session.
		\param	sessionId	session ID returned by \c BeginProgressSession.
	*/
	virtual void EndProgressSession(int sessionId) = 0;
	/**
		Callback function for an progress event.
		\param	sessionId	session ID returned by \c BeginProgressSession.
	*/
	virtual void OnProgress(int sessionId, double currentProgress) = 0;

	/**
		Check if this processing operation should be canceled.
		\param	sessionId	session ID returned by \c BeginProgressSession.
	*/
	virtual bool IsCanceled(int sessionId) const = 0;
};


} // namespace iproc


#endif // !iproc_IProgressManager_included


