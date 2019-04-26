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
	//真正的工作区
	CString recvStr;
	int nIndex;
	int ret;
	//开始监听
	while (true)
	{
		/*
		等待网络事件触发事件对象句柄的工作状态,任何一个事件对象进入“已传信”状态，函数就会返回
		此处的WSAWaitForMultipleEvents的作用是检测 是否有信号，一有信号就会返回
		*/
		nIndex = ::WSAWaitForMultipleEvents(nEventTotal, eventArray, FALSE, 5000, FALSE);
		if (GetExit())//检查是否需要结束线程
			break;

		if (nIndex == WSA_WAIT_FAILED || nIndex == WSA_WAIT_TIMEOUT)
			continue;//没有触发事件，继续等待
		/*
		如果同时有多个内核对象被触发，WSAWaitForMultipleEvents函数返回的只是其中序号最小的那个
		因此从该序号位置的事件及以后的事件，每一个都重新检测，以确定序号大于返回序号的每个事件的状态
		*/
		nIndex = nIndex - WSA_WAIT_EVENT_0;
		for (int i = nIndex; i<nEventTotal; i++)
		{
			/*
			第二个的等待时间设置为0，起检测作用
			超过规定的时间，函数就会立即返回WSA_WAIT_TIMEOUT
			*/
			ret = ::WSAWaitForMultipleEvents(1, &eventArray[i], TRUE, 0, FALSE);
			if (ret == WSA_WAIT_FAILED || ret == WSA_WAIT_TIMEOUT)
			{
				continue;
			}
			else
			{
				/*
				检测出是哪个事件  “已传信”
				WSAEnumNetworkEvents函数会自动重置受信事件,负责将工作状态从已传信更改为未传信
				所以此处不需要手动重置
				*/
				WSANETWORKEVENTS event;
				::WSAEnumNetworkEvents(sockArray[i], eventArray[i], &event);
				/*
				根据event绑定的socket判定出是哪个sensor请求的数据
				*/
				Sensor* pSensor = m_pSensorManager->GetCurrentSensor(sockArray[i]);

				if (event.lNetworkEvents & FD_READ) // 处理FD_READ通知消息  
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
					//收到该次请求命令的数据，发信通知已经收到
					pSensor->ReceivedSignal();
				}
				else if (event.lNetworkEvents & FD_CLOSE)  // 处理FD_CLOSE通知消息, 连接超时，处于关闭状态，由Shutdown或者closesocket触发
				{
					if (event.iErrorCode[FD_CLOSE_BIT] == 0)
					{
						//该方法隐藏了实现细节
						pSensor->BreakConnect();
						//将该event后的元素均向前移动一位，相当于删除掉当前socket和event。
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
	//传入SOCKET管理者
	m_pSensorManager = soma;
	//初始化事件句柄和套节字句柄列表。
	Sensor* pSen = nullptr;
	map<int, Sensor*>::iterator iter = m_pSensorManager->GetSensorMap()->begin();
	while (iter != m_pSensorManager->GetSensorMap()->end())
	{
		pSen = iter->second;
		if (((SensorKeyence*)pSen)->GetIsVice())
		{
			iter++; //更新迭代器
			continue;
		}
			
		eventArray[nEventTotal] = pSen->GetWsaEvent();
		sockArray[nEventTotal] = pSen->GetSocket();
		nEventTotal++;

		iter++; //更新迭代器
	}

}
