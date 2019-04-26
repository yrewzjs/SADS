#ifndef _LASERDEF_H_
#define _LASERDEF_H_

#define  SENSOR_REQ_ERROR  _T("")
#define  SENSOR_GROUP_REQ_ERROR  _T("")
#define  SENSOR_REQ_ERROR_DATA  -999.98

/************   激光传感器安装位置与其id的对应关系 ****************
			激光传感器  General布局
D1(10) | D0(09)                E0(11) | E1(12)
	   |	         				  |
	   | C0(06)     B0(03)     A0(00) |
	————————————————
		 C1(07)     B1(04)     A1(01)
		 C2(08)     B2(05)     A2(02)

     		激光传感器  Roof布局
    		———————————
	F1(14)  |  F0(13)     G0(15)  |  G1(16)
	H1(18)  |  H0(17)     J0(19)  |  J1(20)
  
 *************************************/

#define SENSOR_NUMBER  21
/************ 激光传感器 SensorMark  ****************
1、传感器编号是和配置文件强相关的，强烈不同意更改
2、增加传感器或删除传感器，注意对应的配置文件需对应变化。
**/
enum SensorMark
{	//General
	A0 = 0, //A0，传感器id=0
	A1 = 1, //A1  传感器id=1
	A2 = 2, //A2
	B0 = 3, //B0
	B1 = 4, //B1
	B2 = 5, //B2
	C0 = 6, //C0
	C1 = 7, //C1
	C2 = 8, //C2

	D0 = 9,  //D0
	D1 = 10, //D1
	E0 = 11, //E0
	E1 = 12, //E1

	//Roof
	F0 = 13, //F0
	F1 = 14, //F1
	G0 = 15, //G0
	G1 = 16, //G1
	H0 = 17,
	H1 = 18,
	J0 = 19,
	J1 = 20
};

/************ 激光传感器  DeltaMark  ****************
1、DeltaMark编号与Calculate::UpdataDeltaArr()方法强耦合在一起的，强烈不同意更改
2、若发生更改，注意该方法的更改，该方法是数据运算的核心，若出现对应错误等逻辑问题，很难发现弥补。
**/
#define DELTA_NUMBER  18
enum DeltaMark
{	//General
	A2_A0 = 0, //A2-A0，传感器A2 与 传感器A0 测量值做差  被减数在前面
	B2_B0 = 1,
	C2_C0 = 2,
	A2_A1 = 3,
	B2_B1 = 4,
	C2_C1 = 5,

	D1_D0 = 6,
	E1_E0 = 7,

	D0_E0 = 8,
	C0_A0 = 9,
	D0_C0 = 10,
	E0_A0 = 11,
	
	//Roof
	F1_F0 = 12,
	G1_G0 = 13,

	F0_G0 = 14,
	H0_J0 = 15,
	F0_H0 = 16,
	G0_J0 = 17,
};

enum LaserState
{
	STOPPED,  //关闭
	EMITTING,     //开启
	PARTSTOPPED, //部分关闭
};

enum SensorBrand
{
	SICK,      //sick
	KEYENCE,   //keyence
};

struct ranging_struct
{
	double cali_v[SENSOR_NUMBER];  // 标定值数组
	double meas_v[SENSOR_NUMBER];  // 测量值数组
	double real_v[SENSOR_NUMBER];  // 真实值数组
};

class LaserDef
{
public:

	static CString ParseDeltaMark(DeltaMark mark);

	static CString ParseSensorMark(int mark);
};


#endif
