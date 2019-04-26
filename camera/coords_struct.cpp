#include "stdafx.h"
//#include <cmath>
#include <coords_struct.h>
coords_struct_d intS2doubleS(coords_struct CS)
{
	coords_struct_d CSD;
	for (int i = 0; i < cams_count; i++)
	{
		CSD.c_x[i] = CS.c_x[i];
		CSD.c_y[i] = CS.c_y[i];
	}
	CSD._group = CS._group;
	CSD.isfunction = CS.isfunction;
	return CSD;
}
coords_struct doubleS2intS(coords_struct_d CSD)
{
	coords_struct CS;
	for (int i = 0; i < cams_count; i++)
	{
		CS.c_x[i] = round(CSD.c_x[i]);
		CS.c_y[i] = round(CSD.c_y[i]);
	}
	CS._group = CSD._group;
	CS.isfunction = CSD.isfunction;
	return CS;
}
coords_struct doubleS_avar2intS(coords_struct_d CSD, int coords_c)
{
	coords_struct CS;
	for (int i = 0; i < cams_count; i++)
	{
		CS.c_x[i] = round(CSD.c_x[i] / coords_c);
		CS.c_y[i] = round(CSD.c_y[i] / coords_c);
	}
	CS._group = CSD._group;
	CS.isfunction = CSD.isfunction;
	return CS;
}
coords_struct_d doubleSplus(coords_struct_d CSD, coords_struct_d CSDTP)
{
	for (int i = 0; i < cams_count; i++)
	{
		CSD.c_x[i] = CSD.c_x[i] + CSDTP.c_x[i];
		CSD.c_y[i] = CSD.c_y[i] + CSDTP.c_y[i];
	}
	CSD._group = CSDTP._group;
	CSD.isfunction = CSDTP.isfunction;
	return CSD;
}

coords_struct intS_avar2intS(coords_struct CS, int coords_c)
{
	coords_struct CSA;
	for (int i = 0; i < cams_count; i++)
	{
		CSA.c_x[i] = CS.c_x[i] / coords_c;
		CSA.c_y[i] = CS.c_y[i] / coords_c;
	}
	CSA._group = CS._group;
	CSA.isfunction = CS.isfunction;
	return CSA;
}
coords_struct intSplus(coords_struct CS, coords_struct CSTP)
{
	for (int i = 0; i < cams_count; i++)
	{
		CS.c_x[i] = CSTP.c_x[i] + CSTP.c_x[i];
		CS.c_y[i] = CSTP.c_y[i] + CSTP.c_y[i];
	}
	CS._group = CSTP._group;
	CS.isfunction = CSTP.isfunction;
	return CS;
}