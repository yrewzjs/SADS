#ifndef COORDS_STRUCT_H_
#define COORDS_STRUCT_H_
#include <cmath>
//相机数
#define n_qianban 10 
//cams_count代表最多能拍到的三角标数
const int cams_count = 20;
//#define cams_count 12

struct coords_struct
{
	int _group;
	bool isfunction;
	int c_x[cams_count];
	int c_y[cams_count];
};
struct coords_struct_d
{
	int _group;
	bool isfunction;
	double c_x[cams_count];
	double c_y[cams_count];
};
coords_struct_d intS2doubleS(coords_struct CS);
coords_struct doubleS2intS(coords_struct_d CSD);
coords_struct doubleS_avar2intS(coords_struct_d CSD, int coords_c);
coords_struct_d doubleSplus(coords_struct_d CSD, coords_struct_d CSDTP);

coords_struct intSplus(coords_struct CS, coords_struct CSTP);
coords_struct intS_avar2intS(coords_struct CS, int coords_c);
#endif