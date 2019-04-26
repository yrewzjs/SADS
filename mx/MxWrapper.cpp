#include "stdafx.h"

#include <bitset>
using std::bitset;

#include "MxWrapper.h"
#include "CActUtlType.h"
#include "MXComonentDef.h"
#include "SadsDef.h"
#include "Synchronized.h"

#ifdef PLC_NOT_CONNECT
	#include<stdlib.h>
	#include<time.h>
	#define random(x) (rand()%x)
#endif

#define DEVICE_NAME_501     _T("D501")  // �·�����λ����״̬
#define DEVICE_NAME_503     _T("D503")  // д���͸׶���
#define DEVICE_NAME_504     _T("D504")  // д���͸׶�������


#define DEVICE_NAME_506     _T("D506")  // д���͸׶�������
#define DEVICE_NAME_507     _T("D507")  // д���͸׶�����ת�Ƕ�
#define DEVICE_NAME_510     _T("D510")  // ��ȡ��������͸׶���������

#define DEVICE_NAME_511     _T("D511")  // �·�����λ�쳣״̬
#define DEVICE_NAME_514     _T("D514")  // �·���λ��״̬

#define DEVICE_NAME_2025    _T("D2025") // ��ǰ��Ƭ

MxWrapper::MxWrapper()
{
	m_pActUtl = nullptr;
	m_pSynchronized = new Synchronized();
}


MxWrapper::~MxWrapper()
{
	if (m_pSynchronized != nullptr)
	{
		delete m_pSynchronized;
		m_pSynchronized = nullptr;
	}
}

//CActUtlType* MxWrapper::m_pActUtl = nullptr; //����
void MxWrapper::SetActUtlMx(CActUtlType* pAct)
{
	if (m_pActUtl == nullptr)
	{
		m_pActUtl = pAct; 
	}
}
/**
	func����ʼ��PLCͨ�ſؼ�
	@paramn��LogicNumber �߼�վ��
	@return��0����ɹ�����0Ϊʧ��
*/
long MxWrapper::initActUtl(long nLogicNumber)
{
#ifdef PLC_NOT_CONNECT
	long nCode = 0;
#else
	m_pActUtl->put_ActLogicalStationNumber(nLogicNumber);
	long nCode = m_pActUtl->Open();
#endif
	return nCode;
}

/*
	func������ʼ��PLCͨ�ſؼ�
	@return��0����ɹ�����0Ϊʧ��
*/
long MxWrapper::uninitActUtl()
{
#ifdef PLC_NOT_CONNECT
	long nCode = 0;
#else
	long nCode = m_pActUtl->Close();
#endif
	return nCode;
}

/*
	func����ȡPLC�ڴ���Ϣ
	@param��rplc �洢�ڴ���Ϣ�ṹ��
	@return��0����ɹ���ȡ����0Ϊ��ȡʧ��
*/
long MxWrapper::readPlc(rplc_struct *rplc)
{
	// ��������
#ifdef PLC_NOT_CONNECT

	srand((unsigned int)time(0));
	for (int i = 0; i < DEVICE_D5XX_NUMBER; i++)
	{
		rplc->lpsData_D5xx[i] = random(4);
	}
	for (int i = 0; i < DEVICE_D2XXX_NUMBER; i++)
	{
		rplc->lpsData_D2xxx[i] = random(4);
	}

	int nCode = 0;
#else
	// ��鴫��ṹ���Ƿ�Ϊ��
	if (rplc == nullptr)
	{
		return 1;
	}

	long err5 = m_pActUtl->ReadDeviceBlock2(DEVICE_NAME_500, DEVICE_D5XX_NUMBER, rplc->lpsData_D5xx);
	long err2 = m_pActUtl->ReadDeviceBlock2(DEVICE_NAME_2000, DEVICE_D2XXX_NUMBER, rplc->lpsData_D2xxx);
	long nCode = err2 + err5;
#endif
	return nCode;
}

