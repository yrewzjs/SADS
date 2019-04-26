#include "stdafx.h"
#include "SadsDef.h"

CString SadsDef::ParseAdjustStep(AdjustStep step){
	CString msg = _T("");
	switch (step)
	{
	case GC_STEP_FIR:
		msg = _T("GC_STEP_FIR");
		break;
	case GC_STEP_SEC:
		msg = _T("GC_STEP_SEC");
		break;
	case G_STEP_FIR:
		msg = _T("G_STEP_FIR");
		break;
	case G_STEP_SEC:
		msg = _T("G_STEP_SEC");
		break;
	default:
		msg = _T("ADJUST_NOT_START");
		break;
	}
	return msg;
}

Segment SadsDef::ParseSegment(int segment)
{
	Segment seg;
	switch (segment)
	{
	case 0:
		seg = SEG_B3;
		break;
	case 1:
		seg = SEG_B2;
		break;
	case 2:
		seg = SEG_B4;
		break;
	case 3:
		seg = SEG_B1;
		break;
	case 4:
		seg = SEG_L2;
		break;
	case 5:
		seg = SEG_L1;
		break;
	case 6:
		seg = SEG_F;
		break;
	default:
		
		break;
	}
	return seg;
}


SegmentAD SadsDef::TransformSegment(Segment segment)
{
	SegmentAD seg;
	switch (segment)
	{
	case SEG_B3:
		seg = SEGAD_GENE_FIR;
		break;
	case SEG_B2:
		seg = SEGAD_GENE_RIGHT;
		break;
	case SEG_B4:
		seg = SEGAD_GENE_LEFT;
		break;
	case SEG_B1:
		seg = SEGAD_GENE_RIGHT;
		break;
	case SEG_L2:
		seg = SEGAD_GENE_LEFT;
		break;
	case SEG_L1:
		seg = SEGAD_GENE_RIGHT;
		break;
	case SEG_F:
		seg = SEGAD_ROOF;
		break;
	}
	return seg;
}