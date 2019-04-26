#include "stdafx.h"
#include <math.h>

#include "CalculateRanging.h"
#include "LaserResponse.h"
#include "LaserRequest.h"
#include "LaserDef.h"
#include "SadsDef.h"
#include "MessageProcess.h"

CalculateRanging::CalculateRanging() : Calculate()
{

}

CalculateRanging::~CalculateRanging()
{

}

void CalculateRanging::CalculateProc(LaserRequest* const request, LaserResponse* const response)
{
	const AdjustStep step = request->GetAdjustStep();
	// ���������Ҫ�Ĵ���������
	if (CheckNeedSensorData(step) != true)
	{
		response->SetHasException(true);
		return; //��������ִ��
	}
	//����������Ҫ��delta����
	UpdataDeltaArr(request->GetSegmentAD());

	switch (step)
	{
	case GC_STEP_ZERO:
		this->AdjustGeneCatchZero(request, response);
		break;
	case GC_STEP_FIR:
		this->AdjustGeneCatchFir(request, response);
		break;
	case GC_STEP_SEC:
		this->AdjustGeneCatchSec(request, response);
		break;
	case G_STEP_FIR:
		this->AdjustGeneSpliceFir(request, response);
		break;
	case G_STEP_SEC:
		this->AdjustGeneSpliceSec(request, response);
		break;
	case RC_STEP_ZERO:
		this->AdjustRoofCatchZero(request, response);
		break;
	case RC_STEP_FIR:
		this->AdjustRoofCatchFir(request, response);
		break;
	case RC_STEP_SEC:
		this->AdjustRoofCatchSec(request, response);
		break;
	case R_STEP_FIR:
		this->AdjustRoofSpliceFir(request, response);
		break;
	case R_STEP_SEC:
		this->AdjustRoofSpliceSec(request, response);
		break;
	}
}

/*
*  ��Ƭץȡ ���㲽   Confirmed
*/
void CalculateRanging::AdjustGeneCatchZero(LaserRequest* const request, LaserResponse* const response)
{
	const double avg_ACDE_biao = (GetCaliGcHeight(A0) + GetCaliGcHeight(C0) + GetCaliGcHeight(D0) + GetCaliGcHeight(E0)) / 4;
	double h_A0 = GetGeneRealValue(A0);  //116.71
	double h_B0 = GetGeneRealValue(B0);  //127.55
	double h_C0 = GetGeneRealValue(C0);  //123.18
	double h_D0 = GetGeneRealValue(D0);  //117.09
	double h_E0 = GetGeneRealValue(E0);  //111.59
	double avg_ACDE = (h_A0 + h_C0 + h_D0 + h_E0) / 4;
	const double t_de_avg_ACDE = 30;
	if (avg_ACDE - avg_ACDE_biao > t_de_avg_ACDE)
	{
		response->SetHasException(true);
		const CString exceptionMsg = _T("��Ƭ��̬ƫ������������Ƭ");
		LOG(ERROR) << MessageProcess::ProcessMessage(exceptionMsg);
	}
	return;
}

