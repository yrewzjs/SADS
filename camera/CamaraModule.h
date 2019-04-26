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
	connect_transit InitCamaraModule();/*   ��ʼ������
											{ 
												bool isallconnect;//�Ƿ�ȫ������
											    bool which_NOT_connect[n_qianban];//��һλ����1���ĺ����������
											}*/

	void UninitCamaraModule();         //����ʼ������

	standardize_transit CamaraStandardize(int group, int count);/*�궨������group=1-6��Ϊѡ���������
																count��ʾ�궨��������ٴΣ��ݶ������ڲ��������
																{
																	coords_struct CS4show;//����ձ궨�Ľ������ʾ
																	bool isSTfin;//��ʾ������û����ɱ궨
																}*/
																
	//bool read_CamaraStandardize_data();

	void Clearcylinder_r();

	command_transit cylinderCommand(int group);/*�͸׿��ƺ�����group=1-6��Ϊѡ���������
											   �����Ƿ���յ�����ṹ���������͸����෽��
											   {
											   //����͸׿����������isfunction=true��Ч
											   int Cylinder;               //X=1��������X=2����ת��X=3����ҡ��=0����Ҫ�ƶ�
											   int dir;					//Y=2,��;Y=1������
																		//XY:00,��ɡ�
											   bool isfunction;			//���������ṹ�Ƿ���������������Ƿ���Ч
											   coords_struct CS4show;  //������������ṹ�幩��ʾ
											   }*/
											   
	pic_transit getpic(int n_cam); /*����������Ƭ������n_cam=0-9��Ϊѡ�е������
									���ط����������ͼ��ɹ�����Լ���Ҫ��ʾ��ͼ����Ϣ
									{
										bool isselected;//ѡ�е�����Ƿ�����
										bool isok;      //�����Ƭ�Ƿ����
										CImage pic4show;//�����Ƭ�Թ���ʾ
									}*/
private:


};



#endif