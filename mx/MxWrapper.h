#ifndef _MXWRAPPER_H_
#define _MXWRAPPER_H_
/**
* ������PLCģ�����ݶ�ȡ�ؼ�mx�İ�װ����
* @author zjs
* @Time 2018
*/
#define PLC_NOT_CONNECT     1587

class MxWrapper
{
public:
	MxWrapper();
	~MxWrapper();

private:
	class CActUtlType* m_pActUtl;      //PLCͨ��API
	class Synchronized* m_pSynchronized;    // ͬ��

public:

	struct thrust_cylinder_struct getThrustCylinderStruct(struct rplc_struct *rplc);  //��thrust_cylinder_struct�ṹ��ʽ��ȡPLC�е�����

	struct cylinder_struct getCylinderStruct(struct rplc_struct *rplc);  //��cylinder_struct�ṹ��ʽ��ȡPLC�е�����

public:
	//��ʼ�����ƶ���
	void SetActUtlMx(class CActUtlType* pAct);

	long initActUtl(long nLogicNumber);

	long uninitActUtl();

	long readPlc(struct rplc_struct *rplc);

	long write2Plc(const struct wplc_struct *wplc);

public:
	// ΪPLC�Ϸ�ץȡƴװ����  0 ������ 1 ץȡ 2 ƴװ
	long readBitD500(short &D500);

	// ΪPLC�Ϸ���ǰ��Ƭ
	long readBitD2025(short &D2025);

	// �·�����λ����״̬ 0 δ���� 1 ������ 2 ���
	long writeBitD501(short D501);

	// �·�����λδ��� 0������  1���쳣
	long writeBitD511(short D511);

	// �·���λ��״̬ 1������  2���쳣
	long writeBitD514(short D514);

	// �·��͸׶���
	long writeCylinderAction(const enum CylinderAD cld, const enum Direction dir);

	long writeCylinderAction2(enum CylinderAD cld, enum Direction dir);
};

#endif