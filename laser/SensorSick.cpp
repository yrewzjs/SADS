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
	//������������
	SetRequest(commend);
	//�ô�������������
	CString recvData = RequestSensor();
	CString validData = _T("");
	if (recvData == SENSOR_REQ_ERROR)
	{
		validData = SENSOR_REQ_ERROR;
	}
	else
	{
		/*
		�˴�������ͼ�ǽ��յ������ݵ�ǰ׺�ͺ�׺ȥ�������� ��Ч��Ϣ
		��ǰ׺��׺������������Ϊ�˴���������ʧ�ܣ������յ������ݣ����ؿ��ַ���_T("")
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
			LOG(WARNING) << MessageProcess::FormatLogMessage(_T("�������������ݲ�����"), m_id);
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
		LOG(WARNING) << MessageProcess::FormatLogMessage(_T("�����������쳣����:") + validData, m_id);
		validDData = SENSOR_REQ_ERROR_DATA;
		return;
	}

	validDData = _tstof(LPCTSTR(validData));
}

void SensorSick::GetAndCheckSensorData(CString _data, int uintId, double &validData)
{
	CheckSensorData(_data, validData);
}