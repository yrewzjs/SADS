#include "stdafx.h"
#include "SensorKeyence.h"
#include "IOSocket.h"
#include "SyncCondition.h"
#include "MessageProcess.h"
#include "LaserDef.h"

SensorKeyence::SensorKeyence()
{
	m_laserState = true;
}

SensorKeyence::SensorKeyence(int _unitId)
{
	m_unitId = _unitId;
	if (m_unitId == 0)
		m_isVice = false;
	else
		m_isVice = true;
	m_laserState = true;
}

SensorKeyence::~SensorKeyence()
{
	if (m_isVice)
	{
		m_IoSocket = nullptr;
	}
	else
	{
		if (m_IoSocket != nullptr)
		{
			delete m_IoSocket;
			m_IoSocket = nullptr;
		}
	}
}

void SensorKeyence::SetUnitId(int _gid)
{
	m_unitId = _gid;
}

int SensorKeyence::GetUnitId()
{
	return m_unitId;
}

CString SensorKeyence::GetUnitIdStr()
{
	CString gid;
	gid.Format(_T("%02d"), m_unitId);//������λ��߲���
	return gid;
}

bool SensorKeyence::GetLaserState()
{
	return m_laserState;
}

bool SensorKeyence::GetIsVice()
{
	return m_isVice;
}

void SensorKeyence::SetIsVice(bool _share)
{
	m_isVice = _share;
}

bool SensorKeyence::TriggerLaser()
{
	const CString commend = _T("SW,") + GetUnitIdStr() + _T(",100,0\15\12");
	SetRequest(commend);
	//�ô�������������
	CString validData = RequestSensor();

	CString startStrOfCommend = commend.Left(2);
	CString startMark, endMark;
	startMark = validData.Left(2);
	endMark = validData.Right(2);
	if (startMark == startStrOfCommend)
	{
		m_laserState = true;
		return true;
	}
	else
	{
		return false;
	}
}

bool SensorKeyence::CloseLaser()
{
	const CString commend = _T("SW,") + GetUnitIdStr() + _T(",100,1\15\12");
	//�ô�������������
	SetRequest(commend);
	CString validData = RequestSensor();

	CString startStrOfCommend = commend.Left(2);
	CString startMark, endMark;
	startMark = validData.Left(2);
	endMark = validData.Right(2);
	if (startMark == startStrOfCommend)
	{
		m_laserState = false;
		return true;
	}
	else
	{
		return false;
	}
}

void SensorKeyence::AcquireGroupResponse(CString& validData)
{
	const CString commend = _T("M0\15\12");
	AcquireResponse(commend, validData);

	if (validData == SENSOR_REQ_ERROR)
	{
		validData = SENSOR_GROUP_REQ_ERROR;
	}
}

void SensorKeyence::GetAndCheckSensorData(CString validGroupData, int unitId, double &validDData)
{
	CString sData;
	if (validGroupData == SENSOR_GROUP_REQ_ERROR)
	{
		sData = SENSOR_REQ_ERROR;
	}
	else
	{
		/************   �����ݸ�ʽ  *********
		data��-117.06,-085.57,-999.98,-051.10,-038.18,-021.22,-037.50,+EEE.EE   
		sloc��   0       8       16      24      32      40      48      56   
		*********/
		const int unitLen = 7;  //ÿ��������������ռ����
		int dataLen = CStringA(validGroupData).GetLength();
		int startLoc = (unitLen + 1)*unitId;
		if (startLoc + unitLen > dataLen)
		{
			LOG(WARNING) << MessageProcess::FormatLogMessage(_T("�����������쳣,����unitId����"), m_id);
			sData = SENSOR_REQ_ERROR;
		}
		else
		{
			sData = validGroupData.Mid(startLoc, unitLen);
		}
	}

	CheckSensorData(sData, validDData);
}

void SensorKeyence::AcquireData(double &data)
{
	const CString commend = _T("SR,") + GetUnitIdStr() + _T(",037\15\12");
	CString sdata;
	AcquireResponse(commend, sdata);

	CString validData;
	int commaLoc = sdata.ReverseFind(',');
	validData = sdata.Mid(commaLoc + 1);

	CheckSensorData(validData, data);
}

void SensorKeyence::AcquireResponse(CString commend, CString& validData){
	//������������
	SetRequest(commend);
	//�ô�������������
	CString recvData = RequestSensor();
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
		startMark = recvData.Left(2);
		endMark = recvData.Right(2);
		validData = recvData.Mid(0, len - 2); //��ȥ��CR LF��
		CString startStrOfCommend = commend.Left(2);

		if (startMark == startStrOfCommend && endMark == _T("\15\12"))
		{
			//��ȡ"startMark��"��SR,��M0,  ֻ�������ݲ���
			int commaLoc = validData.Find(',');
			validData = validData.Mid(commaLoc + 1);  //������ǰλ��ֱ��ĩβ
		}
		else if (startMark == _T("ER"))
		{
			LOG(WARNING) << MessageProcess::FormatLogMessage(_T("�������������ݣ�ER"), m_id);
			validData = SENSOR_REQ_ERROR;
		}
		else
		{
			LOG(WARNING) << MessageProcess::FormatLogMessage(_T("�������������ݲ�����"), m_id);
			validData = SENSOR_REQ_ERROR;
		}
	}
}

void SensorKeyence::CheckSensorData(CString sdata, double &validDData)
{
	if (sdata == SENSOR_REQ_ERROR)
	{
		validDData = SENSOR_REQ_ERROR_DATA;
		return;
	}

	if (sdata == _T("+1000.00"))
	{
		LOG(WARNING) << MessageProcess::FormatLogMessage(_T("�������������ݣ�+1000.00"), m_id);
		validDData = SENSOR_REQ_ERROR_DATA;
		return;
	}
	else if (sdata == _T("+EEE.EE"))
	{
		LOG(WARNING) << MessageProcess::FormatLogMessage(_T("�������������ݣ�+EEE.EE�������Ƿ����Ӵ���ͷ"), m_id);
		validDData = SENSOR_REQ_ERROR_DATA;
		return;
	}

	CString mark, realdata;
	realdata = sdata.Right(sdata.GetLength() - 1); //realdataȥ���˷���λ
	validDData = _tstof(LPCTSTR(realdata));
	mark = sdata.Left(1);
	if (mark == _T("-"))
	{
		validDData = -validDData;
	}
}