#include "stdafx.h"
#include "Segments.h"

#include "SadsDef.h"

Segments::Segments()
{
	m_pSegment = new SegState[SEGMENT_NUMBER];

	for (int i = 0; i < SEGMENT_NUMBER; i++)
	{
		m_pSegment[i] = UN_SPLICE;
	}
}

Segments::~Segments()
{
	if (m_pSegment != nullptr)
	{
		delete m_pSegment;
		m_pSegment = nullptr;
	}
}

void Segments::SetSegmentState(const Segment seg, const SegState state)
{
	m_pSegment[seg] = state;
}

segment_struct Segments::UpdateAndGetSegments(const Segment curSeg, const SegState state)
{
	for (int i = 0; i < curSeg; i++)
	{
		m_pSegment[i] = SPLICE_FIN;
	}
	m_pSegment[curSeg] = state;

	return getCurSegmentState();
}

segment_struct Segments::getCurSegmentState()
{
	segment_struct segst;

	for (int i = 0; i < SEGMENT_NUMBER; i++)
	{
		segst.segment_state[i] = m_pSegment[i];
	}

	return segst;
}