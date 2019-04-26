#ifndef _CTHREAD_H_
#define _CTHREAD_H_
/**
* �������Ҫ�����Ǵ���һ���̡߳�
* @author Mostly from the Internet, I only made partial changes
* @Time 2017
*/

class SyncCondition;

//�߳�״̬
typedef enum ThreadState
{
	THREAD_NEW = 0x0,//�½���������δ�������̴߳�������״̬��
	THREAD_RUNNABLE = 0x1, //���У�����ִ�е��̴߳�������״̬�� 
	THREAD_TERMINATED = 0X2,//���������˳����̴߳�������״̬��
}ThreadState;

//�̻߳���
class CThread
{
public:
	CThread();
	CThread(char* name);
	virtual ~CThread();

private:

	unsigned m_ThreadID; //�߳�ID���ڿ����߳�ʱ�����˸�ֵ

	char* m_ThreadName; //�߳�����

	volatile ThreadState m_ThreadState; //�߳�״̬

	HANDLE m_hthreadHandle; //�߳̾��

	volatile bool m_bExit;  //volatile����

	SyncCondition *m_pExitCond;
	
private:
	static unsigned __stdcall ThreadFunction(void*); //start���ô˺������˺����ٵ���run������ִ��ʵ�ʵ�����

public:
	//����Ӧ�̳в�ʵ�֣����򣬸÷�����ִ���κβ��������ء�
	virtual void Run() = 0;//���麯��

	//ʹ���߳̿�ʼִ�У����ø��̵߳� run ������
	void Start();

	//�ж��̡߳�
//	void interrupt();

	//�ȴ����߳���ֹ��
	void join();
	void join(long sec);

	//�����߳�״̬
	void SetThreadState(ThreadState state);
	ThreadState GetThreadState();
	
	//��ȡ�߳�ID
	int GetThreadID(void);

	//�߳�����
	void SetThreadName(char* threadName);
	char* GetThreadName();

	//�߳̾��
	HANDLE GetThreadHandle();
	void  SetThreadHandle(HANDLE hdl);

	//�˳���־
	void SetExit(bool _exit);
	bool GetExit();

	//�����߳��Ƿ��ڻ״̬��
	bool  IsAlive(); 

	//�����߳��Ƿ��Ѿ��жϡ�
	bool isInterrupted();
};

#endif