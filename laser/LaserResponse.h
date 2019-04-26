#ifndef _LASERRESPONSE_H_
#define _LASERRESPONSE_H_
/**
* ������һ��ͼ��ģ��Բ��ģ��request�����Ӧ��
* @author zjs
* @Time 2017
*/
class LaserResponse
{
public:
	LaserResponse();

	~LaserResponse();

private:
	bool m_isFinished;
	bool m_hasException;

	enum LaserState m_allLaserState;

	enum CylinderAD m_cylinder;  // �͸�
	enum Direction  m_dire;      // ����

	struct ranging_struct* m_rangingStruct;

public:
	void SetIsFinished(bool isFinished);
	bool GetIsFinished();

	bool hasException();
	void SetHasException(bool isException);

	void SetAllLaserState(enum LaserState state);
	enum LaserState GetAllLaserState();

	void SetRangingStruct(struct ranging_struct rangingStruct); // ֵ����
	struct ranging_struct GetRangingStruct();

	void AcquireCylinderAction(enum CylinderAD &cylinder, enum Direction &dire);
	void UpdateCylinder(enum CylinderAD cylinder, enum Direction dire);
	//������Ӧ
	void ResetResponse();

	CString ToString();
};

#endif