/*
*  ��Ƭץȡ ��һ��   Confirmed
*/
void CalculateRanging::AdjustGeneCatchFir(LaserRequest* const request, LaserResponse* const response)
{
	//���ݼ���
	double max_DE_CA = MaxAbsDelta(D0_E0, C0_A0); //�졢���͸׵���������ƫҡ
	double max_DC_EA = MaxAbsDelta(D0_C0, E0_A0); //����

#ifdef SADS_LOCAL_TEST
	CString delta_s;
	delta_s.Format(_T("r/b_max_DE_CA = %.2lf  pitch_max_DC_EA = %.2lf"), max_DE_CA, max_DC_EA);
#endif

	/***************************************************************
	1��������
	2����ƫҡ����ƽ��
	3�����߶�
	***/
	const double t_DC_EA = 10; //���Գ���10
	if (abs(max_DC_EA) < t_DC_EA)
	{
		const double t_DE_CA = 12; //���Գ���10
		if (abs(max_DE_CA) <= t_DE_CA)
		{
			//const double avg_BCD_biao = (GetCaliGcHeight(B0) + GetCaliGcHeight(C0) + GetCaliGcHeight(D0)) / 3;
			//const double avg_ABE_biao = (GetCaliGcHeight(A0) + GetCaliGcHeight(B0) + GetCaliGcHeight(E0)) / 3;
			const double avg_ACDE_biao = (GetCaliGcHeight(A0) + GetCaliGcHeight(C0) + GetCaliGcHeight(D0) + GetCaliGcHeight(E0)) / 4;
			//	const double avg_BCD_biao = 122.61; //122.61
			//	const double avg_ABE_biao = 118.62; //118.62

			double h_A0 = GetGeneRealValue(A0);  //116.71
			double h_B0 = GetGeneRealValue(B0);  //127.55
			double h_C0 = GetGeneRealValue(C0);  //123.18
			double h_D0 = GetGeneRealValue(D0);  //117.09
			double h_E0 = GetGeneRealValue(E0);  //111.59

			//double avg_BCD = (h_B0 + h_C0 + h_D0) / 3;
			//double avg_ABE = (h_A0 + h_B0 + h_E0) / 3;
			double avg_ACDE = (h_A0 + h_C0 + h_D0 + h_E0) / 4;
			double t_de_avg_ACDE = 100;
			if (abs(avg_ACDE - avg_ACDE_biao )<= 100)
			{
				//��ɵ���
				response->SetIsFinished(true);
				response->UpdateCylinder(CLD_REDBLUE, KEEP);
			}
			else if (avg_ACDE - avg_ACDE_biao > 100)
			{
				response->UpdateCylinder(CLD_REDBLUE, STRETCH);
			}
			else if (avg_ACDE - avg_ACDE_biao < -100)
			{
				response->UpdateCylinder(CLD_REDBLUE, WITHDREW);
			}
			//double de_BCD = avg_BCD - avg_BCD_biao;  // ����0 ��Ԥ��λ�ø�  
			//double de_ABE = avg_ABE - avg_ABE_biao;

			//double sign_abs_max = 0;
			//if (abs(de_BCD) >= abs(de_ABE))
			//{
			//	sign_abs_max = de_BCD;
			//}
			//else
			//{
			//	sign_abs_max = de_ABE;
			//}

#ifdef SADS_LOCAL_TEST
			//CString deltaH;
			//deltaH.Format(_T("de_ABE = %.2lf  de_BCD = %.2lf sign_abs_max = %.2lf "), de_ABE, de_BCD, sign_abs_max);
			//MessageProcess::ProcessException(NORMAL, deltaH, 0);
#endif
			//const double t_max = 5;
			//if (abs(sign_abs_max) <= t_max)  //  ���ڷ�Χ��
			//{
			//	//��ɵ���
			//	//response->SetIsFinished(GC_STEP_FIR_Y);
			//	//response->UpdateCylinder(KEEP, KEEP, KEEP);
			//}
			//else if (sign_abs_max < -t_max) //���ˣ����ߵ���
			//{
			//	//����ռ����
			//	if (abs(de_ABE) > abs(de_BCD))  // �����
			//	{
			//		response->UpdateCylinder(CLD_RED, WITHDREW);
			//	}
			//	else
			//	{
			//		response->UpdateCylinder(CLD_BLUE, WITHDREW);
			//	}
			//}
			//else if (sign_abs_max > t_max) //���ˣ����͵���
			//{
			//	//����ռ����
			//	if (abs(de_ABE) > abs(de_BCD))  // �����
			//	{
			//		response->UpdateCylinder(CLD_RED, STRETCH);
			//	}
			//	else
			//	{
			//		response->UpdateCylinder(CLD_BLUE, STRETCH);
			//	}
			//}
		}
		else if (max_DE_CA > t_DE_CA)
		{
			response->UpdateCylinder(CLD_BLUE, STRETCH);
		}
		else if (max_DE_CA < -t_DE_CA)
		{
			response->UpdateCylinder(CLD_RED, STRETCH);
		}
	}
	else if (max_DC_EA > t_DC_EA)
	{
		response->UpdateCylinder(CLD_PITCH, STRETCH);
	}
	else if (max_DC_EA < -t_DC_EA)
	{
		response->UpdateCylinder(CLD_PITCH, WITHDREW);
	}
	return; //��������
}

