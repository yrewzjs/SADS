#include "stdafx.h"
#include "LaserRequest.h"
#include "SadsDef.h"

LaserRequest::LaserRequest()
{
	m_AdjustStep = ADJUST_NOT_START;
	m_segmentMark = SEGAD_GENE_FIR;
}

LaserRequest::~LaserRequest()
{
}

void LaserRequest::SetAdjustStep(AdjustStep step)
{
	m_AdjustStep = step;
}

AdjustStep LaserRequest::GetAdjustStep()
{
	return m_AdjustStep;
}

//void LaserRequest::SetCaliStep(CaliStep step)
//{
//	m_CaliStep = step;
//}
//
//CaliStep LaserRequest::GetCaliStep()
//{
//	return m_CaliStep;
//}

void LaserRequest::SetSegmentAD(SegmentAD segMark)
{
	m_segmentMark = segMark;
}

SegmentAD LaserRequest::GetSegmentAD()
{
	return m_segmentMark;
}

void LaserRequest::SetCaliStep(CaliStep step){
	m_CaliStep = step;
}

CaliStep LaserRequest::GetCaliStep(){
	return m_CaliStep;
}