#include "stdafx.h"
#include "LaserDef.h"

CString LaserDef::ParseDeltaMark(DeltaMark mark){
	CString msg = _T("");
	switch (mark)
	{
	case A2_A0:
		msg = _T("A2_A0");
		break;
	case B2_B0:
		msg = _T("B2_B0");
		break;
	case C2_C0:
		msg = _T("C2_C0");
		break;
	case A2_A1:
		msg = _T("A2_A1");
		break;
	case B2_B1:
		msg = _T("B2_B1");
		break;
	case C2_C1:
		msg = _T("C2_C1");
		break;
	case D1_D0:
		msg = _T("D1_D0");
	case E1_E0:
		msg = _T("E1_E0");
		break;
	case D0_E0:
		msg = _T("D0_E0");
		break;
	case C0_A0:
		msg = _T("C0_A0");
		break;
	case D0_C0:
		msg = _T("D0_C0");
		break;
	case E0_A0:
		msg = _T("E0_A0");
		break;
	case F1_F0:
		msg = _T("F1_F0");
		break;
	case G1_G0:
		msg = _T("G1_G0");
		break;
	}
	return msg;
}

CString LaserDef::ParseSensorMark(int mark){
	CString msg = _T("");
	switch (mark)
	{
	case A0:
		msg = _T("A0");
		break;
	case B0:
		msg = _T("B0");
		break;
	case C0:
		msg = _T("C0");
		break;
	case A1:
		msg = _T("A1");
		break;
	case B1:
		msg = _T("B1");
		break;
	case C1:
		msg = _T("C1");
		break;
	case A2:
		msg = _T("A2");
		break;
	case B2:
		msg = _T("B2");
		break;
	case C2:
		msg = _T("C2");
		break;
	case D0:
		msg = _T("D0");
		break;
	case D1:
		msg = _T("D1");
		break;
	case E0:
		msg = _T("E0");
		break;
	case E1:
		msg = _T("E1");
		break;
	case F0:
		msg = _T("F0");
		break;
	case F1:
		msg = _T("F1");
		break;
	case G0:
		msg = _T("G0");
		break;
	case G1:
		msg = _T("G1");
		break;
	case H0:
		msg = _T("H0");
		break;
	case H1:
		msg = _T("H1");
		break;
	case J0:
		msg = _T("J0");
		break;
	case J1:
		msg = _T("J1");
		break;
	}
	return msg;
}

