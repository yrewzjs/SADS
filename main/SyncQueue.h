#ifndef __SYNC_QUEUE_H__
#define __SYNC_QUEUE_H__
#include <queue>
#include "Synchronized.h"

template <class Type>
class SyncQueue : public Synchronized
{
public:
	SyncQueue(void);
	virtual ~SyncQueue(void);
	bool PushBackEv(Type* ev);
	Type* PopFrontEv();
    void ClearAll();
    size_t Size();
private:
	std::queue<Type*> queue_;
};

template <class Type>
SyncQueue<Type>::SyncQueue()
{
}

template <class Type>
SyncQueue<Type>::~SyncQueue()
{
}

template <class Type>
Type* SyncQueue<Type>::PopFrontEv()
{
	Lock();
	while (queue_.empty())
	{
		try
		{
			Wait();
		}
		catch (std::exception* e)
		{
			//std::cout<< e->what()<< std::endl;  
			const char* emsg = e->what();
			throw;
		}
	}

	Type* ev = queue_.front();
	queue_.pop();
	UnLock();

	return ev;
}

template <class Type>
bool SyncQueue<Type>::PushBackEv(Type *ev)
{
	Lock();
	queue_.push(ev);
	try
	{
		Notify();
	}
	catch (std::exception* e)
	{
		//std::cout<< e->what()<< std::endl;  
		throw;  
	}
	UnLock();

	return true;
}

template <class Type>
size_t SyncQueue<Type>::Size()
{
    Lock();
    size_t size = queue_.size();
    UnLock();

    return size;
}

template <class Type>
void SyncQueue<Type>::ClearAll()
{
    Lock();
    while (!queue_.empty())
    {
        Type* ev = queue_.front();
        queue_.pop();
        delete ev;
    }

    UnLock();
}


#endif /*__SYNC_QUEUE_H__*/
