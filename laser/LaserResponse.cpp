#include "stdafx.h"

#include "LaserResponse.h"
#include "SadsDef.h"
#include "LaserDef.h"

LaserResponse::LaserResponse()
{
	m_isFinished = false;
	m_hasException = false;
	m_allLaserState = EMITTING;
	m_rangingStruct = nullptr;
}

LaserResponse::~LaserResponse()
{
}

void LaserResponse::ResetResponse()
{
	m_isFinished = false;
	m_hasException = false;
	m_allLaserState = EMITTING;
	m_rangingStruct = nullptr;

	m_cylinder = this->m_cylinder;  // 油缸保持上次动作油缸
	m_dire = KEEP;      // 动作方向保持不动
}

void LaserResponse::SetIsFinished(bool isFinished){
	m_isFinished = isFinished;
}

bool LaserResponse::GetIsFinished()
{
	return m_isFinished;
}

bool LaserResponse::hasException(){
	return m_hasException;
}

void LaserResponse::SetHasException(bool isException)
{
	m_hasException = isException;
}

void LaserResponse::SetAllLaserState(LaserState state){

	m_allLaserState = state;
}

LaserState LaserResponse::GetAllLaserState(){
	return m_allLaserState;
}

void LaserResponse::AcquireCylinderAction(CylinderAD &cylinder, Direction &dire)
{
	cylinder = m_cylinder;
	dire = m_dire;
}

void LaserResponse::UpdateCylinder(CylinderAD cylinder, Direction dire){
	m_cylinder = cylinder;
	m_dire = dire;
}

CString LaserResponse::ToString(){
	CString inf = _T("");

	return inf;
}

void  LaserResponse::SetRangingStruct(ranging_struct rangingStruct)
{
	m_rangingStruct = &rangingStruct;
}

ranging_struct  LaserResponse:: GetRangingStruct()
{
	return *m_rangingStruct;
}