#include "stdafx.h"
#include <ctrl_cam.h>
#include <cmath>

Ctrl_cam::Ctrl_cam()
{
	iscapture_ready_r = false;
	isassemture_ready_r = false;
	deflect_r = 0;
	slide_r = 0;
	rotate_r = 0;
	//coords_struct coords_new;
	coords_last = { { 0 } };
	coords_stcap_n = { 1, { 0 } };//�궨ֵ
	coords_stcap_k = { 2, { 0 } };
	coords_stassem_n = { 3, { 0 } };
	coords_stassem_nl = { 4, { 0 } };
	coords_stassem_nr = { 5, { 0 } };
	coords_stassem_k = { 6, { 0 } };
	sel_g = 0;
}
Ctrl_cam::~Ctrl_cam()
{

}

bool Ctrl_cam::readxml2st()
{
	bool iscoordsread1 = readSTcoords(coords_stcap_n);
	bool iscoordsread2 = readSTcoords(coords_stcap_k);
	bool iscoordsread3 = readSTcoords(coords_stassem_n);
	bool iscoordsread4 = readSTcoords(coords_stassem_nl);
	bool iscoordsread5 = readSTcoords(coords_stassem_nr);
	bool iscoordsread6 = readSTcoords(coords_stassem_k);
	if (iscoordsread1 == false || iscoordsread2 == false || iscoordsread3 == false
		|| iscoordsread4 == false || iscoordsread5 == false || iscoordsread6 == false)
	{
		iscapture_ready_r = false;
		isassemture_ready_r = false;
		return false;
	}
	iscapture_ready_r = true;
	isassemture_ready_r = true;
	return true;
}

void Ctrl_cam::clearcylinder_r()
{
	deflect_r = 0;
	slide_r = 0;
	rotate_r = 0;
}

