#include "stdafx.h"
#include "IO.h"

CIO::CIO()
{
	m_bConnect = false;
}

CIO::~CIO()
{

}

bool CIO::GetIsConnect(){
	return m_bConnect;
}

void CIO::SetIsConnect(bool bl){
	m_bConnect = bl;
}


