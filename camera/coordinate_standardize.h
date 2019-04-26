//用于三角标中心点标定
#ifndef COORDINATE_STANDARDIZE_H_
#define COORDINATE_STANDARDIZE_H_
#include <cmath>
#include <coords_struct.h>
#include <read_writeXML.h>

class Coordinate_standardize
{
private:
	int cam_num[cams_count];//需要标定的相机/三角标编号
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
	//void write_avar2cap_xml();//将抓取标定值写入配置文件
	//void write_avar2assem_xml();//将拼装标定值写入配置文件
	bool add(const coords_struct &newget_coord);//每一次获得坐标后将坐标值加入标定数组
	void coords_renew();//清空标定数组
	void show_avar();
	coords_struct calcu_avar();//计算标定平均值
	bool isfunctioned();




};
#endif