command_transit Ctrl_cam::move_action(const coords_struct & newget_coord)
{
	command_transit temp_CCT = { 0, 0, 0, 0 ,0};
	if (newget_coord.isfunction == false)
	{
		return temp_CCT;
	}
	else
	{
		switch (newget_coord._group)
		{
		case capngroup:
			if (iscapture_ready_r == true)//��û�ж�ȡ�궨
			{
				temp_CCT.isfunction = newget_coord.isfunction;
				temp_CCT.CS4show = newget_coord;
				if (abs(coords_stcap_n.c_x[0] - newget_coord.c_x[0]) + abs(coords_stcap_n.c_y[0] - newget_coord.c_y[0]) +
					abs(coords_stcap_n.c_x[1] - newget_coord.c_x[1]) + abs(coords_stcap_n.c_y[1] - newget_coord.c_y[1])>400)
				{
					temp_CCT.issafe = false;
					return temp_CCT;
				}
				temp_CCT.issafe = true;
				int delta_x_0 = coords_stcap_n.c_x[0] - 1920 + coords_stcap_n.c_x[1];
				int delta_y_0 = coords_stcap_n.c_y[0] - 1200 + coords_stcap_n.c_y[1];
				int delta_x_new = newget_coord.c_x[0] - 1920 + newget_coord.c_x[1];
				int delta_y_new = newget_coord.c_y[0] - 1200 + newget_coord.c_y[1];
				//��һ���жϣ�ÿһ���͸׶�����Ϻ��ٶ���ֻ�жϺ����͸׶���
				////////���ж�ƫҡ�͸�
				if (deflect_r == 0 && rotate_r == 0 && slide_r == 0)
				{
					if (delta_x_new < delta_x_0 - deflect_err)
					{
						temp_CCT.Cylinder = deflect_cylinder;
						temp_CCT.dir = shrink;
						return temp_CCT;
						//return 31;
					}
					else if (delta_x_new > delta_x_0 + deflect_err)
					{
						temp_CCT.Cylinder = deflect_cylinder;
						temp_CCT.dir = extend;
						return temp_CCT;
						//return 30;
					}
					else if (delta_x_0 - deflect_err <= delta_x_new && delta_x_new <= delta_x_0 + deflect_err)
						deflect_r = 1;
				}

				//////////����ж���ת���
				if (deflect_r == 1 && rotate_r == 0 && slide_r == 0)
				{
					if (newget_coord.c_y[0] < coords_stcap_n.c_y[0] - rotate_err)
					{
						temp_CCT.Cylinder = rotate_cylinder;
						temp_CCT.dir = extend;
						return temp_CCT;
						//return 21;
					}
					else if (newget_coord.c_y[0] > coords_stcap_n.c_y[0] + rotate_err)
					{
						temp_CCT.Cylinder = rotate_cylinder;
						temp_CCT.dir = shrink;
						return temp_CCT;
						//return 20;
					}
					else if (coords_stcap_n.c_y[0] - rotate_err <= newget_coord.c_y[0] && newget_coord.c_y[0] <= coords_stcap_n.c_y[0] + rotate_err)
						rotate_r = 1;
				}

				///////////����жϻ����͸�
				if (deflect_r == 1 && rotate_r == 1 && slide_r == 0)
				{
					if (newget_coord.c_x[0] < coords_stcap_n.c_x[0] - slide_err)
					{
						temp_CCT.Cylinder = slide_cylinder;
						temp_CCT.dir = shrink;
						return temp_CCT;
						//return 11;
					}
					else if (newget_coord.c_x[0] > coords_stcap_n.c_x[0] + slide_err)
					{
						temp_CCT.Cylinder = slide_cylinder;
						temp_CCT.dir = extend;
						return temp_CCT;
						//return 10;
					}
					else if (coords_stcap_n.c_x[0] - slide_err <= newget_coord.c_x[0] && newget_coord.c_x[0] <= coords_stcap_n.c_x[0] + slide_err)
						slide_r = 1;
				}
				//�ڶ����жϣ�ÿ�ξ���ƫת����ת��������˳�������ж��͸׶���
				if (deflect_r == 1 && rotate_r == 1 && slide_r == 1)
				{
					///////����ƫת�ж�
					if (delta_x_new < delta_x_0 - deflect_err)
					{
						temp_CCT.Cylinder = deflect_cylinder;
						temp_CCT.dir = shrink;
						return temp_CCT;
						//return 31;
					}
					else if (delta_x_new > delta_x_0 + deflect_err)
					{
						temp_CCT.Cylinder = deflect_cylinder;
						temp_CCT.dir = extend;
						return temp_CCT;
						//return 30;
					}
					else if (delta_x_0 - deflect_err <= delta_x_new && delta_x_new <= delta_x_0 + deflect_err)
					{
						////////������ת�ж�
						if (newget_coord.c_y[0] < coords_stcap_n.c_y[0] - rotate_err)
						{
							temp_CCT.Cylinder = rotate_cylinder;
							temp_CCT.dir = extend;
							return temp_CCT;
							//return 21;
						}
						else if (newget_coord.c_y[0] > coords_stcap_n.c_y[0] + rotate_err)
						{
							temp_CCT.Cylinder = rotate_cylinder;
							temp_CCT.dir = shrink;
							return temp_CCT;
							//return 20;
						}
						else if (coords_stcap_n.c_y[0] - rotate_err <= newget_coord.c_y[0] && newget_coord.c_y[0] <= coords_stcap_n.c_y[0] + rotate_err)
						{
							/////////////////////////���뻬���ж�
							if (newget_coord.c_x[0] < coords_stcap_n.c_x[0] - slide_err)
							{
								temp_CCT.Cylinder = slide_cylinder;
								temp_CCT.dir = shrink;
								return temp_CCT;
								//return 11;
							}
							else if (newget_coord.c_x[0] > coords_stcap_n.c_x[0] + slide_err)
							{
								temp_CCT.Cylinder = slide_cylinder;
								temp_CCT.dir = extend;
								return temp_CCT;
								//return 10;
							}
							else if (coords_stcap_n.c_x[0] - slide_err <= newget_coord.c_x[0] && newget_coord.c_x[0] <= coords_stcap_n.c_x[0] + slide_err)
							{
								////////////ȫ����������־λ������͸׶��������Ϣ
								deflect_r = 0;
								rotate_r = 0;
								slide_r = 0;
								temp_CCT.Cylinder = fin;
								temp_CCT.dir = fin;
								return temp_CCT;
							}
						}
					}
				}

				
				//...
				//coords_last = newget_coord;
				//return 00;
			}
			else
				return temp_CCT;
			break;

			case capkgroup:
				if (iscapture_ready_r == true)//��û�ж�ȡ�궨
				{
					temp_CCT.isfunction = newget_coord.isfunction;
					temp_CCT.CS4show = newget_coord;
					if (abs(coords_stcap_k.c_x[6] - newget_coord.c_x[6]) + abs(coords_stcap_k.c_y[6] - newget_coord.c_y[6]) +
						abs(coords_stcap_k.c_x[7] - newget_coord.c_x[7]) + abs(coords_stcap_k.c_y[7] - newget_coord.c_y[7])>400)
					{
						temp_CCT.issafe = false;
						return temp_CCT;
					}
					temp_CCT.issafe = true;
					int delta_x_0 = coords_stcap_k.c_x[6] - coords_stcap_k.c_x[7];
					int delta_y_0 = coords_stcap_k.c_y[6] - coords_stcap_k.c_y[7];
					int delta_x_new = newget_coord.c_x[6] - newget_coord.c_x[7];
					int delta_y_new = newget_coord.c_y[6] - newget_coord.c_y[7];
					//��һ���жϣ�ÿһ���͸׶�����Ϻ��ٶ���ֻ�жϺ����͸׶���
					////////���ж�ƫҡ�͸�
					if (deflect_r == 0 && rotate_r == 0 && slide_r == 0)
					{
						if (delta_y_new < delta_y_0 - deflect_err_k)
						{
							temp_CCT.Cylinder = deflect_cylinder;
							temp_CCT.dir = extend;
							return temp_CCT;
							//return 31;
						}
						else if (delta_y_new > delta_y_0 + deflect_err_k)
						{
							temp_CCT.Cylinder = deflect_cylinder;
							temp_CCT.dir = shrink;
							return temp_CCT;
							//return 30;
						}
						else if (delta_y_0 - deflect_err_k <= delta_y_new && delta_y_new <= delta_y_0 + deflect_err_k)
							deflect_r = 1;
					}

					//////////����ж���ת���
					if (deflect_r == 1 && rotate_r == 0 && slide_r == 0)
					{
						if (newget_coord.c_x[6] < coords_stcap_k.c_x[6] - rotate_err_k)
						{
							temp_CCT.Cylinder = rotate_cylinder;
							temp_CCT.dir = extend;
							return temp_CCT;
							//return 21;
						}
						else if (newget_coord.c_x[6] > coords_stcap_k.c_x[6] + rotate_err_k)
						{
							temp_CCT.Cylinder = rotate_cylinder;
							temp_CCT.dir = shrink;
							return temp_CCT;
							//return 20;
						}
						else if (coords_stcap_k.c_x[6] - rotate_err_k <= newget_coord.c_x[6] && newget_coord.c_x[6] <= coords_stcap_k.c_x[6] + rotate_err_k)
							rotate_r = 1;
					}

					///////////����жϻ����͸�
					if (deflect_r == 1 && rotate_r == 1 && slide_r == 0)
					{
						if (newget_coord.c_y[6] < coords_stcap_k.c_y[6] - slide_err_k)
						{
							temp_CCT.Cylinder = slide_cylinder;
							temp_CCT.dir = extend;
							return temp_CCT;
							//return 11;
						}
						else if (newget_coord.c_y[6] > coords_stcap_k.c_y[6] + slide_err_k)
						{
							temp_CCT.Cylinder = slide_cylinder;
							temp_CCT.dir = shrink;
							return temp_CCT;
							//return 10;
						}
						else if (coords_stcap_k.c_y[6] - slide_err_k <= newget_coord.c_y[6] && newget_coord.c_y[6] <= coords_stcap_k.c_y[6] + slide_err_k)
							slide_r = 1;
					}
					//�ڶ����жϣ�ÿ�ξ���ƫת����ת��������˳�������ж��͸׶���
					if (deflect_r == 1 && rotate_r == 1 && slide_r == 1)
					{
						///////����ƫת�ж�
						if (delta_y_new < delta_y_0 - deflect_err_k)
						{
							temp_CCT.Cylinder = deflect_cylinder;
							temp_CCT.dir = extend;
							return temp_CCT;
							//return 31;
						}
						else if (delta_y_new > delta_y_0 + deflect_err_k)
						{
							temp_CCT.Cylinder = deflect_cylinder;
							temp_CCT.dir = shrink;
							return temp_CCT;
							//return 30;
						}
						else if (delta_y_0 - deflect_err_k <= delta_y_new && delta_y_new <= delta_y_0 + deflect_err_k)
						{
							////////������ת�ж�
							if (newget_coord.c_x[6] < coords_stcap_k.c_x[6] - rotate_err_k)
							{
								temp_CCT.Cylinder = rotate_cylinder;
								temp_CCT.dir = extend;
								return temp_CCT;
								//return 21;
							}
							else if (newget_coord.c_x[6] > coords_stcap_k.c_x[6] + rotate_err_k)
							{
								temp_CCT.Cylinder = rotate_cylinder;
								temp_CCT.dir = shrink;
								return temp_CCT;
								//return 20;
							}
							else if (coords_stcap_k.c_x[6] - rotate_err_k <= newget_coord.c_x[6] && newget_coord.c_x[6] <= coords_stcap_k.c_x[6] + rotate_err_k)
							{
								/////////////////////////���뻬���ж�
								if (newget_coord.c_y[6] < coords_stcap_k.c_y[6] - slide_err_k)
								{
									temp_CCT.Cylinder = slide_cylinder;
									temp_CCT.dir = extend;
									return temp_CCT;
									//return 11;
								}
								else if (newget_coord.c_y[6] > coords_stcap_k.c_y[6] + slide_err_k)
								{
									temp_CCT.Cylinder = slide_cylinder;
									temp_CCT.dir = shrink;
									return temp_CCT;
									//return 10;
								}
								else if (coords_stcap_k.c_y[6] - slide_err_k <= newget_coord.c_y[6] && newget_coord.c_y[6] <= coords_stcap_k.c_y[6] + slide_err_k)
								{
									////////////ȫ����������־λ������͸׶��������Ϣ
									deflect_r = 0;
									rotate_r = 0;
									slide_r = 0;
									temp_CCT.Cylinder = fin;
									temp_CCT.dir = fin;
									return temp_CCT;
								}
							}
						}
					}

					
					//...
					//coords_last = newget_coord;
					//return 00;
				}
				else
					return temp_CCT;
				break;

		default:
			temp_CCT.isfunction = false;
			temp_CCT.issafe = false;
			return temp_CCT;
			break;
		}

	}
}
