#ifndef _SEGMENTS_H_
#define _SEGMENTS_H_
/**
* ������һ����Ƭ�����ࡣ
* @author zjs
* @Time 2018
*/


class Segments
{
public:
	Segments();
	~Segments();
private:
	enum SegState* m_pSegment;  // ��Ƭ״̬����
public:

	// state��0  δƴװ  1 ��ƴװ  2 ƴװ��� 4 ƴװʧ��
	void SetSegmentState(const enum Segment seg, const enum SegState state);

	// �൱�ڳ�ʼ�����ã��ù�Ƭ֮ǰ��ȫ�����ƴװ��֮���δƴװ
	struct segment_struct UpdateAndGetSegments(const enum Segment curSeg, const enum SegState state);

	struct segment_struct getCurSegmentState();
};


#endif



