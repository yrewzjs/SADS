#ifndef _CTHREAD_H_
#define _CTHREAD_H_
/**
* 该类的主要功能是创建一个线程。
* @author Mostly from the Internet, I only made partial changes
* @Time 2017
*/

class SyncCondition;

//线程状态
typedef enum ThreadState
{
	THREAD_NEW = 0x0,//新建，至今尚未启动的线程处于这种状态。
	THREAD_RUNNABLE = 0x1, //运行，正在执行的线程处于这种状态。 
	THREAD_TERMINATED = 0X2,//死亡，已退出的线程处于这种状态。
}ThreadState;

//线程基类
class CThread
{
public:
	CThread();
	CThread(char* name);
	virtual ~CThread();

private:

	unsigned m_ThreadID; //线程ID，在开启线程时进行了赋值

	char* m_ThreadName; //线程名字

	volatile ThreadState m_ThreadState; //线程状态

	HANDLE m_hthreadHandle; //线程句柄

	volatile bool m_bExit;  //volatile修饰

	SyncCondition *m_pExitCond;
	
private:
	static unsigned __stdcall ThreadFunction(void*); //start调用此函数，此函数再调用run函数，执行实际的任务

public:
	//子类应继承并实现，否则，该方法不执行任何操作并返回。
	virtual void Run() = 0;//纯虚函数

	//使该线程开始执行；调用该线程的 run 方法。
	void Start();

	//中断线程。
//	void interrupt();

	//等待该线程终止。
	void join();
	void join(long sec);

	//设置线程状态
	void SetThreadState(ThreadState state);
	ThreadState GetThreadState();
	
	//获取线程ID
	int GetThreadID(void);

	//线程名字
	void SetThreadName(char* threadName);
	char* GetThreadName();

	//线程句柄
	HANDLE GetThreadHandle();
	void  SetThreadHandle(HANDLE hdl);

	//退出标志
	void SetExit(bool _exit);
	bool GetExit();

	//测试线程是否处于活动状态。
	bool  IsAlive(); 

	//测试线程是否已经中断。
	bool isInterrupted();
};

#endif