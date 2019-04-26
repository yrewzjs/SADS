#ifndef _CAMARAMODULE_H_
#define _CAMARAMODULE_H_
#include <mutex>
#define CAM_NOT_CONNECT     1589

class IO_cam;
class CamIPg;
class Coordinate_standardize;
class Ctrl_cam;
struct connect_transit;
struct standardize_transit;
struct command_transit;
struct pic_transit;

class CamaraModule
{
public:
	CamaraModule();
	~CamaraModule();

private:
	IO_cam* m_pIO_cam;
	CamIPg* m_pCamIPg;
	Coordinate_standardize* m_pCoordinate_standardize;
	Ctrl_cam* m_pCtrl_cam;
	std::mutex *m_lock;
public:
	connect_transit InitCamaraModule();/*   初始化函数
											{ 
												bool isallconnect;//是否全部连接
											    bool which_NOT_connect[n_qianban];//哪一位返回1则哪号相机有问题
											}*/

	void UninitCamaraModule();         //反初始化函数

	standardize_transit CamaraStandardize(int group, int count);/*标定函数，group=1-6，为选择相机分组
																count表示标定需拍摄多少次，暂定函数内部多次拍照
																{
																	coords_struct CS4show;//输出刚标定的结果供显示
																	bool isSTfin;//表示最终有没有完成标定
																}*/
																
	//bool read_CamaraStandardize_data();

	void Clearcylinder_r();

	command_transit cylinderCommand(int group);/*油缸控制函数，group=1-6，为选择相机分组
											   返回是否接收的坐标结构体完整，油缸种类方向
											   {
											   //输出油缸控制命令，仅在isfunction=true有效
											   int Cylinder;               //X=1，滑动；X=2，旋转；X=3，横摇。=0则不需要移动
											   int dir;					//Y=2,伸;Y=1，缩。
																		//XY:00,完成。
											   bool isfunction;			//传输的坐标结构是否完整，输出命令是否有效
											   coords_struct CS4show;  //输出拍摄的坐标结构体供显示
											   }*/
											   
	pic_transit getpic(int n_cam); /*传输拍摄照片函数，n_cam=0-9，为选中单个相机
									返回发送命令接收图像成功与否以及需要显示的图像信息
									{
										bool isselected;//选中的相机是否连接
										bool isok;      //相机照片是否接收
										CImage pic4show;//输出照片以供显示
									}*/
private:


};



#endif