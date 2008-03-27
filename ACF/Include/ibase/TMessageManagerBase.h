#ifndef ibase_TMessageManagerBase_included
#define ibase_TMessageManagerBase_included


#include "ibase/IMessage.h"

#include "iser/IArchive.h"

#include "imod/TMultiModelObserverBase.h"


namespace ibase
{		

template <class BaseClass>
class TMessageManagerBase:	public imod::TMultiModelObserverBase<ibase::IMessage>,
							public BaseClass

{
public:
	TMessageManagerBase();
	virtual ~TMessageManagerBase();

	// pseudo-reimplemented (ibase::IMessageManager)
	virtual int GetWorstCategory() const;
	virtual int GetMessageCount(int messageCategory) const;
	virtual const ibase::IMessage* GetMessageFrom(int index, int messageCategory = -1) const;
	virtual void AddMessage(ibase::IMessage* message);
	virtual void ClearMessages();

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	int m_maxCategory;

};


template <class BaseClass>
TMessageManagerBase<BaseClass>::TMessageManagerBase()
{
	m_maxCategory = 0;
}


template <class BaseClass>
TMessageManagerBase<BaseClass>::~TMessageManagerBase()
{
	ClearMessages();
}


// pseudo-reimplemented (ibase::IMessageManager)

template <class BaseClass>
int TMessageManagerBase<BaseClass>::GetWorstCategory() const
{
	return m_maxCategory;
}


template <class BaseClass>
int TMessageManagerBase<BaseClass>::GetMessageCount(int messageCategory) const
{
	int count = 0;
	for (int index = 0; index < GetModelCount(); index++){
		IMessage* messagePtr = GetObjectPtr(index);
		I_ASSERT(messagePtr != NULL);

		if (messageCategory == messagePtr->GetCategory()){
			++count;
		}
	}

	return count;
}


template <class BaseClass>
const ibase::IMessage* TMessageManagerBase<BaseClass>::GetMessageFrom(int index, int messageCategory) const
{
	int count = 0;
	for (int messageIndex = 0; messageIndex < GetModelCount(); messageIndex++){
		IMessage* messagePtr = GetObjectPtr(messageIndex);
		I_ASSERT(messagePtr != NULL);

		if (messageCategory == messagePtr->GetCategory()){
			++count;
		}

		if (count == index){
			return messagePtr;
		}
	}

	return NULL;
}


template <class BaseClass>
void TMessageManagerBase<BaseClass>::AddMessage(ibase::IMessage* messagePtr)
{
	I_ASSERT(messagePtr != NULL);

	imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(messagePtr);
	I_ASSERT(modelPtr != NULL);

	if (modelPtr != NULL){
		modelPtr->DetachAllObservers();

		if (modelPtr != NULL){
			if (modelPtr->AttachObserver(this)){
				if (messagePtr->GetCategory() > m_maxCategory){
					m_maxCategory = messagePtr->GetCategory() & ~ibase::IMessage::Debug;
				}
			}
		}
	}
}


template <class BaseClass>
void TMessageManagerBase<BaseClass>::ClearMessages()
{
	while(int modelCount = GetModelCount()){
		for (int messageIndex = 0; messageIndex < modelCount; messageIndex++){
			imod::IModel* modelPtr = GetModelPtr(messageIndex);
			if (modelPtr != NULL){
				modelPtr->DetachObserver(this);

				delete modelPtr;

				break;
			}
		}
	}

	m_maxCategory = 0;
}


// reimplemented (iser::ISerializable)

template <class BaseClass>
bool TMessageManagerBase<BaseClass>::Serialize(iser::IArchive& archive)
{
	return true;
}


} // namespace ibase


#endif // !ibase_TMessageManagerBase_included