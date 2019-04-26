#include "stdafx.h"
#include "SensorSick.h"
#include "IOSocket.h"
#include "SyncCondition.h"
#include "LaserDef.h"
#include "MessageProcess.h"

SensorSick::SensorSick()
{
}


SensorSick::~SensorSick()
{
	if (m_IoSocket != nullptr)
	{
		delete m_IoSocket;
		m_IoSocket = nullptr;
	}
}

void SensorSick::AcquireGroupResponse(CString& validData)
{
	double data;
	AcquireData(data);
	validData.Format(_T("%.3lf"), data);	
}

void SensorSick::AcquireData(double &data)
{
	const CString commend = _T("\2MEASURE\3");
	//设置请求命令
	SetRequest(commend);
	//该传感器请求数据
	CString recvData = RequestSensor();
	CString validData = _T("");
	if (recvData == SENSOR_REQ_ERROR)
	{
		validData = SENSOR_REQ_ERROR;
	}
	else
	{
		/*
		此处检查的意图是将收到的数据的前缀和后缀去掉，返回 有效信息
		若前缀后缀不完整，则认为此次请求数据失败，丢弃收到的数据，返回空字符串_T("")
		*/
		CString startMark, endMark;
		int len = CStringA(recvData).GetLength();
		startMark = recvData.Left(1);
		endMark = recvData.Right(1);
		validData = recvData.Mid(1, len - 2);

		if (startMark == _T("\2") && endMark == _T("\3"))
		{
			// do nothing
		}
		else
		{
			LOG(WARNING) << MessageProcess::FormatLogMessage(_T("传感器返回数据不完整"), m_id);
			validData == SENSOR_REQ_ERROR;
		}
	}
	CheckSensorData(validData, data);
}

void SensorSick::CheckSensorData(CString validData, double &validDData)
{
	if (validData == SENSOR_REQ_ERROR)
	{
		validDData = SENSOR_REQ_ERROR_DATA;
		return;
	}

	if (validData == _T("?") || validData == _T(">") || validData == _T("xxx"))
	{
		LOG(WARNING) << MessageProcess::FormatLogMessage(_T("传感器返回异常数据:") + validData, m_id);
		validDData = SENSOR_REQ_ERROR_DATA;
		return;
	}

	validDData = _tstof(LPCTSTR(validData));
}

void SensorSick::GetAndCheckSensorData(CString _data, int uintId, double &validData)
{
	CheckSensorData(_data, validData);
}