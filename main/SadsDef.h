#ifndef _SADSDEF_H_
#define _SADSDEF_H_

#define SADS_LOCAL_TEST   1;

enum AdjustStep
{
	ADJUST_NOT_START = 0,
	
	//General Catch
	GC_STEP_ZERO= 1,
	GC_STEP_FIR = 2, //普通管片抓取
	GC_STEP_SEC = 3,
	
	//General
	G_STEP_FIR = 4, //
	G_STEP_SEC = 5,

	//Roof Catch
	RC_STEP_ZERO =6,
	RC_STEP_FIR = 7, //封顶管片抓取
	RC_STEP_SEC = 8,

	//General
	R_STEP_FIR = 9, //
	R_STEP_SEC = 10,
};

/***
D2025      管片
0           B3
1           B2
2           B4
3           B1
4           L2
5           L1
6           F
*/
enum Segment
{
	SEG_B3 = 0,
	SEG_B2 = 1,
	SEG_B4 = 2,
	SEG_B1 = 3,
	SEG_L2 = 4,
	SEG_L1 = 5,
	SEG_F = 6, //封顶
};

/***
SegmentAD        管片
SEGAD_GENE_FIR      B3
SEGAD_GENE_LEFT     B4,L2
SEGAD_GENE_RIGHT    B2,B1,L1
SEGAD_ROOF          F
*/
enum SegmentAD
{
	SEGAD_GENE_FIR = 0,
	SEGAD_GENE_LEFT = 1,
	SEGAD_GENE_RIGHT = 2,
	SEGAD_ROOF = 3,
};

/*
0    未拼装
1    正拼装
2    拼装完成
3    拼装失败
*/
enum SegState
{
	UN_SPLICE = 0,
	SPLICING = 1,
	SPLICE_FIN = 2,
	SPLICE_FAILED = 3,
};

#define SEGMENT_NUMBER    7  //管片数量
struct segment_struct
{
	SegState segment_state[SEGMENT_NUMBER];  // 管片状态数组
};

/***
油缸信息显示结构体，这是D2XXX中的顺序
数组下标   油缸
0           红    D2011  D2001
1           蓝
2           滑动
3           旋转
4           俯仰
5           横摇  D2016  D2006
*/

#define CYLINDER_NUMBER    6  //油缸数量
struct cylinder_struct
{
	/*
	0    不动
	1    伸(旋转油缸：左旋转)
	2    缩(旋转油缸：右旋转)
	*/
	int cylin_action[CYLINDER_NUMBER];  // 油缸动作数组

	double cylin_loc[CYLINDER_NUMBER];  // 油缸位置数组
};

/***
推进油缸信息显示结构体
数组下标对应于推进油缸ID
*/
#define THRUST_CYLINDER_NUMBER    19  //推进油缸数量
struct thrust_cylinder_struct
{
	/*
	0    停
	1    伸
	2    缩
	3    到达缩限位
	*/
	int cylin_action[THRUST_CYLINDER_NUMBER];  // 油缸动作数组
};

enum CaliStep
{
	CALI_GENE_CATCH = 0,   //普通块抓取catch标定
	CALI_GENE_SPLICE = 1,  //普通块拼接splice 标定
	CALI_GENE_HEIGHT = 2,  //普通块抓取图像高度picture hight标定
	
	CALI_ROOF_CATCH = 3,   //封顶块抓取标定
	CALI_ROOF_SPLICE = 4,  //封顶块拼接标定
	CALI_ROOF_HEIGHT = 5,  //封顶块抓取图像高度picture hight标定
};

/*油缸动作方向*/
enum Direction
{
	KEEP = 0,    //保持不动
	STRETCH = 1, //伸出，等同于俯仰油缸的俯动作
	WITHDREW = 2,//缩回
};

//这是D503中的油缸顺序
enum CylinderAD
{
	CLD_SLIDE = 1,		// 滑动
	CLD_ROTATE = 2,		// 旋转
	CLD_REDBLUE = 3,    // 红蓝同步
	CLD_RED = 4,		 // 红
	CLD_BLUE = 5,		// 蓝
	CLD_PITCH = 6,		// 俯仰
	CLD_ROLL = 7,		// 横摇
};

enum ModuleState
{
	UNSTART = 0,
	RUNNING = 1,
	EXCEPTION = 2,
};

class SadsDef
{
public:

	static CString ParseAdjustStep(AdjustStep step);

	static Segment ParseSegment(int segment); // 将数字对应于管片

	static SegmentAD TransformSegment(Segment segment); // 将管片对应于调整管片编号

};


#endif