/*
	func����PLC�ڴ�д��ָ��
	@param��wplc �洢д���ڴ�ָ��Ľṹ��
	@return��0����ɹ�д�룬��0Ϊд��ʧ��
*/
long MxWrapper::write2Plc(const wplc_struct *wplc)
{
	// д����
	m_pSynchronized->Lock();

#ifdef PLC_NOT_CONNECT
	int nCode = 0;
#else
	// ��鴫��ṹ���Ƿ�Ϊ��
	if (wplc == nullptr)
	{
		return 1;
	}

	// ��������Ƿ��� ʹ��λ
	if (wplc->hasEnabled)
	{
		// ����ʹ��λ��д��ʹ��λ����
		short enabled_lpsData = wplc->enabled_lpsData;
		long neCode = m_pActUtl->WriteDeviceBlock2(wplc->enabled_szDevice, wplc->enabled_lSize, &enabled_lpsData);

		// ��д��ʹ��λʧ�ܣ�ֱ�ӷ���
		if (neCode != 0)
		{
			return neCode;
		}
	}

	// д���������
	short lpsData = wplc->lpsData;
	long nCode = m_pActUtl->WriteDeviceBlock2(wplc->szDevice, wplc->lSize, &lpsData);
#endif
	// ����
	m_pSynchronized->UnLock();
	return nCode;
}

/*******************************************/
thrust_cylinder_struct MxWrapper::getThrustCylinderStruct(struct rplc_struct *rplc)
{
#ifdef PLC_NOT_CONNECT
	thrust_cylinder_struct tclds;
	srand((unsigned int)time(0));
	for (int i = 0; i < THRUST_CYLINDER_NUMBER; i++)
	{
		tclds.cylin_action[i] = random(4);
	}
	return tclds;
#else
	thrust_cylinder_struct tclds;
	//ʵ��
	//�ȴ�����PLC״̬��Ϣ����ȡ�ƽ��͸���Ϣ(D2019-D2024����6������)�����Ϊshortֵ
	const int CommdNumber = 6;    //�ƽ��͸�������6��
	const int CylinderNumer_0 = 16;
	const int CylinderNumer_1 = 3;
	const int FirstCommand = 19;  //�ƽ��͸���Ϣ�����D2019��ʼ
	short thrust_cylinder_Valve[CommdNumber];
	
	//�Ѷ�Ӧ���ƽ��͸����ݵ�����
	for (int i = 0; i < CommdNumber; i++)
	{
		thrust_cylinder_Valve[i] = rplc->lpsData_D2xxx[i + FirstCommand];
	}

	//����ȡ����shortֵ���а�λ��ȡ��������ȡ�����λ������͸���Ϣ����

	//��һ��ΪD2019��D2020���ȷ�����͸��Ƿ��ǡ��족��״̬
	bitset<16>Cylinder_number_0(thrust_cylinder_Valve[0]);
	bitset<16>Cylinder_number_1(thrust_cylinder_Valve[1]);
	//�ڶ���ΪD2021��D2022���ȷ�����͸��Ƿ��ǡ�������״̬
	bitset<16>Cylinder_number_2(thrust_cylinder_Valve[2]);
	bitset<16>Cylinder_number_3(thrust_cylinder_Valve[3]);
	//������ΪD2023��D2024���ȷ�����͸��Ƿ�ﵽ����λλ��
	bitset<16>Cylinder_number_4(thrust_cylinder_Valve[4]);
	bitset<16>Cylinder_number_5(thrust_cylinder_Valve[5]);

	//���ж�1-16���͸�״̬
	for (int i = 0; i < CylinderNumer_0; i++)
	{
		if (Cylinder_number_0[i])
		{
			tclds.cylin_action[i] = 1;//�͸�״̬Ϊ���족
		}
		else if (Cylinder_number_2[i])
		{
			tclds.cylin_action[i] = 2;//�͸�״̬Ϊ������
		}
		else if (Cylinder_number_4[i])
		{
			tclds.cylin_action[i] = 3;//�͸׵�������λ
		}
		else
		{
			tclds.cylin_action[i] = 0;//�͸�״̬Ϊ��ͣ��
		}
	}
	//�ж�17-19���͸�״̬
	for (int j = 0; j < CylinderNumer_1; j++)
	{
		int CylinderNumber = j + CylinderNumer_0;
		if (Cylinder_number_1[j])
		{
			tclds.cylin_action[CylinderNumber] = 1;//�͸�״̬Ϊ���족
		}
		else if (Cylinder_number_3[j])
		{
			tclds.cylin_action[CylinderNumber] = 2;//�͸�״̬Ϊ������
		}
		else if (Cylinder_number_5[j])
		{
			tclds.cylin_action[CylinderNumber] = 3;//�͸׵�������λ
		}
		else
		{
			tclds.cylin_action[CylinderNumber] = 0;//�͸�״̬Ϊ��ͣ��
		}
	}
	return tclds;
#endif
}

