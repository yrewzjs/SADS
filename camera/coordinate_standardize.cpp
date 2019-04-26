#include "stdafx.h"
#include  "coordinate_standardize.h"
#include <iostream>

Coordinate_standardize::Coordinate_standardize(int count)
{
	cam_num[cams_count] = 0;
	coords_count = 0;
	coords_total = count;
	coord_queue = new coords_struct[count];
	coord_last = { { 0 } };
	coord_avarage = { { 0 } };
	coord_total = { { 0 } };
	calcu_enable = false;
}

Coordinate_standardize::~Coordinate_standardize()
{
	if (coord_queue != nullptr){
		delete []coord_queue;
		coord_queue = nullptr;
	}
}




void Coordinate_standardize::coords_renew()
{
	cam_num[cams_count] = { 0 };
	coords_count = 0;
	for (int i = 0; i < coords_total; i++)
	{
		coord_queue[i] = { { 0 } };
	}
	coord_last = { { 0 } };
	coord_avarage = { { 0 } };
	coord_total = { { 0 } };
	calcu_enable = false;
}
bool Coordinate_standardize::add(const coords_struct &newget_coord)
{
	/*coord_x_last = x;
	coord_y_last = y;
	coord_x_queue[coords_count] = coord_x_last;
	coord_y_queue[coords_count] = coord_y_last;
	coords_count = coords_count + 1;*/
	if (newget_coord.isfunction == true)
	{
		coord_last = newget_coord;
		coord_queue[coords_count] = coord_last;
		coords_count = coords_count + 1;
		return true;
	}
	else
	{
		return false;
	}

}

bool Coordinate_standardize::isfunctioned()
{
	if (coords_count > 0)
	{
		return true;
	}
	return false;
}

coords_struct Coordinate_standardize::calcu_avar()//计算标定平均值
{
	if (coords_count != 0)
	{
		for (int i = 0; i < coords_count; i++)
		{
			/*coord_x_total = coord_x_total + coord_x_queue[i];
			coord_y_total = coord_y_total + coord_y_queue[i];*/
			coord_total = doubleSplus(coord_total, intS2doubleS(coord_queue[i]));
		}
		coord_avarage = doubleS_avar2intS(coord_total, coords_count);
	}
	return coord_avarage;
}

bool Coordinate_standardize::write_avar2xml()
{
	bool iswrite = writeSTcoords(coord_avarage);
	return iswrite;
}

void Coordinate_standardize::show_avar()
{
	//calcu_avar();
	std::cout << coord_avarage.c_x[0] << "," << coord_avarage.c_y[0] << "," << coord_avarage.c_x[2] << "," << coord_avarage.c_y[2] << "," << std::endl;
}