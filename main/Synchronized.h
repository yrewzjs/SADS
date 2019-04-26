#ifndef __SYNCHRONIZED_H__
#define __SYNCHRONIZED_H__

#ifdef WIN32  
#include <Windows.h>  
#else  
#include <pthread.h>  
#include <unistd.h>  
#endif  
#include <errno.h>  
#include <time.h>  
#include <sys/types.h>
#include <stdexcept>

class Synchronized
{
public:
	Synchronized(void);

	virtual ~Synchronized(void);

	// Cause current thread to wait until another thread 
	// invokes the Notify() method or the NotifyAll()
	// method for this object.
	void Wait();

	// Cause current thread to wait until another thread 
	// invokes the Notify() method or the NotifyAll()
	// method for this object, or a specified amount of time
	// has elapsed.
	// @param timeout
	// timeout in milliseconds.
	// @return true if timeout occured, false otherwise.
	bool Wait(unsigned long timeout);

	// Wakes up a single thread that is waiting on this object's monitor.
	void Notify();

	// Wakes up all threads that are waiting on this object's monitor.
	void NotifyAll();

	// Enter a critical section.
	void Lock();

	// Try to enter a critical section.
	// @return  true if the attempt was successful, false otherwise.
	bool TryLock();

	// Leave a critical section.
	void UnLock();

private:
#ifdef WIN32
	char notifies_;

	HANDLE event_;

	HANDLE mutex_;

	bool locked_;

#else
	int cond_timed_wait(const timespec* timeout);

	pthread_cond_t cond_;

	pthread_mutex_t mutex_;
#endif
};

#endif /*__SYNCHRONIZED_H__*/
