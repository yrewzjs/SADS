#ifndef _LASERRESPONSE_H_
#define _LASERRESPONSE_H_
/**
* 该类是一次图像模块对测距模块request后的响应。
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

	enum CylinderAD m_cylinder;  // 油缸
	enum Direction  m_dire;      // 方向

	struct ranging_struct* m_rangingStruct;

public:
	void SetIsFinished(bool isFinished);
	bool GetIsFinished();

	bool hasException();
	void SetHasException(bool isException);

	void SetAllLaserState(enum LaserState state);
	enum LaserState GetAllLaserState();

	void SetRangingStruct(struct ranging_struct rangingStruct); // 值传递
	struct ranging_struct GetRangingStruct();

	void AcquireCylinderAction(enum CylinderAD &cylinder, enum Direction &dire);
	void UpdateCylinder(enum CylinderAD cylinder, enum Direction dire);
	//重置响应
	void ResetResponse();

	CString ToString();
};

#endif