#ifndef _SADSDEF_H_
#define _SADSDEF_H_

#define SADS_LOCAL_TEST   1;

enum AdjustStep
{
	ADJUST_NOT_START = 0,
	
	//General Catch
	GC_STEP_ZERO= 1,
	GC_STEP_FIR = 2, //��ͨ��Ƭץȡ
	GC_STEP_SEC = 3,
	
	//General
	G_STEP_FIR = 4, //
	G_STEP_SEC = 5,

	//Roof Catch
	RC_STEP_ZERO =6,
	RC_STEP_FIR = 7, //�ⶥ��Ƭץȡ
	RC_STEP_SEC = 8,

	//General
	R_STEP_FIR = 9, //
	R_STEP_SEC = 10,
};

/***
D2025      ��Ƭ
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
	SEG_F = 6, //�ⶥ
};

/***
SegmentAD        ��Ƭ
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
0    δƴװ
1    ��ƴװ
2    ƴװ���
3    ƴװʧ��
*/
enum SegState
{
	UN_SPLICE = 0,
	SPLICING = 1,
	SPLICE_FIN = 2,
	SPLICE_FAILED = 3,
};

#define SEGMENT_NUMBER    7  //��Ƭ����
struct segment_struct
{
	SegState segment_state[SEGMENT_NUMBER];  // ��Ƭ״̬����
};

/***
�͸���Ϣ��ʾ�ṹ�壬����D2XXX�е�˳��
�����±�   �͸�
0           ��    D2011  D2001
1           ��
2           ����
3           ��ת
4           ����
5           ��ҡ  D2016  D2006
*/

#define CYLINDER_NUMBER    6  //�͸�����
struct cylinder_struct
{
	/*
	0    ����
	1    ��(��ת�͸ף�����ת)
	2    ��(��ת�͸ף�����ת)
	*/
	int cylin_action[CYLINDER_NUMBER];  // �͸׶�������

	double cylin_loc[CYLINDER_NUMBER];  // �͸�λ������
};

/***
�ƽ��͸���Ϣ��ʾ�ṹ��
�����±��Ӧ���ƽ��͸�ID
*/
#define THRUST_CYLINDER_NUMBER    19  //�ƽ��͸�����
struct thrust_cylinder_struct
{
	/*
	0    ͣ
	1    ��
	2    ��
	3    ��������λ
	*/
	int cylin_action[THRUST_CYLINDER_NUMBER];  // �͸׶�������
};

enum CaliStep
{
	CALI_GENE_CATCH = 0,   //��ͨ��ץȡcatch�궨
	CALI_GENE_SPLICE = 1,  //��ͨ��ƴ��splice �궨
	CALI_GENE_HEIGHT = 2,  //��ͨ��ץȡͼ��߶�picture hight�궨
	
	CALI_ROOF_CATCH = 3,   //�ⶥ��ץȡ�궨
	CALI_ROOF_SPLICE = 4,  //�ⶥ��ƴ�ӱ궨
	CALI_ROOF_HEIGHT = 5,  //�ⶥ��ץȡͼ��߶�picture hight�궨
};

/*�͸׶�������*/
enum Direction
{
	KEEP = 0,    //���ֲ���
	STRETCH = 1, //�������ͬ�ڸ����͸׵ĸ�����
	WITHDREW = 2,//����
};

//����D503�е��͸�˳��
enum CylinderAD
{
	CLD_SLIDE = 1,		// ����
	CLD_ROTATE = 2,		// ��ת
	CLD_REDBLUE = 3,    // ����ͬ��
	CLD_RED = 4,		 // ��
	CLD_BLUE = 5,		// ��
	CLD_PITCH = 6,		// ����
	CLD_ROLL = 7,		// ��ҡ
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

	static Segment ParseSegment(int segment); // �����ֶ�Ӧ�ڹ�Ƭ

	static SegmentAD TransformSegment(Segment segment); // ����Ƭ��Ӧ�ڵ�����Ƭ���

};


#endif
