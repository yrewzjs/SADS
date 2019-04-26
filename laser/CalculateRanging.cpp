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
	// 检查运算需要的传感器数据
	if (CheckNeedSensorData(step) != true)
	{
		response->SetHasException(true);
		return; //不再向下执行
	}
	//更新运算需要的delta数据
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
*  管片抓取 第零步   Confirmed
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
		const CString exceptionMsg = _T("管片姿态偏差过大，请调整管片");
		LOG(ERROR) << MessageProcess::ProcessMessage(exceptionMsg);
	}
	return;
}

/*
*  管片抓取 第一步   Confirmed
*/
void CalculateRanging::AdjustGeneCatchFir(LaserRequest* const request, LaserResponse* const response)
{
	//数据计算
	double max_DE_CA = MaxAbsDelta(D0_E0, C0_A0); //红、蓝油缸单独动，即偏摇
	double max_DC_EA = MaxAbsDelta(D0_C0, E0_A0); //俯仰

#ifdef SADS_LOCAL_TEST
	CString delta_s;
	delta_s.Format(_T("r/b_max_DE_CA = %.2lf  pitch_max_DC_EA = %.2lf"), max_DE_CA, max_DC_EA);
#endif

	/***************************************************************
	1、调俯仰
	2、调偏摇，即平行
	3、调高度
	***/
	const double t_DC_EA = 10; //可以尝试10
	if (abs(max_DC_EA) < t_DC_EA)
	{
		const double t_DE_CA = 12; //可以尝试10
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
				//完成调整
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
			//double de_BCD = avg_BCD - avg_BCD_biao;  // 大于0 比预定位置高  
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
			//if (abs(sign_abs_max) <= t_max)  //  即在范围内
			//{
			//	//完成调整
			//	//response->SetIsFinished(GC_STEP_FIR_Y);
			//	//response->UpdateCylinder(KEEP, KEEP, KEEP);
			//}
			//else if (sign_abs_max < -t_max) //低了，往高调整
			//{
			//	//负的占主导
			//	if (abs(de_ABE) > abs(de_BCD))  // 红缸缩
			//	{
			//		response->UpdateCylinder(CLD_RED, WITHDREW);
			//	}
			//	else
			//	{
			//		response->UpdateCylinder(CLD_BLUE, WITHDREW);
			//	}
			//}
			//else if (sign_abs_max > t_max) //高了，往低调整
			//{
			//	//正的占主导
			//	if (abs(de_ABE) > abs(de_BCD))  // 红缸伸
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
	return; //函数返回
}

/*
*  管片抓取 第二步   Confirmed
*/
void CalculateRanging::AdjustGeneCatchSec(LaserRequest* const request, LaserResponse* const response)
{
	//开始调整，置为未完成
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
		const double t_DC_EA = 20; //已经验证
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
					//调整完成，置为当前步骤完成
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


		return;//返回
	
}

/*
*  第一步,调整红蓝缸------高度和横摇
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
				//说明第一步调整完成,红蓝油缸停止推进，开始利用相机进行管片的平行调整和边缘调整
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
	return;//返回
}

/*
* 第二步，加调俯仰
*/
void CalculateRanging::AdjustGeneSpliceSec(LaserRequest* const request, LaserResponse* const response)
{
	//开始调整，置为未完成
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
				//说明第一步调整完成,红蓝油缸停止推进，开始利用相机进行管片的平行调整和边缘调整
				int thrshold_2 = 1;
				if (abs(minDe_ABC2_C1) <= thrshold_2)
				{
					//说明第二步调整完成，俯仰油缸停止推进，开始利用相机进行管片的平行调整和边缘调整
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
				return;//返回
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
*  封顶抓取 第零步   Confirmed
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
		const CString exceptionMsg = _T("管片姿态偏差过大，请调整管片");
		LOG(ERROR) << MessageProcess::ProcessMessage(exceptionMsg);
	}
	return;
}

// 封顶抓取 第一步
void CalculateRanging::AdjustRoofCatchFir(LaserRequest* const request, LaserResponse* const response)
{
	//数据计算
	double max_FG_HJ = MaxAbsDelta(F0_G0, H0_J0); //红、蓝油缸单独动，即偏摇
	double max_FH_GJ = MaxAbsDelta(F0_H0, G0_J0); //俯仰

#ifdef SADS_LOCAL_TEST
	CString delta_s;
	delta_s.Format(_T("r/b_max_DE_CA = %.2lf  pitch_max_DC_EA = %.2lf"), max_FG_HJ, max_FH_GJ);
#endif

	/***************************************************************
	1、调俯仰
	2、调偏摇，即平行
	3、调高度
	***/
	const double t_FH_GJ = 10; //可以尝试10
	if (abs(max_FH_GJ) < t_FH_GJ)
	{
		const double t_FG_HJ = 12; //可以尝试10
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
				//完成调整
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
			//double de_BCD = avg_BCD - avg_BCD_biao;  // 大于0 比预定位置高  
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
			//if (abs(sign_abs_max) <= t_max)  //  即在范围内
			//{
			//	//完成调整
			//	//response->SetIsFinished(GC_STEP_FIR_Y);
			//	//response->UpdateCylinder(KEEP, KEEP, KEEP);
			//}
			//else if (sign_abs_max < -t_max) //低了，往高调整
			//{
			//	//负的占主导
			//	if (abs(de_ABE) > abs(de_BCD))  // 红缸缩
			//	{
			//		response->UpdateCylinder(CLD_RED, WITHDREW);
			//	}
			//	else
			//	{
			//		response->UpdateCylinder(CLD_BLUE, WITHDREW);
			//	}
			//}
			//else if (sign_abs_max > t_max) //高了，往低调整
			//{
			//	//正的占主导
			//	if (abs(de_ABE) > abs(de_BCD))  // 红缸伸
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
	return; //函数返回
}
// 封顶抓取 第二步
void CalculateRanging::AdjustRoofCatchSec(LaserRequest* const request, LaserResponse* const response)
{
	//开始调整，置为未完成
	double h_F0 = GetRoofRealValue(F0);
	double h_G0 = GetRoofRealValue(G0);
	double h_H0 = GetRoofRealValue(H0);
	double h_J0 = GetRoofRealValue(J0);
	
	double min_FGHJ_0 = MinValue(h_F0, h_G0, h_H0, h_J0);

	double max_FH_GJ = MaxAbsDelta(F0_H0, G0_J0); //俯仰
	double max_FG_HJ = MaxAbsDelta(F0_G0, H0_J0); //红蓝

#ifdef SADS_LOCAL_TEST
	CString delta_s;
	delta_s.Format(_T("min_ACDE_0 = %.2lf  r/b_max_DE_CA = %.2lf  pitch_max_DC_EA = %.2lf"), min_FGHJ_0, max_FH_GJ, max_FG_HJ);
	//MessageProcess::ProcessException(NORMAL, delta_s, 0);
#endif


	const double t_FG_HJ = 5;
	if (abs(max_FG_HJ) <= t_FG_HJ)
	{
		const double t_FH_GJ = 5; //已经验证
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
					//调整完成，置为当前步骤完成
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


	return;//返回

}
// 封顶拼装 第一步
void CalculateRanging::AdjustRoofSpliceFir(LaserRequest* const request, LaserResponse* const response)
{

}
// 封顶拼装 第二步
void CalculateRanging::AdjustRoofSpliceSec(LaserRequest* const request, LaserResponse* const response)
{

}