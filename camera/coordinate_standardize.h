//�������Ǳ����ĵ�궨
#ifndef COORDINATE_STANDARDIZE_H_
#define COORDINATE_STANDARDIZE_H_
#include <cmath>
#include <coords_struct.h>
#include <read_writeXML.h>

class Coordinate_standardize
{
private:
	int cam_num[cams_count];//��Ҫ�궨�����/���Ǳ���
	int coords_count;
	int coords_total;
	/*int coord_x_queue[12][1000];
	int coord_y_queue[12][1000];
	int coord_x_last[12];
	int coord_y_last[12];
	int coord_x_avarage[12];
	int coord_y_avarage[12];
	double coord_x_total[12];
	double coord_y_total[12];*/
	coords_struct *coord_queue;
	coords_struct coord_last;
	coords_struct coord_avarage;
	coords_struct_d coord_total;
	bool calcu_enable;
	
public:
	Coordinate_standardize(const int count);
	~Coordinate_standardize();
	bool write_avar2xml();
	//void write_avar2cap_xml();//��ץȡ�궨ֵд�������ļ�
	//void write_avar2assem_xml();//��ƴװ�궨ֵд�������ļ�
	bool add(const coords_struct &newget_coord);//ÿһ�λ�����������ֵ����궨����
	void coords_renew();//��ձ궨����
	void show_avar();
	coords_struct calcu_avar();//����궨ƽ��ֵ
	bool isfunctioned();




};
#endif