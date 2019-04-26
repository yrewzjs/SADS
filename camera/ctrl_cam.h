#ifndef CTRL_CAM_H_
#define CTRL_CAM_H_
#include <coords_struct.h>
#include<value2transit_formchange.h>
#include <CamIP_manage.h>
#include <read_writeXML.h>
#define deflect_err 15
#define slide_err 5
#define rotate_err 5
#define deflect_err_k 10
#define slide_err_k 5
#define rotate_err_k 3
#define deflect_cylinder 3
#define rotate_cylinder 2
#define slide_cylinder 1
#define fin 0
#define extend 2
#define shrink 1

class Ctrl_cam
{
private:
	bool iscapture_ready_r;
	bool isassemture_ready_r;
	int deflect_r;
	int slide_r;
	int rotate_r;
	//coords_struct coords_new;
	coords_struct coords_last;
	coords_struct coords_stcap_n;//标定值
	coords_struct coords_stcap_k;
	coords_struct coords_stassem_n;
	coords_struct coords_stassem_nl;
	coords_struct coords_stassem_nr;
	coords_struct coords_stassem_k;
	int sel_g;
public:
	Ctrl_cam();
	~Ctrl_cam();
	bool readxml2st();//读取配置文件中标定坐标,完成赋值iscapture_ready_r=1/isassemture_ready_r=1
	//void readassem_xml2st();//读取配置文件中标定坐标,完成赋值isassemture_ready_r=1
	command_transit move_action(const coords_struct & newget_coord);//输出油缸抓取控制命令，“XY”：
	//X=1，滑动；X=2，旋转；X=3，横摇。
	//Y=2,伸;Y=1，缩。
	//XY:00,不动。
	void clearcylinder_r();
	//assemcommand_transit assem_move_action(const coords_struct & newget_coord);


};
#endif