cylinder_struct MxWrapper::getCylinderStruct(struct rplc_struct *rplc)
{
#ifdef PLC_NOT_CONNECT
	cylinder_struct tclds;
	srand((unsigned int)time(0));
	for (int i = 0; i < CYLINDER_NUMBER; i++)
	{
		tclds.cylin_action[i] = random(2);
		tclds.cylin_loc[i] = random(100);
	}
	return tclds;
#else
	cylinder_struct clds;
	//ʵ��
	const int CommdNumber = 6;          //��ͨ�͸׶�����Ϣ�����λ����Ϣ����ֱ���6�����ֱ�ΪD2011-D2016��D2001-D2006
	const int FirstCommandAction = 11;  //��ͨ�͸׶�����Ϣ�����D2011��ʼ
	const int FirstCommandLocation = 1; //��ͨ�͸�λ����Ϣ�����D2001��ʼ
	for (int i = 0; i < CommdNumber; i++)
	{
		clds.cylin_action[i] = rplc->lpsData_D2xxx[i + FirstCommandAction];
		clds.cylin_loc[i] = rplc->lpsData_D2xxx[i + FirstCommandLocation];
	}
	return clds;
#endif
}

// ����ȡD500��־λ,ΪPLC�Ϸ�ץȡƴװ����
long MxWrapper::readBitD500(short &D500)
{
#ifdef PLC_NOT_CONNECT
	D500 = 0; // plc�Ϸ�
	return 0;
#else
	long rCode = m_pActUtl->ReadDeviceBlock2(DEVICE_NAME_500, 1, &D500);
	return rCode;
#endif
}

// ����ȡD2025��־λ,ΪPLC�Ϸ���ǰ��Ƭ
long MxWrapper::readBitD2025(short &D2025)
{
#ifdef PLC_NOT_CONNECT
	D2025 = 1;
	return 0;
#else
	long rCode = m_pActUtl->ReadDeviceBlock2(DEVICE_NAME_2025, 1, &D2025);
	return rCode;
#endif
}

// �·�����λ����״̬
long MxWrapper::writeBitD501(short D501)
{
	// д����
	m_pSynchronized->Lock();
#ifdef PLC_NOT_CONNECT
	long rCode = 0;
#else
	long rCode = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_501, 1, &D501);
#endif
	// ����
	m_pSynchronized->UnLock();
	return rCode;
}

// �·�����λδ���
long MxWrapper::writeBitD511(short D511)
{
	// д����
	m_pSynchronized->Lock();
#ifdef PLC_NOT_CONNECT
	long rCode = 0;
#else
	long rCode = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_511, 1, &D511);
	
#endif
	// ����
	m_pSynchronized->UnLock();
	return rCode;
}

// �·���λ��״̬
long MxWrapper::writeBitD514(short D514)
{
	// д����
	m_pSynchronized->Lock();
#ifdef PLC_NOT_CONNECT
	long rCode = 0;
#else
	long rCode = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_514, 1, &D514);
#endif
	// ����
	m_pSynchronized->UnLock();
	return rCode;
}

