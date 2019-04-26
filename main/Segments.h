#ifndef _SEGMENTS_H_
#define _SEGMENTS_H_
/**
* 该类是一个管片管理类。
* @author zjs
* @Time 2018
*/


class Segments
{
public:
	Segments();
	~Segments();
private:
	enum SegState* m_pSegment;  // 管片状态数组
public:

	// state：0  未拼装  1 正拼装  2 拼装完成 4 拼装失败
	void SetSegmentState(const enum Segment seg, const enum SegState state);

	// 相当于初始化作用，该管片之前的全部完成拼装，之后的未拼装
	struct segment_struct UpdateAndGetSegments(const enum Segment curSeg, const enum SegState state);

	struct segment_struct getCurSegmentState();
};


#endif