/*
*  ��Ƭץȡ �ڶ���   Confirmed
*/
void CalculateRanging::AdjustGeneCatchSec(LaserRequest* const request, LaserResponse* const response)
{
	//��ʼ��������Ϊδ���
	double h_A0 = GetGeneRealValue(A0);
	double h_C0 = GetGeneRealValue(C0);
	double h_D0 = GetGeneRealValue(D0);
	double h_E0 = GetGeneRealValue(E0);
	//double min_ACDE_0;
	double min_ACDE_0 = MinValue(h_D0, h_E0, h_A0, h_C0);

	double max_DC_EA = MaxAbsDelta(D0_C0, E0_A0);
	double max_DE_CA = MaxAbsDelta(D0_E0, C0_A0);

#ifdef SADS_LOCAL_TEST
	CString delta_s;
	delta_s.Format(_T("min_ACDE_0 = %.2lf  r/b_max_DE_CA = %.2lf  pitch_max_DC_EA = %.2lf"), min_ACDE_0, max_DE_CA, max_DC_EA);
	//MessageProcess::ProcessException(NORMAL, delta_s, 0);
#endif


	const double t_DE_CA = 15;
	if (abs(max_DE_CA) <= t_DE_CA)
	{
		const double t_DC_EA = 20; //�Ѿ���֤
		if (abs(max_DC_EA) <= t_DC_EA)
		{
			//m_pMxWrapper2->writeCylinderAction(CLD_PITCH, KEEP);
			const int t_min = 10;// <20mm,
			if (min_ACDE_0 <= t_min)
			{
				const int t_DE_CA_2 = -3;
				if (max_DE_CA > t_DE_CA_2)
				{
					response->UpdateCylinder(CLD_BLUE, STRETCH);
				}
				else
				{
					//������ɣ���Ϊ��ǰ�������
					response->SetIsFinished(true);
					response->UpdateCylinder(CLD_REDBLUE, KEEP);
				}
			}
			else
			{
				response->UpdateCylinder(CLD_REDBLUE, STRETCH);
			}
		}
			if (max_DC_EA > t_DC_EA)
			{
				response->UpdateCylinder(CLD_PITCH, STRETCH);
			}
			else if (max_DC_EA < -t_DC_EA)
			{
				response->UpdateCylinder(CLD_PITCH, WITHDREW);
			}
		
	}
		if (max_DE_CA > t_DE_CA)
		{
			response->UpdateCylinder(CLD_BLUE, STRETCH);
		}
		else if (max_DE_CA < -t_DE_CA)
		{
			response->UpdateCylinder(CLD_RED, STRETCH);
		}


		return;//����
	
}

/*
*  ��һ��,����������------�߶Ⱥͺ�ҡ
*/
void CalculateRanging::AdjustGeneSpliceFir(LaserRequest* const request, LaserResponse* const response)
{
	double minDe_ABC2_C0 = MinDelta(A2_A0, B2_B0, C2_C0);
	double diff_AC2_C0 = GetDiffOfDelta(A2_A0, C2_C0);

#ifdef SADS_LOCAL_TEST
	CString delta_s;
	delta_s.Format(_T("minDe_ABC2_C0 = %.2lf diff_AC2_C0 = %.2lf"), minDe_ABC2_C0, diff_AC2_C0);
	//MessageProcess::ProcessException(NORMAL, delta_s, 0);
#endif

	if (minDe_ABC2_C0 > 0)
	{
		int thrshold = 3;
		if (minDe_ABC2_C0 < thrshold)
		{
			if (abs(diff_AC2_C0) <= thrshold)
			{
				//˵����һ���������,�����͸�ֹͣ�ƽ�����ʼ����������й�Ƭ��ƽ�е����ͱ�Ե����
				//response->SetIsFinished(G_STEP_FIR_Y);
				//response->UpdateCylinder(KEEP, KEEP, KEEP);
			}
			else if (diff_AC2_C0 > thrshold)
			{
				response->UpdateCylinder(CLD_RED, STRETCH);
			}
			else if (diff_AC2_C0 < -thrshold)
			{
				response->UpdateCylinder(CLD_BLUE, STRETCH);
			}
		}
		else
		{
			response->UpdateCylinder(CLD_REDBLUE, STRETCH);
		}
	}
	else
	{
		response->UpdateCylinder(CLD_REDBLUE, WITHDREW);
	}
	return;//����
}