long MxWrapper::writeCylinderAction(const CylinderAD cld, const Direction dir)
{
	// д����
	m_pSynchronized->Lock();
#ifdef PLC_NOT_CONNECT
	return 0;
#else
	short D503 = 0;
	switch (cld)
	{
	case CLD_RED:
		D503 = 4;
		break;
	case CLD_BLUE:
		D503 = 5;
		break;
	case CLD_REDBLUE:
		D503 = 3;
		break;
	case CLD_SLIDE:
		D503 = 1;
		break;
	case CLD_ROTATE:
		D503 = 2;
		break;
	case CLD_PITCH:
		D503 = 6;
		break;
	case CLD_ROLL:
		D503 = 7;
		break;
	}

	short D504 = 0;
	switch (dir)
	{
	case KEEP:
		D504 = 0;
		break;
	case STRETCH:
		D504 = 1;
		break;
	case WITHDREW:
		D504 = 2;
		break;
	}



	if (D503 == 6)
	{
		short D506 = 2;
		short D507 = 50;
		short D510 = 0;

		//��Ҫ�Ƚ�D503дΪ��
		short D503_0 = 0;
		long code1 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_503, 1, &D503_0);
		Sleep(50);

		//��������
		long code4 = 1;
		if (cld == CLD_ROTATE)
		{
			code4 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_507, 1, &D507);
		}
		else
		{
			code4 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_506, 1, &D506);
		}
		// �ĸ��͸�
		long code2 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_503, 1, &D503);
		// �ĸ�����
		long code3 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_504, 1, &D504);

		Sleep(500);
		//��ȡ�����ɹ����
		long rD510 = 1;
		long rCode = 0;
		for (int i = 0; i < 95000; i += 100)
		{
			long rCode = m_pActUtl->ReadDeviceBlock2(DEVICE_NAME_510, 1, &D510);
			//�����Ƿ����
			if (D510 == 1)
			{
				rD510 = 0;
				break;
			}
		}
		// ����
		m_pSynchronized->UnLock();
		return code1 + code2 + code3 + code4 + rCode + rD510;
	}
	else
	{
		//һ����Ҫ�Ƚ�D503дΪ��
		short D503_0 = 0;
		long code0 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_503, 1, &D503_0);
		//������Ҫ�Ƚ�D506����D507дΪ��
		long code1 = 0;
		if (cld == CLD_ROTATE)
		{
			short D507 = 0;
			code1 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_507, 1, &D507);
		}
		else
		{
			short D506 = 0;
			code1 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_506, 1, &D506);
		}
		//���� �ĸ��͸�
		long code2 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_503, 1, &D503);
		//�ġ� �ĸ�����
		long code3 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_504, 1, &D504);
		// ����
		m_pSynchronized->UnLock();
		return code0 + code1 + code2 + code3;
	}

	
#endif
}

long MxWrapper::writeCylinderAction2(CylinderAD cld, Direction dir)
{
	// д����
	m_pSynchronized->Lock();
#ifdef PLC_NOT_CONNECT
	m_pSynchronized->UnLock();
	return 0;
#else
	short D503 = 0;
	switch (cld)
	{
	case CLD_RED:
		D503 = 4;
		break;
	case CLD_BLUE:
		D503 = 5;
		break;
	case CLD_REDBLUE:
		D503 = 3;
		break;
	case CLD_SLIDE:
		D503 = 1;
		break;
	case CLD_ROTATE:
		D503 = 2;
		break;
	case CLD_PITCH:
		D503 = 6;
		break;
	case CLD_ROLL:
		D503 = 7;
		break;
	}

	short D504 = 0;
	switch (dir)
	{
	case KEEP:
		D504 = 0;
		break;
	case STRETCH:
		D504 = 1;
		break;
	case WITHDREW:
		D504 = 2;
		break;
	}

	short D506 = 2;
	short D507 = 50;
	short D510 = 0;

	//��Ҫ�Ƚ�D503дΪ��
	short D503_0 = 0;
	long code1 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_503, 1, &D503_0);
	Sleep(50);

	//��������
	long code4 = 1;
	if (cld == CLD_ROTATE)
	{
		code4 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_507, 1, &D507);
	}
	else
	{
		code4 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_506, 1, &D506);
	}
	// �ĸ��͸�
	long code2 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_503, 1, &D503);
	// �ĸ�����
	long code3 = m_pActUtl->WriteDeviceBlock2(DEVICE_NAME_504, 1, &D504);

	Sleep(500);
	//��ȡ�����ɹ����
	long rD510 = 1;
	long rCode = 0;
	for (int i = 0; i < 95000; i += 100)
	{
		long rCode = m_pActUtl->ReadDeviceBlock2(DEVICE_NAME_510, 1, &D510);
		//�����Ƿ����
		if (D510 == 1)
		{
			rD510 = 0;
			break;
		}
	}


	// ����
	m_pSynchronized->UnLock();
	return code1 + code2 + code3 + code4 + rCode + rD510;
#endif
}


