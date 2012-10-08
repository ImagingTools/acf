#ifndef ibase_CDelegatedProgressManager_included
#define ibase_CDelegatedProgressManager_included


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include "istd/IChangeable.h"

#include "ibase/IProgressManager.h"


namespace ibase
{


/**
	Progress manager delegating cumulated progress result to another manager.
*/
class CDelegatedProgressManager:
			virtual public ibase::IProgressManager,
			virtual public istd::IChangeable
{
public:
	enum ChangeFlags
	{
		CF_SESSIONS_NUMBER = 1 << 22,
		CF_PROGRESS_CHANGED = 1 << 23
	};

	/**
		Create progress manager without slave manager.
	*/
	CDelegatedProgressManager();
	/**
		Create progress manager with slave manager.
		\param	slaveManagerPtr	pointer to slave manager object.
		\param	progressId		ID of type of progress slave manager session.
		\param	description		Description of progress slave manager session.
	*/
	CDelegatedProgressManager(
				IProgressManager* slaveManagerPtr,
				const QByteArray& progressId,
				const QString& description,
				bool isCancelable = false);
	virtual ~CDelegatedProgressManager();

	double GetCumulatedProgress() const;

	// reimplemented (ibase::IProgressManager)
	virtual int BeginProgressSession(const QByteArray& progressId, const QString& description, bool isCancelable = false);
	virtual void EndProgressSession(int sessionId);
	virtual void OnProgress(int sessionId, double currentProgress);
	virtual bool IsCanceled(int sessionId) const;

protected:
	int GetOpenSessionsCount() const;

	/**
		Called when state of cancel operation has changed.
	*/
	virtual void OnCancelable(bool cancelState);

	// reimplemented (istd::IChangeable)
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

private:
	struct ProgressInfo
	{
		double progress;
		bool isCancelable;
	};

	typedef QMap<int, ProgressInfo> ProgressMap;

	ProgressMap m_progressMap;

	int m_nextSessionId;
	double m_progressSum;

	int m_cancelableSessionsCount;

	IProgressManager* m_slaveManagerPtr;
	int m_slaveSessionId;
};


} // namespace ibase


#endif // !ibase_CDelegatedProgressManager_included