/*
* �ڶ������ӵ�����
*/
void CalculateRanging::AdjustGeneSpliceSec(LaserRequest* const request, LaserResponse* const response)
{
	//��ʼ��������Ϊδ���
	double minDe_ABC2_C0 = MinDelta(A2_A0, B2_B0, C2_C0);
	double diff_AC2_C0 = GetDiffOfDelta(A2_A0, C2_C0);
	double minDe_ABC2_C1 = MinDelta(A2_A1, B2_B1, C2_C1);
#ifdef SADS_LOCAL_TEST
	CString delta_s;
	delta_s.Format(_T("minDe_ABC2_C0 = %.2lf diff_AC2_C0 = %.2lf minDe_ABC2_C1 = %.2lf"),
		minDe_ABC2_C0, diff_AC2_C0, minDe_ABC2_C1);
	//MessageProcess::ProcessException(NORMAL, delta_s, 0);
#endif
	if (minDe_ABC2_C0 > 0)
	{
		if (minDe_ABC2_C0 < 3)
		{
			int thrshold = 3;
			if (abs(diff_AC2_C0) <= thrshold)
			{
				//˵����һ���������,�����͸�ֹͣ�ƽ�����ʼ����������й�Ƭ��ƽ�е����ͱ�Ե����
				int thrshold_2 = 1;
				if (abs(minDe_ABC2_C1) <= thrshold_2)
				{
					//˵���ڶ���������ɣ������͸�ֹͣ�ƽ�����ʼ����������й�Ƭ��ƽ�е����ͱ�Ե����
					//response->SetIsFinished(G_STEP_SEC_Y);
					//response->UpdateCylinder(KEEP, KEEP, KEEP);
				}
				else if (minDe_ABC2_C1 > thrshold_2)
				{
					response->UpdateCylinder(CLD_PITCH, WITHDREW);
				}
				else
				{
					response->UpdateCylinder(CLD_PITCH, STRETCH);
				}
				return;//����
			}
			else if (diff_AC2_C0 > thrshold)
			{
				response->UpdateCylinder(CLD_RED, STRETCH);
			}
			else if (diff_AC2_C0 < -thrshold)
			{
				response->UpdateCylinder(CLD_BLUE, STRETCH);
			}
		}
		else
		{
			response->UpdateCylinder(CLD_REDBLUE, STRETCH);
		}
	}
	else
	{
		response->UpdateCylinder(CLD_REDBLUE, WITHDREW);
	}
}

/*
*  �ⶥץȡ ���㲽   Confirmed
*/
void CalculateRanging::AdjustRoofCatchZero(LaserRequest* const request, LaserResponse* const response)
{
	const double avg_FGHJ_biao = (GetCaliGcHeight(F0) + GetCaliGcHeight(G0) + GetCaliGcHeight(H0) + GetCaliGcHeight(J0)) / 4;
	double h_F0 = GetRoofRealValue(F0);  //116.71
	double h_G0 = GetRoofRealValue(G0);  //123.18
	double h_H0 = GetRoofRealValue(H0);  //117.09
	double h_J0 = GetRoofRealValue(J0);  //111.59
	double avg_FGHJ = (h_F0 + h_G0 + h_H0 + h_J0) / 4;
	const double t_de_avg_FGHJ = 30;
	if (abs(avg_FGHJ - avg_FGHJ_biao) > t_de_avg_FGHJ)
	{
		response->SetHasException(true);
		const CString exceptionMsg = _T("��Ƭ��̬ƫ������������Ƭ");
		LOG(ERROR) << MessageProcess::ProcessMessage(exceptionMsg);
	}
	return;
}

