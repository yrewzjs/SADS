#include "stdafx.h"

#include "Synchronized.h"

Synchronized::Synchronized(void)
{
#ifdef WIN32
	// Semaphore initially auto signaled, auto reset mode, unamed
	event_ = CreateEvent(0, false, false, 0);

	// Semaphore initially unowed, unamed
	mutex_ = CreateMutex(0, false, 0);

	locked_ = false;

	notifies_ = 0;
#else
	memset(&mutex_, 0, sizeof(mutex_));
	int result = pthread_mutex_init(&monitor, 0);
	if (result)
	{
		throw std::runtime_error("Synchronized mutex_init failed!");
	}

	memset(&cond_, 0, sizeof(cond_));
	result = pthread_cond_init(&cond_, 0);
	if (result)
	{
		throw std::runtime_error("Synchronized cond_init failed!");
	}
#endif
}


Synchronized::~Synchronized(void)
{
#ifdef WIN32
	CloseHandle(event_);
	CloseHandle(mutex_);
#else
	pthread_cond_destroy(&cond_);
	pthread_mutex_destroy(&mutex_);
#endif
}


void Synchronized::Wait()
{
#ifdef WIN32
	Wait(INFINITE);
#else
	cond_timed_wait(0);
#endif
}

#ifndef WIN32
int Synchronized::cond_timed_wait(const timespec *timeout)
{
	int result = 0;
	if (timeout)
	{
		result = std::pthread_cond_timedwait(&cond_, &mutex_, timeout);
	}
	else
	{
		result = std::pthread_cond_wait(&cond_, &mutex_);
	}

	return result;
}
#endif


bool Synchronized::Wait(unsigned long timeout)
{
	bool timeout_occurred = false;
#ifdef WIN32
	locked_ = false;
	if (!ReleaseMutex(mutex_))
	{
		throw std::runtime_error("Synchronized: releasing mutex failed");  
	}

	int error = WaitForSingleObject(event_, timeout);
	switch(error)
	{
	case WAIT_TIMEOUT:
		timeout_occurred = true;
		break;
	case WAIT_ABANDONED:
		throw std::runtime_error("Synchronized: waiting for event failed");
		timeout_occurred = false;
		break;
	default:
		break;
	}

	if (WAIT_OBJECT_0 != WaitForSingleObject(mutex_, INFINITE))
	{
		throw std::runtime_error("Synchronized: waiting for mutex failed");
	}

	locked_ = true;
#else
	struct timespec ts;
	struct timeval tv;
	gettimeofday(&tv, 0);
	ts.tv_sec = tv.tv_sec + (int)timeout / 1000;
	ts.tv_nsec = (tv.tv_usec + (timeout % 1000) * 1000) * 1000;

	int error = cond_timed_wait(&ts);

	if (error > 0)
	{
		switch(error)
		{
		case ETIMEDOUT:
			timeout_occurred = true;
			break;
		default:
			throw std::runtime_error("Synchronized: wait with timeout returned!");
			break;
		}
	}
#endif

	return timeout_occurred;
}

void Synchronized::Notify()
{
#ifdef WIN32
	notifies_ = 1;
	if (!SetEvent(event_))
	{
		throw std::runtime_error("Synchronized: notify failed!");
	}
#else
	int result = pthread_cond_signal(&cond_);
	if (result)
	{
		throw std::runtime_error("Synchronized: notify failed!");
	}
#endif
}

void Synchronized::NotifyAll()
{
#ifdef WIN32
	notifies_ = (char)0x80;
	while (notifies_--)
	{
		if (!SetEvent(event_))
		{
			throw std::runtime_error("Synchronized: notify all failed!");
		}
	}
#else
	int result = pthread_cond_broadcast(&cond_);
	if (result)
	{
		throw std::runtime_error("Synchronized: notify all failed!");
	}
#endif
}

void Synchronized::Lock()
{
#ifdef WIN32
	if (WAIT_OBJECT_0 != WaitForSingleObject(mutex_, INFINITE))
	{
		throw std::runtime_error("Synchronized: lock failed!");
		return;
	}

	if (locked_)
	{
		// This thread owns already the lock, but  
		// we do not like recursive locking. Thus  
		// release it immediately and print a warning!  
		if(!ReleaseMutex(mutex_))  
		{  
			throw std::runtime_error("Synchronized: unlock failed!");  
		}        
		throw std::runtime_error("Synchronized: recursive locking detected!"); 
	}

	locked_ = true;
#else
	pthread_mutex_lock(&mutex_);
#endif
}

void Synchronized::UnLock()
{
#ifdef WIN32
	locked_ = false;
	if (!ReleaseMutex(mutex_))
	{
		throw std::runtime_error("Synchronized: unlock failed!");
		return;
	}
#else
	pthread_mutex_unlock(&mutex_);
#endif
}

bool Synchronized::TryLock()
{
#ifdef WIN32
	if (WAIT_OBJECT_0 != WaitForSingleObject(mutex_, 0))
	{
		throw std::runtime_error("Synchronized: try lock failed!");
		return false;
	}

	if (locked_)
	{
		if (!ReleaseMutex(mutex_))
		{
			throw std::runtime_error("Synchronized: unlock failed!");
		}
		return false;
	}
	else
	{
		locked_ = true;
	}

	return true;
#else
	if(0 == pthread_mutex_trylock(&monitor))
	{
		return true;
	}
	else
	{
		return false;
	}
#endif
}
