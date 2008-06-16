#ifndef imebase_CMeContext_included
#define imebase_CMeContext_included


#include <QMutex>
#include <QWaitCondition>

#include "isig/ISamplesContainer.h"

#include "imebase.h"


namespace imebase
{


/**
	Context for data processing
*/
class CMeContext
{
public:
	CMeContext(const CMeAddr& address, int id, bool isOutput, isig::ISamplesContainer* containerPtr);
	~CMeContext();

	/**
		Register this context to MEiDS
	*/
	bool Register(double interval);
	void Unregister();

	int GetId()  const;

	/**
		Check buffer status.
	*/
	int GetCount()  const;
	bool IsDone();
	double GetInterval() const;

	/**
		Wait for task to end. If time out return false.
		\param	timeout	time out in seconds.
	*/
	bool Wait(double timeout);

	void CopyToContainer();
	void CopyFromContainer();

protected:
	bool ConfigInputStream();
	bool ConfigOutputStream();

	bool StartStream();

	// static methods
	static int __stdcall cbAIFunc(int device, int subdevice, int count, void* context, int error);
	static int __stdcall cbAOFunc(int device, int subdevice, int count, void* context, int error);

private:
	QMutex m_activeTaskMutex;
	QWaitCondition m_dataReadyCondition;

	CMeAddr m_address;
	int m_id;
	int m_bufferCount;
	std::vector<int> m_hwBuffer;
	bool m_isOutput;
	double m_interval;

	isig::ISamplesContainer& m_samplesContainer;
};


} // namespace imebase


#endif // !imebase_CMeContext_included


