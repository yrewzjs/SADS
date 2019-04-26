#ifndef _WORKINGTHREAD_H_
#define _WORKINGTHREAD_H_

#include <string>
using std::string;
#include "Thread.h"
#include "SyncQueue.h"
#include "MXComonentDef.h"

/***
	type    ��������
	 0        ϵͳ
	 1        ���
	 2        ͼ��
	 3        PLC
	 4        �Զ�ƴװ
*/
struct transit_struct
{
	int type;   // ��������
	int funcId; // �������µĵڼ�������
	/* ��������ָ�� */
	void*  pointer;  // void *��Ϊ��������ָ�롱��ָ���κ����͵�����,����ǿתʹ�� 
	int value;  // �洢��ֵ
};


class WorkingThread:public CThread
{
public:
	WorkingThread();
	~WorkingThread();

private:
	class LaserDriver* m_pLaser;   // ���ģ����
	class CamaraModule* m_pCamera;  // ���ģ����
	class MxWrapper* m_pMxWrapper; // PLCģ��ͨ�ž��

	class VideoThread* m_pVideoThread; //��Ƶ�߳�
	class PlcMonitorThread* m_pPlcMonitorThread; // plc������߳�
	class AdjustThread* m_pAdjustThread;  // �����߳�

	SyncQueue<transit_struct>* m_pSyncQueue;  //�������

	volatile bool m_isOpen; // �Ƿ񿪻��ɹ�

public:
	void Run(); //�߳����

	class MxWrapper* getMxWrapper();
private:

public:
	//����ģʽӦ����һ���ȽϺõĽ������
	// d2w  displayToWork
	/* 
	  type = 0    ϵͳ����
	  funcId    ������ 
	    0      d2w_open
		1      d2w_close
		2      d2w_stopAuto
		3      d2w_exit
	*/
	void d2w_open();   //  ����  
	void d2w_close();  // �ػ�
	void d2w_stopAuto();  // ֹͣ�Զ�ƴװ
	void d2w_exit();   // �˳�
	/*
		type = 1    ���ģ��
		funcId    ������
		0      d2w_caliLaser
	*/
	void d2w_caliLaser(int caliId);  //�궨����
	
	/*
		type = 2    ͼ��ģ��
		funcId      ������
		0      d2w_caliCamera
		1      d2w_takePhoto
		2      d2w_openVideo
	*/
	void d2w_caliCamera(int caliId); //�궨���
	void d2w_takePhoto(int camId);  // ��һ��
	void d2w_openVideo(int camId);  // ��Ƶģʽ
	void d2w_closeVideo();          // �ر���Ƶ
	/*
		type = 3    PLCģ��
		funcId      ������
		0      d2w_caliPlc
		1      d2w_inputPlc
	*/
	void d2w_caliPlc(wplc_struct* wplc);  //�궨PLC    wplc������ڴ���clearAllʱ�����ڴ�й©�ķ���
	void d2w_inputPlc(wplc_struct* wplc); //PLC����

private:
	/*   ϵͳ	*/
	void w2d_open();   //  ����
	void w2d_close();  // �ػ�
	void w2d_stopAuto();  // ֹͣ�Զ�ƴװ
	void w2d_exit();   // �˳�

	/*   ���ģ��	*/
	void w2d_caliLaser(int caliId);  //�궨����

	/*   ͼ��ģ�� 	*/
	void w2d_caliCamera(int caliId); //�궨���
	void w2d_takePhoto(int camId);  // ��һ��
	void w2d_openVideo(int camId);  // ��Ƶģʽ
	void w2d_closeVideo();          // �ر���Ƶ

	/*   PLCģ��   */
	void w2d_caliPlc(wplc_struct* wplc);  //�궨PLC
	void w2d_inputPlc(wplc_struct* wplc); //PLC����
};

#endif