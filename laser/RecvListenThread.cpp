#include "stdafx.h"

#include "RecvListenThread.h"
#include "SensorManager.h"
#include "Sensor.h"
#include "SensorKeyence.h"
#include "LaserDef.h"

RecvListenThread::RecvListenThread()
{
	nEventTotal = 0;
}

RecvListenThread::~RecvListenThread()
{
	
}

void RecvListenThread::Run()
{
	//�����Ĺ�����
	CString recvStr;
	int nIndex;
	int ret;
	//��ʼ����
	while (true)
	{
		/*
		�ȴ������¼������¼��������Ĺ���״̬,�κ�һ���¼�������롰�Ѵ��š�״̬�������ͻ᷵��
		�˴���WSAWaitForMultipleEvents�������Ǽ�� �Ƿ����źţ�һ���źžͻ᷵��
		*/
		nIndex = ::WSAWaitForMultipleEvents(nEventTotal, eventArray, FALSE, 5000, FALSE);
		if (GetExit())//����Ƿ���Ҫ�����߳�
			break;

		if (nIndex == WSA_WAIT_FAILED || nIndex == WSA_WAIT_TIMEOUT)
			continue;//û�д����¼��������ȴ�
		/*
		���ͬʱ�ж���ں˶��󱻴�����WSAWaitForMultipleEvents�������ص�ֻ�����������С���Ǹ�
		��˴Ӹ����λ�õ��¼����Ժ���¼���ÿһ�������¼�⣬��ȷ����Ŵ��ڷ�����ŵ�ÿ���¼���״̬
		*/
		nIndex = nIndex - WSA_WAIT_EVENT_0;
		for (int i = nIndex; i<nEventTotal; i++)
		{
			/*
			�ڶ����ĵȴ�ʱ������Ϊ0����������
			�����涨��ʱ�䣬�����ͻ���������WSA_WAIT_TIMEOUT
			*/
			ret = ::WSAWaitForMultipleEvents(1, &eventArray[i], TRUE, 0, FALSE);
			if (ret == WSA_WAIT_FAILED || ret == WSA_WAIT_TIMEOUT)
			{
				continue;
			}
			else
			{
				/*
				�������ĸ��¼�  ���Ѵ��š�
				WSAEnumNetworkEvents�������Զ����������¼�,���𽫹���״̬���Ѵ��Ÿ���Ϊδ����
				���Դ˴�����Ҫ�ֶ�����
				*/
				WSANETWORKEVENTS event;
				::WSAEnumNetworkEvents(sockArray[i], eventArray[i], &event);
				/*
				����event�󶨵�socket�ж������ĸ�sensor���������
				*/
				Sensor* pSensor = m_pSensorManager->GetCurrentSensor(sockArray[i]);

				if (event.lNetworkEvents & FD_READ) // ����FD_READ֪ͨ��Ϣ  
				{
					if (event.iErrorCode[FD_READ_BIT] == 0)
					{
						char szText[1024] = { 0 };
						int bufferSize = 1024;
						recvStr = _T("");
						int nRecv = ::recv(sockArray[i], szText, bufferSize, 0);
						if (nRecv >= 0)
						{
							recvStr = CA2CT(szText);
							pSensor->SetResponse(recvStr);
						}
					}
					else
					{
						pSensor->SetResponse(SENSOR_REQ_ERROR);
					}
					//�յ��ô�������������ݣ�����֪ͨ�Ѿ��յ�
					pSensor->ReceivedSignal();
				}
				else if (event.lNetworkEvents & FD_CLOSE)  // ����FD_CLOSE֪ͨ��Ϣ, ���ӳ�ʱ�����ڹر�״̬����Shutdown����closesocket����
				{
					if (event.iErrorCode[FD_CLOSE_BIT] == 0)
					{
						//�÷���������ʵ��ϸ��
						pSensor->BreakConnect();
						//����event���Ԫ�ؾ���ǰ�ƶ�һλ���൱��ɾ������ǰsocket��event��
						for (int j = i; j<nEventTotal - 1; j++)
						{
							sockArray[j] = sockArray[j + 1];
							sockArray[j] = sockArray[j + 1];
						}
						nEventTotal--;
					}
				}
			}
		}
	}
}

void RecvListenThread::InitRecvListenThread(SensorManager* const soma)
{
	//����SOCKET������
	m_pSensorManager = soma;
	//��ʼ���¼�������׽��־���б�
	Sensor* pSen = nullptr;
	map<int, Sensor*>::iterator iter = m_pSensorManager->GetSensorMap()->begin();
	while (iter != m_pSensorManager->GetSensorMap()->end())
	{
		pSen = iter->second;
		if (((SensorKeyence*)pSen)->GetIsVice())
		{
			iter++; //���µ�����
			continue;
		}
			
		eventArray[nEventTotal] = pSen->GetWsaEvent();
		sockArray[nEventTotal] = pSen->GetSocket();
		nEventTotal++;

		iter++; //���µ�����
	}

}