// �ⶥץȡ ��һ��
void CalculateRanging::AdjustRoofCatchFir(LaserRequest* const request, LaserResponse* const response)
{
	//���ݼ���
	double max_FG_HJ = MaxAbsDelta(F0_G0, H0_J0); //�졢���͸׵���������ƫҡ
	double max_FH_GJ = MaxAbsDelta(F0_H0, G0_J0); //����

#ifdef SADS_LOCAL_TEST
	CString delta_s;
	delta_s.Format(_T("r/b_max_DE_CA = %.2lf  pitch_max_DC_EA = %.2lf"), max_FG_HJ, max_FH_GJ);
#endif

	/***************************************************************
	1��������
	2����ƫҡ����ƽ��
	3�����߶�
	***/
	const double t_FH_GJ = 10; //���Գ���10
	if (abs(max_FH_GJ) < t_FH_GJ)
	{
		const double t_FG_HJ = 12; //���Գ���10
		if (abs(max_FG_HJ) <= t_FG_HJ)
		{
			//const double avg_BCD_biao = (GetCaliGcHeight(B0) + GetCaliGcHeight(C0) + GetCaliGcHeight(D0)) / 3;
			//const double avg_ABE_biao = (GetCaliGcHeight(A0) + GetCaliGcHeight(B0) + GetCaliGcHeight(E0)) / 3;
			const double avg_FGHJ_biao = (GetCaliRcHeight(F0) + GetCaliRcHeight(G0) + GetCaliRcHeight(H0) + GetCaliRcHeight(J0)) / 4;
			//	const double avg_BCD_biao = 122.61; //122.61
			//	const double avg_ABE_biao = 118.62; //118.62

			double h_F0 = GetRoofRealValue(F0);  //116.71
			//double h_B0 = GetGeneRealValue(B0);  //127.55
			double h_G0 = GetRoofRealValue(G0);  //123.18
			double h_H0 = GetRoofRealValue(H0);  //117.09
			double h_J0 = GetRoofRealValue(J0);  //111.59

			//double avg_BCD = (h_B0 + h_C0 + h_D0) / 3;
			//double avg_ABE = (h_A0 + h_B0 + h_E0) / 3;
			double avg_FGHJ = (h_F0 + h_G0 + h_H0 + h_J0) / 4;
			double t_de_avg_FGHJ = 10;
			if (abs(avg_FGHJ - avg_FGHJ_biao) <= 10)
			{
				//��ɵ���
				response->SetIsFinished(true);
				response->UpdateCylinder(CLD_REDBLUE, KEEP);
			}
			else if (avg_FGHJ - avg_FGHJ_biao > 10)
			{
				response->UpdateCylinder(CLD_REDBLUE, STRETCH);
			}
			else if (avg_FGHJ - avg_FGHJ_biao < -10)
			{
				response->UpdateCylinder(CLD_REDBLUE, WITHDREW);
			}
			//double de_BCD = avg_BCD - avg_BCD_biao;  // ����0 ��Ԥ��λ�ø�  
			//double de_ABE = avg_ABE - avg_ABE_biao;

			//double sign_abs_max = 0;
			//if (abs(de_BCD) >= abs(de_ABE))
			//{
			//	sign_abs_max = de_BCD;
			//}
			//else
			//{
			//	sign_abs_max = de_ABE;
			//}

#ifdef SADS_LOCAL_TEST
			//CString deltaH;
			//deltaH.Format(_T("de_ABE = %.2lf  de_BCD = %.2lf sign_abs_max = %.2lf "), de_ABE, de_BCD, sign_abs_max);
			//MessageProcess::ProcessException(NORMAL, deltaH, 0);
#endif
			//const double t_max = 5;
			//if (abs(sign_abs_max) <= t_max)  //  ���ڷ�Χ��
			//{
			//	//��ɵ���
			//	//response->SetIsFinished(GC_STEP_FIR_Y);
			//	//response->UpdateCylinder(KEEP, KEEP, KEEP);
			//}
			//else if (sign_abs_max < -t_max) //���ˣ����ߵ���
			//{
			//	//����ռ����
			//	if (abs(de_ABE) > abs(de_BCD))  // �����
			//	{
			//		response->UpdateCylinder(CLD_RED, WITHDREW);
			//	}
			//	else
			//	{
			//		response->UpdateCylinder(CLD_BLUE, WITHDREW);
			//	}
			//}
			//else if (sign_abs_max > t_max) //���ˣ����͵���
			//{
			//	//����ռ����
			//	if (abs(de_ABE) > abs(de_BCD))  // �����
			//	{
			//		response->UpdateCylinder(CLD_RED, STRETCH);
			//	}
			//	else
			//	{
			//		response->UpdateCylinder(CLD_BLUE, STRETCH);
			//	}
			//}
		}
		else if (max_FG_HJ > t_FG_HJ)
		{
			response->UpdateCylinder(CLD_BLUE, STRETCH);
		}
		else if (max_FG_HJ < -t_FG_HJ)
		{
			response->UpdateCylinder(CLD_RED, STRETCH);
		}
	}
	else if (max_FH_GJ > t_FH_GJ)
	{
		response->UpdateCylinder(CLD_PITCH, STRETCH);
	}
	else if (max_FH_GJ < -t_FH_GJ)
	{
		response->UpdateCylinder(CLD_PITCH, WITHDREW);
	}
	return; //��������
}
// �ⶥץȡ �ڶ���
void CalculateRanging::AdjustRoofCatchSec(LaserRequest* const request, LaserResponse* const response)
{
	//��ʼ��������Ϊδ���
	double h_F0 = GetRoofRealValue(F0);
	double h_G0 = GetRoofRealValue(G0);
	double h_H0 = GetRoofRealValue(H0);
	double h_J0 = GetRoofRealValue(J0);
	
	double min_FGHJ_0 = MinValue(h_F0, h_G0, h_H0, h_J0);

	double max_FH_GJ = MaxAbsDelta(F0_H0, G0_J0); //����
	double max_FG_HJ = MaxAbsDelta(F0_G0, H0_J0); //����

#ifdef SADS_LOCAL_TEST
	CString delta_s;
	delta_s.Format(_T("min_ACDE_0 = %.2lf  r/b_max_DE_CA = %.2lf  pitch_max_DC_EA = %.2lf"), min_FGHJ_0, max_FH_GJ, max_FG_HJ);
	//MessageProcess::ProcessException(NORMAL, delta_s, 0);
#endif


	const double t_FG_HJ = 5;
	if (abs(max_FG_HJ) <= t_FG_HJ)
	{
		const double t_FH_GJ = 5; //�Ѿ���֤
		if (abs(max_FH_GJ) <= t_FH_GJ)
		{
			//m_pMxWrapper2->writeCylinderAction(CLD_PITCH, KEEP);
			const int t_min = 0;// <20mm,
			if (min_FGHJ_0 <= t_min)
			{
				/*const int t_FG_HJ_2 = 0;
				if (max_FG_HJ > t_FG_HJ_2)
				{
					response->UpdateCylinder(CLD_BLUE, STRETCH);
				}

				else
				{*/
					//������ɣ���Ϊ��ǰ�������
					response->SetIsFinished(true);
					response->UpdateCylinder(CLD_REDBLUE, KEEP);
				//}
			}
			else
			{
				response->UpdateCylinder(CLD_REDBLUE, STRETCH);
			}
		}
		if (max_FH_GJ > t_FH_GJ)
		{
			response->UpdateCylinder(CLD_PITCH, STRETCH);
		}
		else if (max_FH_GJ < -t_FH_GJ)
		{
			response->UpdateCylinder(CLD_PITCH, WITHDREW);
		}

	}
	if (max_FG_HJ > t_FG_HJ)
	{
		response->UpdateCylinder(CLD_BLUE, STRETCH);
	}
	else if (max_FG_HJ < -t_FG_HJ)
	{
		response->UpdateCylinder(CLD_RED, STRETCH);
	}


	return;//����

}
// �ⶥƴװ ��һ��
void CalculateRanging::AdjustRoofSpliceFir(LaserRequest* const request, LaserResponse* const response)
{

}
// �ⶥƴװ �ڶ���
void CalculateRanging::AdjustRoofSpliceSec(LaserRequest* const request, LaserResponse* const response)
{

}